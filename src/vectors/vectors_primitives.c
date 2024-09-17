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
#include "vectors_vectors.h"
#include "vectors_3d.h"

void generate_box(unsigned char size_x, unsigned char size_y, unsigned char size_z)
{
	// Top  points in order TLF, TRF, TRB, TLB
	add_point3d(-size_x, -size_y, size_z);
	add_point3d(size_x, -size_y, size_z);
	add_point3d(size_x, -size_y, -size_z);
	add_point3d(-size_x, -size_y, -size_z);
	// Bottom points in order BLF, BRF, BRB, BLB
	add_point3d(-size_x, size_y, size_z);
	add_point3d(size_x, size_y, size_z);
	add_point3d(size_x, size_y, -size_z);
	add_point3d(-size_x, size_y, -size_z);
	
	// Top edges
	add_edge3d(0,1);
	add_edge3d(1,2);
	add_edge3d(2,3);
	add_edge3d(3,0);
	// Bottom edges
	add_edge3d(4,5);
	add_edge3d(5,6);
	add_edge3d(6,7);
	add_edge3d(7,4);
	// Vert edges
	add_edge3d(0,4);
	add_edge3d(1,5);
	add_edge3d(2,6);
	add_edge3d(3,7);

}
