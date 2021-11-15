/*============================================================================
	Aznable OS - Main application

	Author: Jim Gregory - https://github.com/JimmyStones/
	Version: 1.0
	Date: 2021-07-03

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

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "sys.c"
//#include "sys_custom.c"
#include "ps2.c"
#include "ui.c"
//#include "ui_custom.c"
#include "sprite.c"
#include "audio.c"
//#include "app_console.c"
#include "app_meteorstorm.c"

//#define DEBUG_PRINT

void test_loop()
{

	clear_bgcolor(0b01000000);

	starfield[0] = 2;
	starfield[2] = 8;
	starfield[4] = 32;
	char s = 0;
	for (char y = 0; y < 4; y++)
	{
		for (char x = 0; x < 8; x++)
		{
			spr_index[s] = 12 + s;
			s++;
		}
	}

	signed short logoX = 120;
	signed short logoY = 100;

	while (1)
	{
		// hsync = input0 & 0x80;
		// vsync = input0 & 0x40;
		// hblank = input0 & 0x20;
		vblank = input0 & 0x10;

		if (VBLANK_RISING)
		{
			update_sprites();
		}
		if (VBLANK_FALLING)
		{
			char s = 0;

			logoX += 1;
			if (logoX > 336)
			{
				logoX = -112;
			}

			for (char y = 0; y < 4; y++)
			{
				for (char x = 0; x < 8; x++)
				{
					unsigned short lx = logoX + (x * 16);
					spr_on[s] = lx > 0;
					spr_x[s] = lx;
					spr_y[s] = logoY + (y * 16);
					s++;
				}
			}
		}

		// hsync_last = hsync;
		// vsync_last = vsync;
		// hblank_last = hblank;
		vblank_last = vblank;
	}
}

unsigned char player_speed_last = 0;

unsigned char sf_speed1 = 4;
void game_loop()
{

	while (1)
	{
		// hsync = input0 & 0x80;
		// vsync = input0 & 0x40;
		// hblank = input0 & 0x20;
		vblank = input0 & 0x10;

		// console();

		if (VBLANK_RISING)
		{
			unsigned char debug_y = 16;
#ifdef DEBUG_PRINT
			debug_t1 = GET_TIMER;
#endif
			update_sprites();
#ifdef DEBUG_PRINT
			debug_t2 = GET_TIMER;
			write_stringf_ushort("spr: %4d us", 0b01011011, 0, debug_y++, debug_t2 - debug_t1);
#endif
// Update starfield
#ifdef DEBUG_PRINT
			debug_t1 = GET_TIMER;
#endif
			if (player_speed != player_speed_last)
			{
				player_speed_last = player_speed;
				unsigned char s = player_speed / sf_speed1;
				starfield[0] = s;
				s = s / 2;
				starfield[2] = s;
				s = s / 2;
				starfield[4] = s;
			}
#ifdef DEBUG_PRINT
			debug_t2 = GET_TIMER;
			write_stringf_ushort(" sf: %4d us", 0b01011011, 0, debug_y++, debug_t2 - debug_t1);
#endif

			// ay_set_ch(0, 70);
			//  for (unsigned char c = 0; c < 2; c++)
			//  {
			//  	if (channel_on[c])
			//  	{
			//  		channel_tick[c]++;
			//  		if (channel_tick[c] == channel_speed[c])
			//  		{
			//  			channel_pos[c] += channel_dir[c];
			//  			ay_set_ch(c, channel_pos[c]);
			//  			if (channel_pos[c] >= channel_high[c])
			//  			{
			//  				channel_dir[c] = -channel_dir[c];
			//  			}
			//  			if (channel_pos[c] <= channel_low[c])
			//  			{
			//  				channel_dir[c] = -channel_dir[c];
			//  			}
			//  			channel_tick[c] = 0;
			//  		}
			//  	}
			//  }
		}

		if (VBLANK_FALLING)
		{
			unsigned char debug_y = 26;

#ifdef DEBUG_PRINT
			debug_t1 = GET_TIMER;
#endif
			handle_player();
#ifdef DEBUG_PRINT
			debug_t2 = GET_TIMER;
			write_stringf_ushort("ply: %4d us", 0b01011011, 0, debug_y++, debug_t2 - debug_t1);
#endif

#ifdef DEBUG_PRINT
			debug_t1 = GET_TIMER;
#endif
			handle_trails();
#ifdef DEBUG_PRINT
			debug_t2 = GET_TIMER;
			write_stringf_ushort("trl: %4d us", 0b01011011, 0, debug_y++, debug_t2 - debug_t1);
#endif

#ifdef DEBUG_PRINT
			debug_t1 = GET_TIMER;
#endif
			handle_meteors();
#ifdef DEBUG_PRINT
			debug_t2 = GET_TIMER;
			write_stringf_ushort("met: %4d us", 0b01011011, 0, debug_y++, debug_t2 - debug_t1);
#endif

			// Draw player score last
			write_stringf_ushort("%10d", 0xFF, 30, 0, player_score);
		}

		// hsync_last = hsync;
		// vsync_last = vsync;
		// hblank_last = hblank;
		vblank_last = vblank;
	}
}

// Main entry and state machine
void main()
{
	// Setup charmap
	chram_size = chram_cols * chram_rows;
	clear_bgcolor(0);
	clear_chars(0);

	init_audio();
	channel_on[0] = 1;
	ay_set_ch(0, channel_pos[0]);

	setup_area();
	setup_meteors();
	setup_trails();
	setup_player();

#ifdef DEBUG_PRINT
	unsigned short debug_t1;
	unsigned short debug_t2;
#endif

	test_loop();

	game_loop();
}
