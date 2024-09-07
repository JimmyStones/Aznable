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
// #include <math.h>
// #include <float.h>

// #define M_PI 3.14159265358979323846 // pi

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

void gen_poly(unsigned char cx, unsigned char cy, unsigned char r, unsigned char l)
{
	write_string("what?", colour_cga_yellow, 1, 1);
	//write_stringf("what?", colour_cga_darkred, 1, 2, l);
	// add_line(l, 16, 1);
	// float angleStep = 2 * M_PI / l;

	//write_stringf_float("gp %d", colour_cga_yellow, 0, 2, angleStep);

	// for (unsigned char i = 0; i < l; i++)
	// {
	// 	float xd = r * cosf(i * angleStep);
	// 	float yd = r * sinf(i * angleStep);
	// 	unsigned char x = cx + xd;
	// 	unsigned char y = cy + yd;
	// 	add_point(x, y);
	// }
}

void app_main()
{
	chram_size = chram_cols * chram_rows;
	clear_bgcolor(transparent_char);
	set_default_char_palette();

	add_line(4, 10, 1);
	add_point(16, 16);
	add_point(32, 16);
	add_point(32, 32);
	add_point(16, 32);
	add_point(16, 16);

	// gen_poly(100, 100, 15, 6);
	
	write_string("what?", colour_cga_yellow, 1, 1);

	// add_line(3, 10, 1);
	// add_point(50, 20);
	// add_point(60, 120);
	// add_point(40, 120);
	// // add_point(50, 20);

	// add_line(2, 10, 1);
	// add_point(5, 100);
	// add_point(190, 120);
	// add_point(5, 70);

	// add_line(2, 10, 1);
	// add_point(45, 120);
	// add_point(220, 140);
	// add_point(45, 90);

	// unsigned char f = 0;
	// while (1)
	// {
	// 	vblank = CHECK_BIT(input0, INPUT_VBLANK);
	// 	if (VBLANK_RISING)
	// 	{
	// 		f++;
	// 		write_stringf("%d", colour_cga_yellow, 0, 1, f);
	// 	// 		for (unsigned char i = 2; i < 12; i++)
	// 	// 		{
	// 	// 			vectorram[i]++;
	// 	// 		}
	// 	}
	// 	vblank_last = vblank;
	// }
}