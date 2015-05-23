`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2015/05/08 16:24:51
// Design Name: 
// Module Name: inout
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module myinout #
    (
    )
    (
        input wire clk,
        output wire oclk
    );
    
    assign oclk = ~clk;
endmodule
