
`timescale 1ns / 1ps

module oddr_2 #
	(
		parameter DDR_ALIGNMENT = "NONE",
		parameter [0:0] INIT = 0,
		parameter SRTYPE = "SYNC"
	)
	
	(
		output reg Q,
		input wire C0,
		input wire C1,
		input wire CE,
		input wire D0,
		input wire D1,
		input wire R,
		input wire S
	);

	always @(posedge C0 or posedge C1) begin
		if(CE == 1) begin
			if(C0 == 1) begin
				Q <= D0;
			end

			if(C1 == 1) begin
				Q <= D1;
			end
		end
	end
endmodule
