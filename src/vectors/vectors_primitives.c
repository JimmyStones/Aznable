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

void generate_sidewinder()
{
	VERTEX(-32, 0, 36, 0, 1, 4, 5, 31)	 // Vertex 0
	VERTEX(32, 0, 36, 0, 2, 5, 6, 31)	 // Vertex 1
	VERTEX(64, 0, -28, 2, 3, 6, 6, 31)	 // Vertex 2
	VERTEX(-64, 0, -28, 1, 3, 4, 4, 31)	 // Vertex 3
	VERTEX(0, 16, -28, 0, 1, 2, 3, 31)	 // Vertex 4
	VERTEX(0, -16, -28, 3, 4, 5, 6, 31)	 // Vertex 5
	VERTEX(-12, 6, -28, 3, 3, 3, 3, 15)	 // Vertex 6
	VERTEX(12, 6, -28, 3, 3, 3, 3, 15)	 // Vertex 7
	VERTEX(12, -6, -28, 3, 3, 3, 3, 12)	 // Vertex 8
	VERTEX(-12, -6, -28, 3, 3, 3, 3, 12) // Vertex 9

	EDGE(0, 1, 0, 5, 31) // Edge 0
	EDGE(1, 2, 2, 6, 31) // Edge 1
	EDGE(1, 4, 0, 2, 31) // Edge 2
	EDGE(0, 4, 0, 1, 31) // Edge 3
	EDGE(0, 3, 1, 4, 31) // Edge 4
	EDGE(3, 4, 1, 3, 31) // Edge 5
	EDGE(2, 4, 2, 3, 31) // Edge 6
	EDGE(3, 5, 3, 4, 31) // Edge 7
	EDGE(2, 5, 3, 6, 31) // Edge 8
	EDGE(1, 5, 5, 6, 31) // Edge 9
	EDGE(0, 5, 4, 5, 31) // Edge 10
	EDGE(6, 7, 3, 3, 15) // Edge 11
	EDGE(7, 8, 3, 3, 12) // Edge 12
	EDGE(6, 9, 3, 3, 12) // Edge 13
	EDGE(8, 9, 3, 3, 12) // Edge 14

	FACE(0, 32, 8, 31)	  // Face 0
	FACE(-12, 47, 6, 31)  // Face 1
	FACE(12, 47, 6, 31)	  // Face 2
	FACE(0, 0, -112, 31)  // Face 3
	FACE(-12, -47, 6, 31) // Face 4
	FACE(0, -32, 8, 31)	  // Face 5
	FACE(12, -47, 6, 31)  // Face 6
}

void generate_box(unsigned char size_x, unsigned char size_y, unsigned char size_z)
{
	// Top  points in order TLF, TRF, TRB, TLB
	VERTEX(-size_x, -size_y, size_z, 0, 3, 4, 9, 0);
	VERTEX(size_x, -size_y, size_z, 0, 3, 5, 9, 0);
	VERTEX(size_x, -size_y, -size_z, 0, 2, 5, 9, 0);
	VERTEX(-size_x, -size_y, -size_z, 0, 2, 4, 9, 0);
	// Bottom points in order BLF, BRF, BRB, BLB
	VERTEX(-size_x, size_y, size_z, 1, 3, 4, 9, 0);
	VERTEX(size_x, size_y, size_z, 1, 3, 5, 9, 0);
	VERTEX(size_x, size_y, -size_z, 1, 2, 5, 9, 0);
	VERTEX(-size_x, size_y, -size_z, 1, 2, 4, 9, 0);

	// Top edges
	EDGE(0, 1, 0, 9, 9);
	EDGE(1, 2, 0, 9, 9);
	EDGE(2, 3, 0, 9, 9);
	EDGE(3, 0, 0, 9, 9);
	// Bottom edges
	EDGE(4, 5, 1, 9, 9);
	EDGE(5, 6, 1, 9, 9);
	EDGE(6, 7, 1, 9, 9);
	EDGE(7, 4, 1, 9, 9);
	// Vert edges
	EDGE(0, 4, 9, 9, 9);
	EDGE(1, 5, 9, 9, 9);
	EDGE(2, 6, 9, 9, 9);
	EDGE(3, 7, 9, 9, 9);

	FACE(0, -50, 0, 0); // Top
	FACE(0, 50, 0, 0);	// Bottom
	FACE(0, 0, -50, 0);	// Front
	FACE(0, 0, 50, 0);	// Back
	FACE(-50, 0, 0, 0);	// Left
	FACE(50, 0, 0, 0);	// Right
}
