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
#include <float.h>

unsigned short count = 0;

unsigned short temp[1024];

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

void app_main()
{
	chram_size = chram_cols * chram_rows;
	clear_chars(0);
	clear_bgcolor(transparent_char);
	set_default_char_palette();

	add_line(4, 10, 1);
	add_point(16, 16);
	add_point(32, 16);
	add_point(32, 32);
	add_point(16, 32);
	add_point(16, 16);

	// add_line(3, 10, 1);
	// add_point(50, 20);
	// add_point(60, 120);
	// add_point(40, 120);
	// add_point(50, 20);

	add_line(2, 10, 1);
	add_point(5,100);
	add_point(190, 120);
	add_point(5,70);

	add_line(2, 10, 1);
	add_point(45, 120);
	add_point(220, 140);
	add_point(45, 90);

	while (1)
	{
		vblank = CHECK_BIT(input0, INPUT_VBLANK);
		if (VBLANK_RISING)
		{
			for (unsigned char i = 2; i < 12; i++)
			{
				vectorram[i]++;
			}
		}
		vblank_last = vblank;
	}
}