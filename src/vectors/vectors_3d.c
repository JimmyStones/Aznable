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
#include "vectors_3d.h"
#include "vectors_vectors.h"

unsigned char rot_x = 0;
unsigned char rot_y = 0;
unsigned char rot_z = 0;
// signed short translate_x = 0;
// signed short translate_y = 0;
// signed short translate_z = 0;
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

signed short div = 64;
signed short div2 = 1;
signed short mul = 8;
unsigned char first_render_point;
unsigned char last_face;

void render_points()
{
	vector_address = first_render_point;
	unsigned char first_point = 0;
	last_face = 255;
	signed short cosx = lut_cos[rot_x];
	signed short sinx = lut_sin[rot_x];
	signed short cosy = lut_cos[rot_y];
	signed short siny = lut_sin[rot_y];
	signed short r1;
	signed short r2;

	for (unsigned char p = 0; p < next_point; p++)
	{
		///write_stringf("p %d", colour_cga_white, 0, p, p);
		if (last_face != point_face[p])
		{
			if (last_face != 255)
			{
				add_point(vectorram[first_point], vectorram[first_point + 1]);
			}
			last_face = point_face[p];
			add_line(face_points[last_face] - 1, 16, 1);
			first_point = vector_address;
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

		// write_stringf("%d", colour_cga_white, 0, p, p);
		//  write_stringf_short("%6d", colour_cga_white, 3, p, x);
		//  write_stringf_short("%6d", colour_cga_white, 9, p, y);
		//  write_stringf_short("%6d", colour_cga_white, 15, p, z);

		signed short d = 2 + (((z / mul) + 64) / 8);
		signed short sx = x / d;
		signed short sy = y / d;

		add_point(sx + 128, sy + 128);
	}

	add_point(vectorram[first_point], vectorram[first_point + 1]);
}