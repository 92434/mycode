//=============================================================================
//	SEND_DATA_TO_WM8731.v
//	one time send 24 bits data;
//	READ_EN and READ_SYNC as a signal use to get data from RAM or FIFO.
//=============================================================================
//system clock = 50MHz
`timescale 1ns/1ns

module SEND_DATA_TO_WM8731(
	CLK,
	RST,
	DATA_SOURCE,
	BCLK,
	READ_SYNC,
	READ_EN,
	DACLRC,
	DACDAT,
	data_valid,
	data_flag
	);

input CLK;
input RST;
input [23:0] DATA_SOURCE;
output BCLK;
output READ_SYNC;
output READ_EN;
output DACLRC;
output DACDAT;
output data_valid;
output data_flag;
reg BCLK;
reg READ_EN;
reg READ_SYNC;
reg DACLRC;
reg DACDAT;
wire [23:0] DATA_SOURCE;
reg CNT;	//BLCK >= 50 ns
reg [5:0] CNT_BCLK;
reg NEWBCLK;
reg [23:0] DATABUF;
reg data_valid;
reg data_flag;

// DIV system clock to BCLK
always @(posedge CLK or negedge RST) begin
	if(!RST) begin
		CNT <= 1'b0;
		NEWBCLK <= 1'b0;
	end
	else if(CNT==1'b1) begin
		CNT <= 1'b0;
		NEWBCLK <= ~NEWBCLK; //DIV clock
	end
	else
		CNT <= CNT + 1'b1;
end

// BCLK
always @(posedge CLK or negedge RST) begin
	if(!RST) begin
		BCLK <= 1'b0;
		READ_SYNC <= 1'b0;
	end
	else begin
		BCLK <= NEWBCLK;
		READ_SYNC <= NEWBCLK; //read synchronization
	end
end

// DACLRC counter
always @(negedge BCLK or negedge RST) begin
	if(!RST)
		CNT_BCLK <= 6'b0;
	else if(CNT_BCLK == 6'b11_1111)
		CNT_BCLK <= 6'b0;
	else
		CNT_BCLK <= CNT_BCLK + 1'b1;
end

// left channel ,right channel
always @(negedge BCLK) begin
	if((CNT_BCLK>=0)&&(CNT_BCLK<=31)) 
		DACLRC <= 1'b0; // left channel
	else
		DACLRC <= 1'b1; // right chanel
end

always @(negedge BCLK) begin
	if(!RST)
		data_flag <= 0;
	else
		data_flag <= ~data_flag;
end

// buffer
always @(posedge BCLK or negedge RST) begin
	if(!RST) begin
		DACDAT <= 6'b0;
		data_valid <= 0;
	end
	else if(CNT_BCLK>=6'd1 && CNT_BCLK<=6'd24) begin//it's time send left channel
		data_valid <= 1;
		DACDAT <= DATABUF[23]; // shift output
	end
	else if(CNT_BCLK>=6'd33 && CNT_BCLK<=6'd56) begin // send right channel
		data_valid <= 1;
		DACDAT <= DATABUF[23]; // shift output
	end
	else begin
		data_valid <= 0;
		DACDAT <= 6'b0;
	end
end

// shift register
always @(negedge BCLK or negedge RST) begin
	if(!RST) begin
		READ_EN <= 0;
		DATABUF <= 24'h0;
	end
	else if(CNT_BCLK==6'd0 || CNT_BCLK==6'd32) begin
		READ_EN <= 1;	//Read data enable
		DATABUF <= DATA_SOURCE; //get data
		//DATABUF <= temp;
	end
	else if((CNT_BCLK>=6'd1&& CNT_BCLK<=6'd24)||(CNT_BCLK>=6'd33&&CNT_BCLK<=6'd56)) begin
		READ_EN <= 0;
		DATABUF[23:0] <= {DATABUF[22:0],1'b0};
	end
	else
		READ_EN <= 0;
end

endmodule
