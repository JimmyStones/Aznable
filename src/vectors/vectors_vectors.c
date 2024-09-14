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
#include "vectors_vectors.h"

unsigned char vector_address = 0;

void add_line(unsigned char length, unsigned char intensity, unsigned char colour)
{
	vectorram[vector_address] = length;
	vector_address++;
	vectorram[vector_address] = (intensity) + (colour << 4);
	vector_address++;
}

void add_point(unsigned char x, unsigned char y)
{
	vectorram[vector_address] = x;
	vector_address++;
	vectorram[vector_address] = y;
	vector_address++;
}

