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
signed char rot_speed_x = 0;
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

	// rot_speed_x -= abs(rot_speed_x);
	// rot_speed_y -= abs(rot_speed_y);
	// rot_speed_z -= abs(rot_speed_z);
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
	// Left
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

	first_render_point = vector_address;
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

			rot_x = rot_pos_x / 5;
			rot_y = rot_pos_y / 5;
			rot_z = rot_pos_z / 5;

			render_points();
		}
		vblank_last = vblank;
	}
}