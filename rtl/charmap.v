`timescale 1ps / 1ps
/*============================================================================
	Aznable (custom 8-bit computer system) - Casval (character map)

	Author: Jim Gregory - https://github.com/JimmyStones/
	Version: 1.0
	Date: 2021-10-20

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

module charmap (
	input	clk,
	input	reset,
	input	[8:0] hcnt,
	input	[8:0] vcnt,
	input	[7:0] chrom_data_out,
	input	[7:0] fgcolram_data_out,
	input	[7:0] bgcolram_data_out,
	input	[23:0] charpaletteram_data_out,
	input	[7:0] chmap_data_out,
    output	[11:0] chram_addr,
	output	[7:0] charpaletteram_addr_rd,
    output	[11:0] chrom_addr,
	output	[7:0] r,
	output	[7:0] g,
	output	[7:0] b,
	output 		  a
);

// Character map
wire [3:0] chpos_x = 4'd7 - hcnt[2:0];
wire [2:0] chpos_y = vcnt[2:0];
wire [5:0] chram_x = hcnt[8:3];
wire [5:0] chram_y = vcnt[8:3];
assign chram_addr = {chram_y, chram_x};
assign chrom_addr = {1'b0, chmap_data_out[7:0], chpos_y};
wire char_a = chrom_data_out[chpos_x[2:0]];

wire [7:0] palette_index = char_a ? fgcolram_data_out : bgcolram_data_out;
assign charpaletteram_addr_rd = { palette_index };

reg [1:0] cycle = 2'd2;

wire [2:0] r_temp = char_a ? fgcolram_data_out[2:0] : bgcolram_data_out[2:0];
wire [2:0] g_temp = char_a ? fgcolram_data_out[5:3] : bgcolram_data_out[5:3];
wire [1:0] b_temp = char_a ? fgcolram_data_out[7:6] : bgcolram_data_out[7:6];

// always @(posedge clk)
// begin
// 	cycle <= cycle + 1;
// 	if(vcnt == 0)
// 	begin
// 		$display("cycle=%d) hcnt=%d vcnt=%d chram_y=%d chram_x=%d chram_addr=%x chmap_do=%x", cycle, hcnt, vcnt, cycle, chram_y, chram_x, chram_addr, chmap_data_out);
// 		$display("cycle=%d) chpos_x=%d chpos_y=%d chrom_data_out=%x", cycle, chpos_x, chpos_y, chrom_data_out);
// 		$display("cycle=%d) chram_addr=%x palette_index=%x paldo=%x", cycle, chram_addr, palette_index, charpaletteram_data_out);
// 	end
// end

// assign r = {{2{r_temp}},2'b0};
// assign g = {{2{g_temp}},2'b0};
// assign b = {{3{b_temp}},2'b0};
// assign a = char_a ? char_a : (bgcolram_data_out != 8'b11000111);
assign r = charpaletteram_data_out[7:0];
assign g = charpaletteram_data_out[15:8];
assign b = charpaletteram_data_out[23:16];
assign a = char_a ? char_a : (bgcolram_data_out != 8'hFF);



endmodule