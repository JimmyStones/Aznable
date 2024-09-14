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
}

void app_main()
{
	chram_size = chram_cols * chram_rows;
	clear_bgcolor(transparent_char);
	clear_chars(0);
	set_default_char_palette();

	timestamp[3] = next_point;
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

	timestamp[3] = next_point;

	first_render_point = vector_address;
	while (1)
	{
		basic_input();

		vblank = CHECK_BIT(input0, INPUT_VBLANK);
		if (VBLANK_RISING)
		{
			render_points();

			handle_inputs();
		}
		vblank_last = vblank;
	}
}