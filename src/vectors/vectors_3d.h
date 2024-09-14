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

#ifndef VECTORS_3D_H
#define VECTORS_3D_H

extern unsigned char rot_x;
extern unsigned char rot_y;
extern unsigned char rot_z;
extern signed short translate_x;
extern signed short translate_y;
extern signed short translate_z;

#define const_faces_max 8
#define const_points_max 16

#define rot_max 72
#define translate_step 32

extern unsigned char face_points[];
extern unsigned char point_face[];
extern signed short point_x[];
extern signed short point_y[];
extern signed short point_z[];

extern unsigned char next_point;

extern void add_point3(unsigned char f, short x, signed short y, signed short z);
extern void render_points();

#endif