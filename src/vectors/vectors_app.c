/*============================================================================
	Aznable OS - Vector demo application

	Author: Jim Gregory - https://github.com/JimmyStones/
	Version: 1.0
	Date: 2024-09-06

	This program is free software; you can redistribute it and/or modify it
	under the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 3 of the License, or (at your option)
	any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along
	with this program. If not, see <http://www.gnu.org/licenses/>.
===========================================================================*/

#include "../shared/sys.h"
#include "../shared/ui.h"
#include "vectors_app.h"
#include <math.h>

unsigned char v = 0;

void add_line(unsigned char length, unsigned char intensity, unsigned char colour)
{
	vectorram[v] = length;
	v++;
	vectorram[v] = (intensity) + (colour << 4);
	v++;
}

void add_point(unsigned char x, unsigned char y)
{
	vectorram[v] = x;
	v++;
	vectorram[v] = y;
	v++;
}

signed char lut_cos[36] = {
	64, 63, 61, 57, 50, 42, 32, 22, 11, 0,
	-11, -22, -32, -42, -50, -57, -61, -63, -64, -63,
	-61, -57, -50, -42, -32, -22, -11, 0, 11, 22,
	32, 42, 50, 57, 61, 63};
signed char lut_sin[36] = {
	0, 11, 22, 32, 42, 50, 57, 61, 63, 64,
	63, 61, 57, 50, 42, 32, 22, 11, 0, -11,
	-22, -32, -42, -50, -57, -61, -63, -64, -63, -61,
	-57, -50, -42, -32, -22, -11};

unsigned char gen_poly_i(unsigned char cx, unsigned char cy, unsigned char r, unsigned char l, unsigned char angle_start)
{
	add_line(l, 16, 1);
	unsigned char ls = v;
	unsigned short a = angle_start;
	unsigned short as = 360 / l;
	for (unsigned char i = 0; i < l; i++)
	{
		unsigned char ai = a / 10;
		signed short xd = (r * lut_cos[ai]);
		signed char xdc = xd / 64;
		signed short yd = (r * lut_sin[ai]);
		signed char ydc = yd / 64;
		vectorram[v] = cx + xdc;
		v++;
		vectorram[v] = cy + ydc;
		v++;
		a += as;
		if (a >= 360)
		{
			a -= 360;
		}
	}
	add_point(vectorram[ls], vectorram[ls + 1]);
	return ls;
}

#define const_faces_max 8
#define const_points_max 16

unsigned char face_points[const_faces_max];
unsigned char point_face[const_points_max];
signed short point_x[const_points_max];
signed short point_y[const_points_max];
signed short point_z[const_points_max];

unsigned char next_point = 0;

void add_point3(unsigned char f, short x, signed short y, signed short z)
{
	point_face[next_point] = f;
	point_x[next_point] = x;
	point_y[next_point] = y;
	point_z[next_point] = z;
	next_point++;
}

// Render points to 2d (angles in 10 deg increments)
unsigned char rot_x = 0;
unsigned char rot_y = 0;
unsigned char rot_z = 0;
// signed short translate_x = 0;
// signed short translate_y = 0;
// signed short translate_z = 0;

// DPAD tracker
bool input_left = 0;
bool input_left_last = 0;
bool input_right = 0;
bool input_right_last = 0;
bool input_up = 0;
bool input_up_last = 0;
bool input_down = 0;
bool input_down_last = 0;
bool input_a;
bool input_a_last = 0;

// Track joypad 1 directions and start for menu control
void basic_input()
{
	input_up_last = input_up;
	input_down_last = input_down;
	input_left_last = input_left;
	input_right_last = input_right;
	input_a_last = input_a;
	input_up = CHECK_BIT(joystick[0], 3);
	input_down = CHECK_BIT(joystick[0], 2);
	input_left = CHECK_BIT(joystick[0], 1);
	input_right = CHECK_BIT(joystick[0], 0);
	input_a = CHECK_BIT(joystick[0], 4);
}

void handle_inputs()
{
	if (!input_a)
	{
		if (input_up)
		{
			rot_x++;
			if (rot_x == 36)
			{
				rot_x = 0;
			}
		}
		if (input_down)
		{
			rot_x--;
			if (rot_x == 255)
			{
				rot_x = 35;
			}
		}
		if (input_right)
		{
			rot_y++;
			if (rot_y == 36)
			{
				rot_y = 0;
			}
		}
		if (input_left)
		{
			rot_y--;
			if (rot_y == 255)
			{
				rot_y = 35;
			}
		}
	}
	// else
	// {
	// 	if (input_up)
	// 	{
	// 		translate_y++;
	// 	}
	// 	if (input_down)
	// 	{
	// 		translate_x++;
	// 	}
	// 	if (input_right)
	// 	{
	// 		translate_x++;
	// 	}
	// 	if (input_left)
	// 	{
	// 		translate_x--;
	// 	}
	// }
}

