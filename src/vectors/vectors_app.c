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

// #define M_PI 3.14159265358979323846 // pi
#define M_2PI 6.28318530718 // pi

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

		// write_stringf_short("%3d", colour_cga_white, 0, i, i);
		// write_stringf_short("%3d", colour_cga_white, 8, i, xd);
		// write_stringf_short("%3d", colour_cga_white, 16, i, yd);

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

void app_main()
{
	chram_size = chram_cols * chram_rows;
	clear_bgcolor(transparent_char);
	set_default_char_palette();

	// add_line(4,16,1);
	// add_point(32,32);
	// add_point(64,32);
	// add_point(64,64);
	// add_point(32,64);
	// add_point(32,32);

	// for(unsigned char f=0; f<10; f++){
	// 	unsigned char x = rand_uchar(16, 164);
	// 	unsigned char y = rand_uchar(16, 164);
	// 	unsigned char l = rand_uchar(3, 14);
	// 	unsigned char r = rand_uchar(8, 36);
	// 	gen_poly_i(x, y, r, l);
	// }

	unsigned char a = 0;
	unsigned char p = 16;
	unsigned char pd = 0;
	while (1)
	{
		vblank = CHECK_BIT(input0, INPUT_VBLANK);
		if (VBLANK_RISING)
		{
			v = 0;
			// gen_poly_i(64, 64, 24, 3, a);

			gen_poly_i(128, 128, p, 3, a);
			a += 5;
			if (pd)
			{
				p++;
				if (p == 36)
				{
					pd = !pd;
				}
			}
			else
			{
				p--;
				if (p == 8)
				{
					pd = !pd;
				}
			}
			write_stringf("%d", colour_cga_white, 0, 0, a);
		}
		vblank_last = vblank;
	}
}