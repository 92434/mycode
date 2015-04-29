`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2015/04/07 10:54:57
// Design Name: 
// Module Name: i2s_test
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


module i2s_test #
	(
		parameter integer ATA_WIDTH = 32,
		parameter integer I2S_DATA_BIT_WIDTH = 24,
		parameter integer NUMBER_OF_OUTPUT_WORDS = 8
	)
	();


	wire M_AXIS_ACLK;
	wire M_AXIS_ARESETN;

	wire read_testdata_en;
	wire [23:0] data_source;

	wire bclk;
 	wire read_sync;
 	wire daclrc;
 	wire dacdat;

	wire [I2S_DATA_BIT_WIDTH:0] i2s_received_data;
	wire s_data_valid;

	wire [ATA_WIDTH - 1:0] wdata;
	
	reg debug_1 = 0;


	assign M_AXIS_ARESETN = 1;
	clkgen #(.clk_period(1)) xiaofeiclk(.clk(M_AXIS_ACLK));
	wire read_enable;

	assign read_enable = output_ready;
	



 	data_gen #
		(
		)
		testdata
		(
			.rst(M_AXIS_ARESETN),
			.clk(read_testdata_en),
			.data_source(data_source)
 		);

 	SEND_DATA_TO_WM8731
		sender
		(
			.CLK(M_AXIS_ACLK),
			.RST(M_AXIS_ARESETN),
			.DATA_SOURCE(data_source),
			.BCLK(bclk),
			.READ_SYNC(read_sync),
			.READ_EN(read_testdata_en),
			.DACLRC(daclrc),
			.DACDAT(dacdat)
 		);
	
    always @(posedge bclk) begin
        debug_1 <= ~debug_1;
    end
 	receive_data_from_i2s #
		(
			.I2S_DATA_BIT_WIDTH(24)
		)
		receiver
		(
			.rst(M_AXIS_ARESETN),
			.bclk(bclk),
			.lrclk(daclrc),
			.sd(dacdat),
			.i2s_received_data(i2s_received_data),
			.s_data_valid(s_data_valid)
		);

	assign wdata = {i2s_received_data[I2S_DATA_BIT_WIDTH], {(ATA_WIDTH - 1 - I2S_DATA_BIT_WIDTH){1'b0}}, i2s_received_data[I2S_DATA_BIT_WIDTH - 1 : 0]};


	my_fifo #
		(
			.DATA_WIDTH(ATA_WIDTH),
			.NUMBER_OF_OUTPUT_WORDS(NUMBER_OF_OUTPUT_WORDS)
		)
		xiaofei_fifo
		(
			.rst(M_AXIS_ARESETN),
			.wclk(s_data_valid),
			.rclk(M_AXIS_ACLK),
			.wdata(wdata),
			.rdata(rdata),
			.read_enable(read_enable),//stay 3 cycles
			.output_ready(output_ready),
			.buffer_full_error(buffer_full_error),
			.buffer_empty_error(buffer_empty_error)
		);

endmodule