void app_main()
{
	chram_size = chram_cols * chram_rows;
	clear_bgcolor(transparent_char);
	clear_chars(0);
	set_default_char_palette();

	// Bottom
	unsigned char f = 0;
	face_points[f] = 4;
	add_point3(f, -32, -32, -32);
	add_point3(f, 32, -32, -32);
	add_point3(f, 32, -32, 32);
	add_point3(f, -32, -32, 32);
	f++;
	// Top
	face_points[f] = 4;
	add_point3(f, -32, 32, -32);
	add_point3(f, 32, 32, -32);
	add_point3(f, 32, 32, 32);
	add_point3(f, -32, 32, 32);
	f++;
	// // Left
	face_points[f] = 4;
	add_point3(f, -32, -32, -32);
	add_point3(f, -32, 32, -32);
	add_point3(f, -32, 32, 32);
	add_point3(f, -32, -32, 32);
	f++;
	// Right
	face_points[f] = 4;
	add_point3(f, 32, -32, -32);
	add_point3(f, 32, 32, -32);
	add_point3(f, 32, 32, 32);
	add_point3(f, 32, -32, 32);
	f++;

	unsigned char first_render_point = v;
	while (1)
	{
		basic_input();

		vblank = CHECK_BIT(input0, INPUT_VBLANK);
		if (VBLANK_RISING)
		{

			v = first_render_point;
			unsigned char last_face = 255;
			unsigned char first_point = 0;
			signed short div = 64;
			signed short div2 = 1;
			signed short mul = 8;

			signed short cosx = lut_cos[rot_x];
			signed short sinx = lut_sin[rot_x];
			signed short cosy = lut_cos[rot_y];
			signed short siny = lut_sin[rot_y];
			signed short r1;
			signed short r2;

			for (unsigned char p = 0; p < next_point; p++)
			{
				if (last_face != point_face[p])
				{
					if (last_face != 255)
					{
						add_point(vectorram[first_point], vectorram[first_point + 1]);
					}
					last_face = point_face[p];
					add_line(face_points[last_face] - 1, 16, 1);
					first_point = v;
				}
				signed short x = point_x[p] * mul;
				signed short y = point_y[p] * mul;
				signed short z = point_z[p] * mul;

				signed short nx = 0;
				signed short ny = 0;
				signed short nz = 0;

				// Y rotation
				//;P'(X) = COS(𝛉) * P(X) - SIN(𝛉) * P(Z)
				r1 = (x * cosy) / div;
				r2 = (z * siny) / div;
				nx = (r1 - r2);
				//;P'(Z) = SIN(𝛉) * P(X) + COS(𝛉) * P(Z)
				r1 = (x * siny) / div;
				r2 = (z * cosy) / div;
				nz = (r1 + r2);
				x = nx;
				z = nz;

				// X rotation
				//;P'(Y) = COS(𝛉) * P(Y) + SIN(𝛉) * P(Z)
				r1 = (y * cosx);
				r2 = (z * sinx);
				ny = (r1 + r2) / div;
				//;P'(Z) = COS(𝛉) * P(Z) - SIN(𝛉) * P(Y)
				r1 = (z * cosx);
				r2 = (y * sinx);
				nz = (r1 - r2) / div;
				y = ny;
				z = nz;

				//write_stringf("%d", colour_cga_white, 0, p, p);
				// write_stringf_short("%6d", colour_cga_white, 3, p, x);
				// write_stringf_short("%6d", colour_cga_white, 9, p, y);
				// write_stringf_short("%6d", colour_cga_white, 15, p, z);

				// unsigned short d = 1 + ((z + 64) / 64);
				signed short d = 2 + (((z / mul) + 64) / 8);
				signed short sx = x / d;
				signed short sy = y / d;

				// sx = sx / div2;
				// sy = sy / div2;

				add_point(sx + 128, sy + 128);
			}
			add_point(vectorram[first_point], vectorram[first_point + 1]);

			handle_inputs();

			//write_stringf("%3d", colour_cga_white, 0, 0, rot_x);
			//  write_stringf("%3d", colour_cga_white, 10, 0, rot_y);
			//  write_stringf("%3d", colour_cga_white, 15, 0, rot_z);
		}
		vblank_last = vblank;
	}
}