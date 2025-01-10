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
#include "vectors_vectors.h"
#include "vectors_3d.h"
#include "vectors_primitives.h"

// DPAD tracker
bool input_left = 0;
bool input_right = 0;
bool input_up = 0;
bool input_down = 0;
bool input_a;
bool input_b;

unsigned char input_speed = 1;
signed char rot_timer_x;
signed char rot_timer_y;
signed char rot_timer_z;
signed char rot_speed_x = 1;
signed char rot_speed_y = 4;
signed char rot_speed_z = 0;
signed short rot_pos_x = 320;
signed short rot_pos_y = 50;
signed short rot_pos_z;
signed char rot_timer_max = 4;

// Track joypad 1 directions and start for menu control
void basic_input()
{
	input_up = CHECK_BIT(joystick[0], 3);
	input_down = CHECK_BIT(joystick[0], 2);
	input_left = CHECK_BIT(joystick[0], 1);
	input_right = CHECK_BIT(joystick[0], 0);
	input_a = CHECK_BIT(joystick[0], 4);
	input_b = CHECK_BIT(joystick[0], 5);
}

void handle_inputs()
{
	rot_speed_x += (input_up ? input_speed : input_down ? -input_speed
														: 0);
	rot_speed_y += (input_right ? input_speed : input_left ? -input_speed
														   : 0);
	rot_speed_z += (input_a ? input_speed : input_b ? -input_speed
													: 0);
}

unsigned char first_face_index;
unsigned char last_face_index;
unsigned char first_point_index;
unsigned char last_point_index;
unsigned char next_face_point;

