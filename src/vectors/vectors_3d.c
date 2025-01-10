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
#include "vectors_3d.h"
#include "vectors_vectors.h"

// unsigned char rot_x = 0;
// unsigned char rot_y = 0;
// unsigned char rot_z = 0;
signed short translate_x = 0;
signed short translate_y = 0;
signed short translate_z = 0;

// signed char lut_cos[36] = {
// 	64, 63, 61, 57, 50, 42, 32, 22, 11, 0,
// 	-11, -22, -32, -42, -50, -57, -61, -63, -64, -63,
// 	-61, -57, -50, -42, -32, -22, -11, 0, 11, 22,
// 	32, 42, 50, 57, 61, 63};
// signed char lut_sin[36] = {
// 	0, 11, 22, 32, 42, 50, 57, 61, 63, 64,
// 	63, 61, 57, 50, 42, 32, 22, 11, 0, -11,
// 	-22, -32, -42, -50, -57, -61, -63, -64, -63, -61,
// 	-57, -50, -42, -32, -22, -11};

signed char lut_cos_5[72] = {64, 64, 63, 62, 60, 58, 55, 52, 49, 45,
							 41, 37, 32, 27, 22, 17, 11, 6, 0, -6,
							 -11, -17, -22, -27, -32, -37, -41, -45, -49, -52,
							 -55, -58, -60, -62, -63, -64, -64, -64, -63, -62,
							 -60, -58, -55, -52, -49, -45, -41, -37, -32, -27,
							 -22, -17, -11, -6, 0, 6, 11, 17, 22, 27,
							 32, 37, 41, 45, 49, 52, 55, 58, 60, 62,
							 63, 64};

signed char lut_sin_5[72] = {0, 6, 11, 17, 22, 27, 32, 37, 41, 45,
							 49, 52, 55, 58, 60, 62, 63, 64, 64, 64,
							 63, 62, 60, 58, 55, 52, 49, 45, 41, 37,
							 32, 27, 22, 17, 11, 6, 0, -6, -11, -17,
							 -22, -27, -32, -37, -41, -45, -49, -52, -55, -58,
							 -60, -62, -63, -64, -64, -64, -63, -62, -60, -58,
							 -55, -52, -49, -45, -41, -37, -32, -27, -22, -17,
							 -11, -6};

unsigned char object_firstpoint[const_objects_max];
unsigned char object_firstedge[const_objects_max];
unsigned char object_firstface[const_objects_max];
unsigned char object_points[const_objects_max];
unsigned char object_edges[const_objects_max];
unsigned char object_faces[const_objects_max];
signed short object_pos_x[const_objects_max];
signed short object_pos_y[const_objects_max];
signed short object_pos_z[const_objects_max];
unsigned short object_rot_x[const_objects_max];
unsigned short object_rot_y[const_objects_max];
unsigned short object_rot_z[const_objects_max];
unsigned char next_object = 0;
unsigned char next_point = 0;
unsigned char next_edge = 0;
unsigned char next_face = 0;

signed short point_x[const_points_max];
signed short point_y[const_points_max];
signed short point_z[const_points_max];
unsigned char point_face1[const_edges_max];
unsigned char point_face2[const_edges_max];
unsigned char point_face3[const_edges_max];
unsigned char point_face4[const_edges_max];
signed short transformed_point_x[const_points_max];
signed short transformed_point_y[const_points_max];
signed short transformed_point_z[const_points_max];

unsigned char edge_p1[const_edges_max];
unsigned char edge_p2[const_edges_max];

signed short face_normal_x[const_faces_max];
signed short face_normal_y[const_faces_max];
signed short face_normal_z[const_faces_max];

signed short dot_product(signed short vx_a, signed short vy_a, signed short vz_a, signed short vx_b, signed short vy_b, signed short vz_b)
{
	short product = 0;
	product += vx_a * vx_b;
	product += vy_a * vy_b;
	product += vz_a * vz_b;
	return product;
}
signed short crossProduct_x;
signed short crossProduct_y;
signed short crossProduct_z;
void crossProduct(signed short vx_a, signed short vy_a, signed short vz_a, signed short vx_b, signed short vy_b, signed short vz_b)
{
	crossProduct_x = vy_a * vz_b - vz_a * vy_b;
	crossProduct_y = vz_a * vx_b - vx_a * vz_b;
	crossProduct_z = vx_a * vy_b - vy_a * vx_b;
}

void add_point3d(unsigned char o, signed short x, signed short y, signed short z, unsigned char face1, unsigned char face2, unsigned char face3, unsigned char face4)
{
	point_x[next_point] = x;
	point_y[next_point] = y;
	point_z[next_point] = z;
	point_face1[next_point] = face1;
	point_face2[next_point] = face2;
	point_face3[next_point] = face3;
	point_face4[next_point] = face4;
	next_point++;
}
void add_edge3d(unsigned char o, unsigned char p1, unsigned char p2)
{
	edge_p1[next_edge] = object_firstpoint[o] + p1;
	edge_p2[next_edge] = object_firstpoint[o] + p2;
	next_edge++;
}
void add_face3d(unsigned char o, signed char normal_x, signed char normal_y, signed char normal_z)
{
	face_normal_x[next_face] = normal_x;
	face_normal_y[next_face] = normal_y;
	face_normal_z[next_face] = normal_z;
	next_face++;
}

