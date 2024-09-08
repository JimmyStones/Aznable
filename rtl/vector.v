`timescale 1ps / 1ps
/*============================================================================
	Aznable (custom 8-bit computer system) - Zechs (tilemap)

	Author: Jim Gregory - https://github.com/JimmyStones/
	Version: 0.1
	Date: 2022-01-04

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

module vector #(
	parameter VECTOR_RAM_WIDTH = 9
)(
	input				clk,
	input				ce_pix,
	input				reset,
	input				pause,
	input		[8:0]	hcnt,
	input		[8:0]	vcnt,
	input				hblank,
	input				vblank,

	input		[15:0]	addr,
	input		[7:0]	data_in,
	output		[7:0]	data_out,
	input				write,
	
	output		[7:0]	vector_r,
	output		[7:0]	vector_g,
	output		[7:0]	vector_b,
	output				vector_a
);


// Vector RAM
reg [VECTOR_RAM_WIDTH-1:0] vectorram_sys_addr;
reg [7:0] vectorram_sys_data_out;
dpram #(VECTOR_RAM_WIDTH,8) vectorram
(
	.clock_a(clk),
	.address_a(addr[8:0]),
	.wren_a(write),
	.data_a(data_in),
	.q_a(data_out),

	.clock_b(clk),
	.address_b(vectorram_sys_addr),
	.wren_b(),
	.data_b(),
	.q_b(vectorram_sys_data_out)
);

// Vector Framebuffer - (0x10000 / 65,536 bytes)
// Framebuffer is 256x256 bytes = 65,536
wire [15:0] vectorframeram_read_addr = { vcnt[7:0], hcnt[7:0] };
reg vectorframeram_read_wr;
reg [7:0] vectorframeram_read_data_in;
wire [7:0] vectorframeram_read_data_out;
wire [15:0] vectorframeram_write_addr = { vector_draw_y, vector_draw_x };
reg vectorframeram_write_wr;
reg [7:0] vectorframeram_write_data_in;
wire [7:0] vectorframeram_write_data_out;
dpram #(16,8) vectorframeram
(
	.clock_a(clk),
	.address_a(vectorframeram_read_addr),
	.wren_a(vectorframeram_read_wr),
	.data_a(vectorframeram_read_data_in),
	.q_a(vectorframeram_read_data_out),

	.clock_b(clk),
	.address_b(vectorframeram_write_addr),
	.wren_b(vectorframeram_write_wr),
	.data_b(vectorframeram_write_data_in),
	.q_b(vectorframeram_write_data_out)
);

// VECTOR TEMP
reg [7:0] vector_gfx_out;
reg [1:0] vector_cycle = 0;
reg ce_pix_last;
always @(posedge clk)
begin
	// if(vector_timer_last>0) 
	// begin
	// 	$display("ce_pix: %d c=%d h=%x v=%d  vfw=%d vdo=%d", ce_pix, vector_cycle, hcnt, vcnt, vectorframeram_read_wr, vectorframeram_read_data_out);
	// end
	ce_pix_last <= ce_pix;
	if(hcnt<256 && vcnt<256)
	begin
		case(vector_cycle)
			0:
			begin
				vectorframeram_read_wr <= 0;
			end
			1:
			begin
				vector_gfx_out <= vectorframeram_read_data_out;
			end
			2:
			begin
				vectorframeram_read_data_in <= vector_gfx_out << 1;
				vectorframeram_read_wr <= 1;
			end
			3:
			begin
				vectorframeram_read_wr <= 0;
			end
		endcase
		vector_cycle <= vector_cycle + 1;

		if(ce_pix && !ce_pix_last)
		begin
			vector_cycle <= 0;
		end
	end
	else
	begin
		vector_gfx_out <= 0;
	end
end
assign	vector_r = vector_gfx_out;
assign	vector_g = vector_gfx_out;
assign	vector_b = vector_gfx_out;
assign	vector_a = vector_gfx_out != 0;

// Parameters
localparam VECTOR_STATE_WIDTH = 5;
localparam VECTOR_POINTS_MAX = 8;
localparam VECTOR_POINT_WIDTH = 8;

// State machine constants
localparam VEC_WAITFORVB = 0;
localparam VEC_RESET = 1;
localparam VEC_STARTLOAD = 2;
localparam VEC_GETATTRIBUTES = 3;
localparam VEC_GETPOINTX = 4;
localparam VEC_GETPOINTY = 5;
localparam VEC_STARTDRAW = 6;
localparam VEC_DRAW = 7;
localparam VEC_ENDDRAW = 8;
localparam VEC_WAIT = {VECTOR_STATE_WIDTH{1'b1}};

reg [VECTOR_STATE_WIDTH-1:0]	vector_state = VEC_RESET;
reg [VECTOR_STATE_WIDTH-1:0]	vector_state_next;
reg [7:0]	vector_point_loadindex;
reg [7:0]	vector_point_drawindex;
reg [7:0]	vector_line_index = 0;
reg [7:0]	vector_line_length = 0;
reg [3:0]	vector_line_intensity = 0;
reg [3:0]	vector_line_colour = 0;
reg [(VECTOR_POINTS_MAX * VECTOR_POINT_WIDTH)-1:0]	vector_points_x;
reg [(VECTOR_POINTS_MAX * VECTOR_POINT_WIDTH)-1:0]	vector_points_y;

wire [VECTOR_POINT_WIDTH-1:0] vector_point_x0 = vector_points_x[(vector_point_drawindex*8)+:8];
wire [VECTOR_POINT_WIDTH-1:0] vector_point_x1 = vector_points_x[((vector_point_drawindex+1)*8)+:8];
wire [VECTOR_POINT_WIDTH-1:0] vector_point_y0 = vector_points_y[(vector_point_drawindex*8)+:8];
wire [VECTOR_POINT_WIDTH-1:0] vector_point_y1 = vector_points_y[((vector_point_drawindex+1)*8)+:8];

reg signed [VECTOR_POINT_WIDTH:0] vector_draw_dx;
reg signed [VECTOR_POINT_WIDTH:0] vector_draw_dy;
reg vector_draw_right, vector_draw_down;  

reg [VECTOR_POINT_WIDTH-1:0] vector_draw_x;
reg [VECTOR_POINT_WIDTH-1:0] vector_draw_y;

reg vector_move_x;
reg vector_move_y;
reg signed [VECTOR_POINT_WIDTH:0] vector_err;
reg signed [VECTOR_POINT_WIDTH:0] vector_derr;


reg [15:0] vector_timer = 0;
reg [15:0] vector_timer_last = 0;

reg [31:0] cycle_timer;
reg [25:0] frame_timer;
reg [25:0] blank_timer;


always @(posedge clk) 
begin
	reg vblank_last;
	vblank_last <= vblank;

	cycle_timer <= cycle_timer + 1;

	frame_timer <= (vblank && !vblank_last) ? 26'b0 : frame_timer + 26'b1;
	blank_timer <= (vblank && !vblank_last) ? 26'b0 : vblank ? blank_timer + 26'b1 : blank_timer;

	vector_timer <= vector_timer + 1;

	case (vector_state)
	VEC_WAIT:
	begin
		vector_state <= vector_state_next;
	end
	VEC_WAITFORVB:
	begin
		// Wait for vblank
		if(vblank && !vblank_last)
		begin
			$display("VEC_PASS_FINISHED in %d / last frame=%d last blank=%d", vector_timer, frame_timer, blank_timer);
			vector_timer <= 0;
			vector_state <= VEC_RESET;
		end
	end
	VEC_RESET:
	begin
		// Reset vector renderer
		//$display("VEC_RESET");
		vectorram_sys_addr <= 0;
		vector_point_loadindex <= 0;
		vector_line_index <= 0;
		vector_state_next <= VEC_STARTLOAD;
		vector_state <= VEC_WAIT;
	end
	VEC_STARTLOAD:
	begin
		// Start a new line -> first check for line length
		if(vectorram_sys_data_out>0)
		begin
			vector_point_loadindex <= 0;
			vector_line_index <= vector_line_index + 1;
			vector_line_length <= vectorram_sys_data_out;
			vectorram_sys_addr <= vectorram_sys_addr + 1;
			//$display("VEC_STARTLOAD: vector_line_length: %d", vectorram_sys_data_out);
			vector_state_next <= VEC_GETATTRIBUTES;
			vector_state <= VEC_WAIT;
		end
		else
		begin
			//$display("VEC_STARTLOAD: NO MORE LINES");
			vector_timer_last <= vector_timer;
			vector_state <= VEC_WAITFORVB;
		end
	end
	VEC_GETATTRIBUTES:
	begin
		// Start a new line -> first check for line length
		vector_line_intensity <= vectorram_sys_data_out[3:0];
		vector_line_colour <= vectorram_sys_data_out[7:4];
		vectorram_sys_addr <= vectorram_sys_addr + 1;
		$display("VEC_GETATTRIBUTES: vector_line_intensity: %d vector_line_colour: %d", vectorram_sys_data_out[3:0], vectorram_sys_data_out[7:4]);
		vector_state_next <= VEC_GETPOINTX;
		vector_state <= VEC_WAIT;
	end
	VEC_GETPOINTX:
	begin
		// Add next X position to points
		$display("VEC_GETPOINTX: vector_point_loadindex: %d x: %d", vector_point_loadindex, vectorram_sys_data_out);
		vector_points_x[(vector_point_loadindex*8)+:8] <= vectorram_sys_data_out;
		vectorram_sys_addr <= vectorram_sys_addr + 1;
		vector_state_next <= VEC_GETPOINTY;
		vector_state <= VEC_WAIT;
	end
	VEC_GETPOINTY:
	begin
		// Add next X position to points
		vector_points_y[(vector_point_loadindex*8)+:8] <= vectorram_sys_data_out;
		vectorram_sys_addr <= vectorram_sys_addr + 1;
		//$display("VEC_GETPOINTY: vector_point_loadindex: %d y: %d", vector_point_loadindex, vectorram_sys_data_out);
		if(vector_point_loadindex == vector_line_length)
		begin
			vector_point_drawindex <= 0;
			vector_state <= VEC_STARTDRAW;
		end
		else
		begin
			vector_point_loadindex <= vector_point_loadindex + 1;
			vector_state_next <= VEC_GETPOINTX;
			vector_state <= VEC_WAIT;
		end
	end
	VEC_STARTDRAW:
	begin
		// Calculate deltas for this pair of points
	    vector_draw_right = vector_point_x0 < vector_point_x1;
	    vector_draw_down  = vector_point_y0 < vector_point_y1;
	    vector_draw_dx = vector_draw_right ? vector_point_x1 - vector_point_x0 : vector_point_x0 - vector_point_x1;
	    vector_draw_dy = vector_draw_down  ? vector_point_y0 - vector_point_y1 : vector_point_y1 - vector_point_y0;
		vector_draw_x <= vector_point_x0;
		vector_draw_y <= vector_point_y0;
		vector_err <= vector_draw_dx + vector_draw_dy;
		vector_derr = 0;

		// Start drawing line segment
		$display("VEC_STARTDRAW - l=%d p=%d/%d - %d,%d > %d,%d", vector_line_index, vector_point_drawindex, vector_line_length, vector_point_x0, vector_point_y0, vector_point_x1, vector_point_y1);

		// Load the next line
		vector_state <= VEC_DRAW;
	end
	VEC_DRAW:
	begin
		// Start drawing line segment
		vectorframeram_write_wr = 1'b1;
		vectorframeram_write_data_in = 8'hFF;

		if(vector_draw_x == vector_point_x1 && vector_draw_y == vector_point_y1)
		begin
			if(vector_point_drawindex == vector_line_length - 1)
			begin
				// This segment is done
				vector_state <= VEC_ENDDRAW;
			end
			else
			begin
				vector_point_drawindex <= vector_point_drawindex + 1;
				vector_state <= VEC_STARTDRAW;
			end
		end
		else
		begin
			vector_move_x = (2*vector_err >= vector_draw_dy);
			vector_move_y = (2*vector_err <= vector_draw_dx);
        	vector_derr = vector_move_x ? vector_draw_dy : 0;
        	if (vector_move_y) vector_derr = vector_derr + vector_draw_dx;
			if(vector_move_x) vector_draw_x <= vector_draw_x + (vector_draw_right ? 1 : -1);
			if(vector_move_y) vector_draw_y <= vector_draw_y + (vector_draw_down ? 1 : -1);
			vector_err <= vector_err + vector_derr;
		end

		//$display("VEC_DRAW - l=%d p=%d - right=%d down=%d dx=%d dy=%d x=%d y=%d", vector_line_index, vector_point_drawindex, vector_draw_right, vector_draw_down, vector_draw_dx, vector_draw_dy, vector_draw_x, vector_draw_y);
		// $display("VEC_DRAW - err=%d derr=%d", vector_err, vector_derr);		

	end
	VEC_ENDDRAW:
	begin
		// Finish drawing line segment
		//$display("VEC_ENDDRAW - l=%d p=%d - x=%d y=%d", vector_line_index, vector_point_drawindex, vector_draw_x, vector_draw_y);

		vectorframeram_write_wr = 1'b0;

		// Load the next line
		vector_state <= VEC_STARTLOAD;
	end

	endcase

// 	//$display("h=%d v=%d vec_addr=%d vec_out=%d", hcnt, vcnt, vectorframeram_read_addr, vectorframeram_read_data_out);

end

endmodule