void app_main()
{
	chram_size = chram_cols * chram_rows;
	clear_bgcolor(transparent_char);
	clear_chars(0);
	set_default_char_palette();

	// object_firstpoint[next_object] = next_point;
	// object_firstedge[next_object] = next_edge;
	// object_pos_x[next_object] = 64;
	// object_pos_y[next_object] = 0;
	// object_pos_z[next_object] = 0;
	// object_rot_x[next_object] = 0;
	// object_rot_y[next_object] = 0;
	// object_rot_z[next_object] = 0;
	// generate_box(16, 32, 24);
	// object_points[next_object] = next_point - object_firstpoint[next_object];
	// object_edges[next_object] = next_edge - object_firstedge[next_object];
	// next_object++;

	object_firstpoint[next_object] = next_point;
	object_firstedge[next_object] = next_edge;
	object_firstface[next_object] = next_face;
	object_pos_x[next_object] = 0;
	object_pos_y[next_object] = 0;
	object_pos_z[next_object] = 0;
	object_rot_x[next_object] = 0;
	object_rot_y[next_object] = 0;
	object_rot_z[next_object] = 0;
	// generate_box(32, 32, 32);
	generate_sidewinder();
	object_points[next_object] = next_point - object_firstpoint[next_object];
	object_edges[next_object] = next_edge - object_firstedge[next_object];
	object_faces[next_object] = next_face - object_firstface[next_object];

	scaleObjectDivide(next_object, 2);

	// first_point_index = object_firstpoint[next_object];
	// last_point_index = first_point_index + object_points[next_object];
	// first_face_index = object_firstface[next_object];
	// last_face_index = first_face_index + object_faces[next_object];
	// for (unsigned char f = first_face_index; f < last_face_index; f++)
	// {
	// 	signed short nx = face_normal_x[f] / 4;
	// 	signed short ny = face_normal_y[f] / 4;
	// 	signed short nz = face_normal_z[f] / 4;
	// 	unsigned char face_points[4] = {0, 0, 0, 0};
	// 	next_face_point = 0;
	// 	for (unsigned char p = first_point_index; p < last_point_index; p++)
	// 	{
	// 		if (point_face1[p] == f)
	// 		{
	// 			face_points[next_face_point] = p;
	// 			next_face_point++;
	// 		}
	// 		if (point_face2[p] == f)
	// 		{
	// 			face_points[next_face_point] = p;
	// 			next_face_point++;
	// 		}
	// 		if (point_face3[p] == f)
	// 		{
	// 			face_points[next_face_point] = p;
	// 			next_face_point++;
	// 		}
	// 		if (point_face4[p] == f)
	// 		{
	// 			face_points[next_face_point] = p;
	// 			next_face_point++;
	// 		}
	// 	}
	// 	signed short c_x = 0;
	// 	signed short c_y = 0;
	// 	signed short c_z = 0;
	// 	unsigned char lfp = next_face_point - 1;
	// 	// write_stringf("f=%d", colour_cga_white, 0, f, f);
	// 	// write_stringf("lf=%d", colour_cga_white, 4, f, last_face_index);
	// 	// write_stringf("p1=%d", colour_cga_white, 11, f, face_points[0]);
	// 	// write_stringf("p2=%d", colour_cga_white, 16, f, face_points[1]);
	// 	// write_stringf("p3=%d", colour_cga_white, 21, f, face_points[2]);
	// 	// write_stringf("p4=%d", colour_cga_white, 26, f, face_points[3]);
	// 	// write_stringf("lfp=%d", colour_cga_white, 31, f, lfp);
	// 	for (int ffp = 0; ffp <= lfp; ffp++)
	// 	{
	// 		unsigned char fpi = face_points[ffp];
	// 		c_x += point_x[fpi];
	// 		c_y += point_y[fpi];
	// 		c_z += point_z[fpi];
	// 	}
	// 	c_x /= lfp;
	// 	c_y /= lfp;
	// 	c_z /= lfp;

	// 	add_point3d(next_object, c_x, c_y, c_z, 0, 0, 0, 0);
	// 	add_point3d(next_object, c_x + nx, c_y + ny, c_z + nz, 0, 0, 0, 0);
	// 	add_edge3d(next_object, next_point - 2, next_point - 1);
	// }

	// object_points[next_object] = next_point - object_firstpoint[next_object];
	// object_edges[next_object] = next_edge - object_firstedge[next_object];
	// object_faces[next_object] = next_face - object_firstface[next_object];
	// next_object++;

	// object_firstpoint[next_object] = next_point;
	// object_firstedge[next_object] = next_edge;
	// object_pos_x[next_object] = -64;
	// object_pos_y[next_object] = -5;
	// object_pos_z[next_object] = -5;
	// object_rot_x[next_object] = 0;
	// object_rot_y[next_object] = 0;
	// object_rot_z[next_object] = 0;
	// generate_box(12, 18, 12);
	// object_points[next_object] = next_point - object_firstpoint[next_object];
	// object_edges[next_object] = next_edge - object_firstedge[next_object];

	vector_address_offset = vector_address;

	timer[0] = 0;
	render_objects();
	timer[1] = 0;

	unsigned char pd = 0;

	while (1)
	{
		basic_input();

		vblank = CHECK_BIT(input0, INPUT_VBLANK);
		if (VBLANK_RISING)
		{
			handle_inputs();

			rot_timer_x -= abs(rot_speed_x);
			if (rot_timer_x < 0)
			{
				rot_timer_x = rot_timer_max;
				rot_pos_x += rot_speed_x;
				if (rot_pos_x < 0)
				{
					rot_pos_x = 359;
				}
				else if (rot_pos_x > 359)
				{
					rot_pos_x = 0;
				}
			}
			rot_timer_y -= abs(rot_speed_y);
			if (rot_timer_y < 0)
			{
				rot_timer_y = rot_timer_max;
				rot_pos_y += rot_speed_y;
				if (rot_pos_y < 0)
				{
					rot_pos_y = 359;
				}
				else if (rot_pos_y > 359)
				{
					rot_pos_y = 0;
				}
			}
			rot_timer_z -= abs(rot_speed_z);
			if (rot_timer_z < 0)
			{
				rot_timer_z = rot_timer_max;
				rot_pos_z += rot_speed_z;
				if (rot_pos_z < 0)
				{
					rot_pos_z = 359;
				}
				else if (rot_pos_z > 359)
				{
					rot_pos_z = 0;
				}
			}

			object_rot_x[0] = rot_pos_x / 5;
			object_rot_y[0] = rot_pos_y / 5;
			object_rot_z[0] = rot_pos_z / 5;

			// pd++;
			// if (pd >= rot_max)
			// {
			// 	pd = 0;
			// }
			// object_pos_y[1] = lut_sin_5[pd];
			// object_pos_y[1] = lut_sin_5[pd];

			// object_rot_x[2] = 71 - object_rot_x[0];
			// object_rot_y[2] = 71 - object_rot_y[0];
			// object_rot_z[2] = 71 - object_rot_z[0];

			// timer[0] = 0;
			render_objects();
			// timer[1] = 0;
		}
		vblank_last = vblank;
	}
}