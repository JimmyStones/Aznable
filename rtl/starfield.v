// Project F: Ad Astra - Starfield
// (C)2021 Will Green, open source hardware released under the MIT License
// Learn more at https://projectf.io

`default_nettype none
`timescale 1ns / 1ps

module starfield #(
    parameter H=800,
    parameter V=525,
    parameter INC=-1,
    parameter SEED=21'h1FFFFF,
    parameter MASK=21'hFFF
    ) (
    input  wire clk,           // clock
    input  wire en,            // enable
    input  wire rst,           // reset
    output reg  sf_on,         // star on
    output reg  [7:0] sf_star  // star brightness
    );

    localparam RST_CNT = H * V + INC - 1;  // counter starts at zero, so sub 1
    wire [20:0] sf_reg;
    reg  [20:0] sf_cnt;

    always @(posedge clk) begin
        if (en) begin
            sf_cnt <= sf_cnt + 1;
            /* verilator lint_off WIDTH */
            if (sf_cnt == RST_CNT) sf_cnt <= 0;
            /* verilator lint_on WIDTH */
        end
        if (rst) sf_cnt <= 0;
    end

    // select some bits to form stars
    always @(posedge clk) begin
        sf_on = &{sf_reg | MASK};
        sf_star = sf_reg[7:0];
    end

    lfsr #(
        .LEN(21),
        .TAPS(21'b101000000000000000000),
        .SEED(SEED)
        ) lsfr_sf (
        .clk(clk),
        .rst(sf_cnt == 21'b0),
        .en(en),
        .sreg(sf_reg)
    );
endmodule