//=============================================================================
//	SEND_DATA_TO_WM8731.v
//	one time send 24 bits data;
//	READ_EN and READ_SYNC as a signal use to get data from RAM or FIFO.
//=============================================================================
//system clock = 50MHz
`timescale 1ns/1ns

module SEND_DATA_TO_WM8731(
		input wire CLK,
		input wire RST,
		input wire [23:0] DATA_SOURCE,
		output reg BCLK = 1'b0,
		output reg READ_SYNC = 1'b0,
		output reg READ_EN = 0,
		output reg DACLRC = 0,
		output reg DACDAT = 0
	);

	reg clk_div1 = 0;
	reg [5:0] CNT_BCLK = 6'b0;
	reg [23:0] DATABUF;

	always @(posedge CLK) begin
		if(RST == 0) begin
			clk_div1 <= 0;
		end
		else begin
			clk_div1 <= ~clk_div1;
		end
	end

	always @(posedge clk_div1) begin
		if(RST == 0) begin
			BCLK <= 0;
			READ_SYNC <= 0;
		end
		else begin
			BCLK <= ~BCLK;
			READ_SYNC <= ~READ_SYNC;
		end
	end

	always @(negedge BCLK) begin
		if(RST == 0) begin
			DACDAT <= 0;
			DACLRC <= 0;
			DATABUF <= 0;
		end
		else begin
			if((CNT_BCLK>=0)&&(CNT_BCLK<=31)) 
				DACLRC <= 1'b0; // left channel
			else
				DACLRC <= 1'b1; // right chanel

			if(CNT_BCLK==6'd0 || CNT_BCLK==6'd32) begin
			end
			else if(CNT_BCLK==6'd1 || CNT_BCLK==6'd33) begin
				DACDAT <= DATA_SOURCE[23]; // shift output
				DATABUF[23:0] <= {DATA_SOURCE[22:0],1'b0};
			end
			else if(CNT_BCLK>=6'd2 && CNT_BCLK<=6'd24) begin//it's time send left channel
				DACDAT <= DATABUF[23]; // shift output
				DATABUF[23:0] <= {DATABUF[22:0],1'b0};
			end
			else if(CNT_BCLK>=6'd34 && CNT_BCLK<=6'd56) begin // send right channel
				DACDAT <= DATABUF[23]; // shift output
				DATABUF[23:0] <= {DATABUF[22:0],1'b0};
			end
			else begin
				DACDAT <= 0;
				DATABUF <= 0;
			end


			if(CNT_BCLK == 6'b11_1111) begin
				CNT_BCLK <= 6'b0;
			end
			else begin
				CNT_BCLK <= CNT_BCLK + 1'b1;
			end
		end
	end

	always @(posedge BCLK) begin
		if(RST == 0) begin
			READ_EN <= 0;
		end
		else begin
			if(CNT_BCLK==6'd0 || CNT_BCLK==6'd32) begin
				READ_EN <= 1;	//Read data enable
			end
			else begin
				READ_EN <= 0;
			end
		end
	end
endmodule
