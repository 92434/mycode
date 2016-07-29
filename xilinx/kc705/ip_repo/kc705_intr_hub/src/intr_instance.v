`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2015/08/19 15:27:44
// Design Name: 
// Module Name: intr_instance
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


module intr_instance #
    ()
    (
        input wire clk,
        input wire rst,
        input wire int_i,
        output reg int_o    
    );
    
    reg int_i_old = 0;
    
    always @(posedge clk) begin
        if(rst == 0) begin
            int_o <= 0;
            int_i_old <= 0;
        end
        else begin
            if(int_i != int_i_old) begin
                if(int_i_old == 0) begin
                    int_o <= 1;
                end
                else begin
                    int_o <= 0;
                end
            end
            else begin
                int_o <= 0;
            end
            int_i_old <= int_i;
        end
    end
endmodule
