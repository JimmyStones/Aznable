/*============================================================================
	Aznable OS - Zorblaxx demo application

	Author: Jim Gregory - https://github.com/JimmyStones/
	Version: 1.0
	Date: 2021-11-27

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

#ifndef ZORBLAXX_APP_H
#define ZORBLAXX_APP_H

// Area and units
extern const unsigned char x_divisor;
extern const unsigned char y_divisor;
extern unsigned short x_min;
extern unsigned short x_max;
extern unsigned short x_h_min;
extern unsigned short x_h_max;
extern unsigned char scroll_speed;

// Sprite ROM image indexes
#define meteor_sprite_index_first 0
#define meteor_sprite_index_count 6
#define player_sprite_index_default meteor_sprite_index_count
#define player_sprite_index_left player_sprite_index_default + 2
#define player_sprite_index_right player_sprite_index_default + 3
#define trail_sprite_index_first player_sprite_index_default + 4
#define trail_sprite_index_last trail_sprite_index_first + 3 
#define explosion_sprite_index_first trail_sprite_index_last + 1
#define explosion_sprite_index_last explosion_sprite_index_first + 7

// Game defaults
extern unsigned short player_spawn_x;
extern unsigned short player_spawn_y;

extern void setup_area();

extern void intro_loop();

extern void game_loop();

extern void app_zorblaxx();

#endif