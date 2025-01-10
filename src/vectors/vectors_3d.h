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

extern signed char lut_cos_5[];
extern signed char lut_sin_5[];

extern signed short translate_x;
extern signed short translate_y;
extern signed short translate_z;

// #define const_faces_max 24
#define const_points_max 64
#define const_edges_max 64
#define const_faces_max 64
#define const_objects_max 3

#define rot_max 72

extern unsigned char object_firstpoint[];
extern unsigned char object_firstedge[];
extern unsigned char object_firstface[];
extern unsigned char object_points[];
extern unsigned char object_edges[];
extern unsigned char object_faces[];

extern signed short object_pos_x[];
extern signed short object_pos_y[];
extern signed short object_pos_z[];
extern unsigned short object_rot_x[];
extern unsigned short object_rot_y[];
extern unsigned short object_rot_z[];

extern signed short point_x[];
extern signed short point_y[];
extern signed short point_z[];
extern signed short transformed_point_x[];
extern signed short transformed_point_y[];
extern signed short transformed_point_z[];
extern unsigned char point_face1[];
extern unsigned char point_face2[];
extern unsigned char point_face3[];
extern unsigned char point_face4[];

extern unsigned char edge_p1[];
extern unsigned char edge_p2[];

extern signed short face_normal_x[];
extern signed short face_normal_y[];
extern signed short face_normal_z[];

extern unsigned char next_object;
extern unsigned char next_point;
extern unsigned char next_edge;
extern unsigned char next_face;

extern signed short x_min;
extern signed short x_max;
extern signed short y_min;
extern signed short y_max;
extern signed short z_min;
extern signed short z_max;

extern void add_point3d(unsigned char o, signed short x, signed short y, signed short z, unsigned char face1, unsigned char face2, unsigned char face3, unsigned char face4);
extern void add_edge3d(unsigned char o, unsigned char p1, unsigned char p2);
extern void add_face3d(unsigned char o, signed char x, signed char y, signed char z);
extern void scaleObjectDivide(unsigned char o, unsigned char divisor);
extern void render_objects();

#define VERTEX(x, y, z, face1, face2, face3, face4, visibility) add_point3d(next_object, x, y, z, face1, face2, face3, face4);
#define EDGE(v1, v2, face1, face2, visibility) add_edge3d(next_object, v1, v2);
#define FACE(normal_x, normal_y, normal_z, visibility) add_face3d(next_object, normal_x, normal_y, normal_z);

#endif