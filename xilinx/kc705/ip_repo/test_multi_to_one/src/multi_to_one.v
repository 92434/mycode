`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2015/08/27 11:07:30
// Design Name: 
// Module Name: multi_to_one
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

module multi_to_one #
    (
        parameter integer COUNT = 32
    )
    (
        input wire clk,
        input wire rst,
        input wire [7 : 0] in_data,
        output wire [7 : 0] result
    );

    reg [7 : 0] data [COUNT : 0];
    reg [7 : 0] in_data_R = 0;
    reg [COUNT : 0] valid = 0;
    integer i;
       
    always @(posedge clk) begin
        if(rst == 0) begin
            for(i = 0;i < COUNT; i=i+1)
            begin
                data[i] <= i + 32;
            end
            
            in_data_R <= 0 + 32;
            valid = 0;
        end
        else begin
            for(i = 0;i < COUNT; i=i+1) begin
                if(in_data == i) begin
                        in_data_R <= data[i];
                        valid[i] <= 1;
                end
                else begin
                    valid[i] <= 0;
                end
            end
        end
    end
    
    assign result = (valid != 0) ? in_data_R : 8'hff;
    
endmodule