void scaleObjectDivide(unsigned char o, unsigned char divisor)
{
	unsigned char fp = object_firstpoint[o];
	unsigned char lp = fp + object_points[o];
	for (unsigned char p = fp; p < lp; p++)
	{
		point_x[p] /= divisor;
		point_y[p] /= divisor;
		point_z[p] /= divisor;
	}
}
signed short cosx;
signed short sinx;
signed short cosy;
signed short siny;
signed short cosz;
signed short sinz;
signed short tx;
signed short ty;
signed short tz;

unsigned char div = 64;
unsigned char mul = 16;
unsigned char zoom_div = 8;
unsigned char zoom_bias = 64;

signed short x_min;
signed short x_max;
signed short y_min;
signed short y_max;
signed short z_min;
signed short z_max;

void transform_point3d(unsigned char p)
{
	signed short r1;
	signed short r2;
	signed short x = point_x[p] * mul;
	signed short y = point_y[p] * mul;
	signed short z = point_z[p] * mul;
	signed short nx = 0;
	signed short ny = 0;
	signed short nz = 0;

	// Y rotation
	//;P'(X) = COS(𝛉) * P(X) - SIN(𝛉) * P(Z)
	r1 = (x * cosy) / div;
	r2 = (z * siny) / div;
	nx = (r1 - r2);
	//;P'(Z) = SIN(𝛉) * P(X) + COS(𝛉) * P(Z)
	r1 = (x * siny) / div;
	r2 = (z * cosy) / div;
	nz = (r1 + r2);
	x = nx;
	z = nz;

	// X rotation
	//;P'(Y) = COS(𝛉) * P(Y) + SIN(𝛉) * P(Z)
	r1 = (y * cosx);
	r2 = (z * sinx);
	ny = (r1 + r2) / div;
	//;P'(Z) = COS(𝛉) * P(Z) - SIN(𝛉) * P(Y)
	r1 = (z * cosx);
	r2 = (y * sinx);
	nz = (r1 - r2) / div;
	y = ny;
	z = nz;

	// Z rotation
	// ;P'(X) = COS(𝛉) * P(X) + SIN(𝛉) * P(Y)
	r1 = (x * cosz);
	r2 = (y * sinz);
	nx = (r1 + r2) / div;
	// ;P'(Y) = COS(𝛉) * P(Y) - SIN(𝛉) * P(X)
	r1 = (y * cosz);
	r2 = (x * sinz);
	ny = (r1 - r2) / div;
	x = nx;
	y = ny;

	if (x > x_max)
	{
		x_max = x;
	}
	if (x < x_min)
	{
		x_min = x;
	}
	if (y > y_max)
	{
		y_max = y;
	}
	if (y < y_min)
	{
		y_min = y;
	}
	if (z > z_max)
	{
		z_max = z;
	}
	if (z < z_min)
	{
		z_min = z;
	}

	transformed_point_x[p] = x;
	transformed_point_y[p] = y;
	transformed_point_z[p] = z;
}

void draw_point3d(unsigned char p)
{
	signed short x = transformed_point_x[p] + tx;
	signed short y = transformed_point_y[p] + ty;
	signed short z = transformed_point_z[p] + tz;
	signed short dx = 2 + (((z / mul) + zoom_bias) / zoom_div);
	signed short dy = (dx * 4) / 3;
	signed short sx = (x / dx) + 128;
	signed short sy = (y / dy) + 128;
	if (sx < 0)
	{
		sx = 0;
	}
	if (sy < 0)
	{
		sy = 0;
	}
	add_point(sx, sy);
}

void render_objects()
{
	vector_address = vector_address_offset;
	for (unsigned char o = 0; o <= next_object; o++)
	{
		// Generate rotates
		unsigned char rot_x = object_rot_x[o];
		unsigned char rot_y = object_rot_y[o];
		unsigned char rot_z = object_rot_z[o];
		cosx = lut_cos_5[rot_x];
		sinx = lut_sin_5[rot_x];
		cosy = lut_cos_5[rot_y];
		siny = lut_sin_5[rot_y];
		cosz = lut_cos_5[rot_z];
		sinz = lut_sin_5[rot_z];
		unsigned char fp = object_firstpoint[o];
		unsigned char lp = fp + object_points[o];
		for (unsigned char p = fp; p < lp; p++)
		{
			transform_point3d(p);
		}

		// Generate translates
		tx = translate_x + (object_pos_x[o] * mul);
		ty = translate_y + (object_pos_y[o] * mul);
		tz = translate_z + (object_pos_z[o] * mul);
		unsigned char fe = object_firstedge[o];
		unsigned char le = fe + object_edges[o];
		for (unsigned char e = fe; e < le; e++)
		{
			add_line(1, 15, 15);
			draw_point3d(edge_p1[e]);
			draw_point3d(edge_p2[e]);
		}
	}
}