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
signed char rot_speed_x = 4;
signed char rot_speed_y = 0;
signed char rot_speed_z = 0;
signed short rot_pos_x;
signed short rot_pos_y;
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

void app_main()
{
	chram_size = chram_cols * chram_rows;
	clear_bgcolor(transparent_char);
	clear_chars(0);
	set_default_char_palette();

	object_firstpoint[next_object] = next_point;
	object_firstface[next_object] = next_face;
	object_pos_x[next_object] = 64;
	object_pos_y[next_object] = 0;
	object_pos_z[next_object] = 0;
	object_rot_x[next_object] = 0;
	object_rot_y[next_object] = 0;
	object_rot_z[next_object] = 0;
	generate_box(16, 32, 24);
	object_points[next_object] = next_point - object_firstpoint[next_object];
	next_object++;

	object_firstpoint[next_object] = next_point;
	object_firstface[next_object] = next_face;
	object_pos_x[next_object] = 0;
	object_pos_y[next_object] = 0;
	object_pos_z[next_object] = 0;
	object_rot_x[next_object] = 0;
	object_rot_y[next_object] = 0;
	object_rot_z[next_object] = 0;
	generate_box(16, 32, 24);
	object_points[next_object] = next_point - object_firstpoint[next_object];
	next_object++;

	object_firstpoint[next_object] = next_point;
	object_firstface[next_object] = next_face;
	object_pos_x[next_object] = -64;
	object_pos_y[next_object] = -5;
	object_pos_z[next_object] = -5;
	object_rot_x[next_object] = 4;
	object_rot_y[next_object] = 8;
	object_rot_z[next_object] = 16;
	generate_box(16, 16, 16);
	object_points[next_object] = next_point - object_firstpoint[next_object];

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

			pd++;
			if (pd >= rot_max)
			{
				pd = 0;
			}
			object_pos_y[1] = lut_sin_5[pd];

			object_rot_x[2] = 71 - object_rot_x[0];
			object_rot_y[2] = 71 - object_rot_x[1];
			object_rot_z[2] = 71 - object_rot_x[2];
			// object_rot_x[2] = rot_pos_x / 4;
			// object_rot_y[2] = rot_pos_y / 4;
			// object_rot_z[2] = rot_pos_z / 4;

			// object_rot_x[2] = 360 - object_rot_x[0];
			// object_rot_y[2] = 360 - object_rot_y[0];
			// object_rot_z[2] = 360 - object_rot_z[0];

			render_objects();
		}
		vblank_last = vblank;
	}
}