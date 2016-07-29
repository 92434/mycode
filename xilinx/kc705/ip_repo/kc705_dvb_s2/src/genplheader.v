`timescale 1ns / 1ps
module genplheader(
input	          							sys_clk,
input											fs_en,
input	          							rst_n,
//////////////////////////////////////////////////////////////
input	          							frame_vld,
input	          							frame_head,
input	          							xfec_ready,// must earlier 2 cycles than frame_head
input	          		[1:0]				mod_mode,
input	          		[3:0]				ldpc_mode,
input	          							frame_mode,
input	          							pilot_mode,

output 				reg						oe,
output 				reg	signed	[15:0]			symbol_re_out,
output 				reg	signed	[15:0]			symbol_im_out
);

reg							[4:0]			MODCOD;

always @(posedge sys_clk)begin
	if(~rst_n)begin
		MODCOD <= 5'b00000;
	end
	else if(fs_en == 1'b1)begin
		if(xfec_ready == 1'b1)begin
			case(mod_mode)
			2'b00:begin
				MODCOD <= ldpc_mode + 5'b00001;
			end
			2'b01:begin
				if((ldpc_mode>4'b0011)&&(ldpc_mode<4'b0111))begin
					MODCOD <= ldpc_mode + 5'b01000;
				end
				else if(ldpc_mode>4'b0111)begin
					MODCOD <= ldpc_mode + 5'b00111;
				end
				else begin
					MODCOD <= 5'b11101;
				end
			end
			2'b10:begin
				if(ldpc_mode>4'b0100)begin
					MODCOD <= ldpc_mode + 5'b01101;
				end
				else begin
					MODCOD <= 5'b11101;
				end
			end
			2'b11:begin
				if(ldpc_mode>4'b0101)begin
					MODCOD <= ldpc_mode + 5'b10010;
				end
				else begin
					MODCOD <= 5'b11101;
				end
			end
			default:begin
				MODCOD <= 5'b11101;
			end
			endcase
		end
		else begin
			MODCOD <= 5'b00000;
		end
	end
	else begin
	end
end

wire			[1:0]				TYPE_BIT;

assign	TYPE_BIT = {pilot_mode,frame_mode};

wire			[3:0]				Header_y_1msb;
wire			[3:0]				Header_y_2msb;
wire			[3:0]				Header_y_3msb;
wire			[3:0]				Header_y_4msb;
wire			[3:0]				Header_y_5msb;
wire			[3:0]				Header_y_6msb;
wire			[3:0]				Header_y_7msb;
wire			[3:0]				Header_y_8msb;

// matrix y1~y32
assign	Header_y_1msb = {TYPE_BIT[0],(MODCOD[4])^TYPE_BIT[0],(MODCOD[3])^TYPE_BIT[0],(MODCOD[4])^(MODCOD[3])^TYPE_BIT[0]};
assign	Header_y_2msb = {(MODCOD[2])^TYPE_BIT[0],(MODCOD[4])^(MODCOD[2])^TYPE_BIT[0],(MODCOD[3])^(MODCOD[2])^TYPE_BIT[0],(MODCOD[4])^(MODCOD[3])^(MODCOD[2])^TYPE_BIT[0]};
assign	Header_y_3msb = {(MODCOD[1])^TYPE_BIT[0],(MODCOD[4])^(MODCOD[1])^TYPE_BIT[0],(MODCOD[3])^(MODCOD[1])^TYPE_BIT[0],(MODCOD[4])^(MODCOD[3])^(MODCOD[1])^TYPE_BIT[0]};
assign	Header_y_4msb = {(MODCOD[2])^(MODCOD[1])^TYPE_BIT[0],(MODCOD[4])^(MODCOD[2])^(MODCOD[1])^TYPE_BIT[0],(MODCOD[3])^(MODCOD[2])^(MODCOD[1])^TYPE_BIT[0],(MODCOD[4])^(MODCOD[3])^(MODCOD[2])^(MODCOD[1])^TYPE_BIT[0]};
assign	Header_y_5msb = {(MODCOD[0])^TYPE_BIT[0],(MODCOD[4])^(MODCOD[0])^TYPE_BIT[0],(MODCOD[3])^(MODCOD[0])^TYPE_BIT[0],(MODCOD[4])^(MODCOD[3])^(MODCOD[0])^TYPE_BIT[0]};
assign	Header_y_6msb = {(MODCOD[2])^(MODCOD[0])^TYPE_BIT[0],(MODCOD[4])^(MODCOD[2])^(MODCOD[0])^TYPE_BIT[0],(MODCOD[3])^(MODCOD[2])^(MODCOD[0])^TYPE_BIT[0],(MODCOD[4])^(MODCOD[3])^(MODCOD[2])^(MODCOD[0])^TYPE_BIT[0]};
assign	Header_y_7msb = {(MODCOD[1])^(MODCOD[0])^TYPE_BIT[0],(MODCOD[4])^(MODCOD[1])^(MODCOD[0])^TYPE_BIT[0],(MODCOD[3])^(MODCOD[1])^(MODCOD[0])^TYPE_BIT[0],(MODCOD[4])^(MODCOD[3])^(MODCOD[1])^(MODCOD[0])^TYPE_BIT[0]};
assign	Header_y_8msb = {(MODCOD[2])^(MODCOD[1])^(MODCOD[0])^TYPE_BIT[0],(MODCOD[4])^(MODCOD[2])^(MODCOD[1])^(MODCOD[0])^TYPE_BIT[0],(MODCOD[3])^(MODCOD[2])^(MODCOD[1])^(MODCOD[0])^TYPE_BIT[0],(MODCOD[4])^(MODCOD[3])^(MODCOD[2])^(MODCOD[1])^(MODCOD[0])^TYPE_BIT[0]};

// matrix y32~y1
wire			[3:0]				Header_y_1msb_inv;
wire			[3:0]				Header_y_2msb_inv;
wire			[3:0]				Header_y_3msb_inv;
wire			[3:0]				Header_y_4msb_inv;
wire			[3:0]				Header_y_5msb_inv;
wire			[3:0]				Header_y_6msb_inv;
wire			[3:0]				Header_y_7msb_inv;
wire			[3:0]				Header_y_8msb_inv;

assign	Header_y_1msb_inv = {Header_y_1msb[0],Header_y_1msb[1],Header_y_1msb[2],Header_y_1msb[3]};
assign	Header_y_2msb_inv = {Header_y_2msb[0],Header_y_2msb[1],Header_y_2msb[2],Header_y_2msb[3]};
assign	Header_y_3msb_inv = {Header_y_3msb[0],Header_y_3msb[1],Header_y_3msb[2],Header_y_3msb[3]};
assign	Header_y_4msb_inv = {Header_y_4msb[0],Header_y_4msb[1],Header_y_4msb[2],Header_y_4msb[3]};
assign	Header_y_5msb_inv = {Header_y_5msb[0],Header_y_5msb[1],Header_y_5msb[2],Header_y_5msb[3]};
assign	Header_y_6msb_inv = {Header_y_6msb[0],Header_y_6msb[1],Header_y_6msb[2],Header_y_6msb[3]};
assign	Header_y_7msb_inv = {Header_y_7msb[0],Header_y_7msb[1],Header_y_7msb[2],Header_y_7msb[3]};
assign	Header_y_8msb_inv = {Header_y_8msb[0],Header_y_8msb[1],Header_y_8msb[2],Header_y_8msb[3]};

parameter	Header_Scramble_Arrays = 64'b0111000110011101100000111100100101010011010000100010110111111010;

wire			[7:0]				PLS_BIT_1msb;
wire			[7:0]				PLS_BIT_2msb;
wire			[7:0]				PLS_BIT_3msb;
wire			[7:0]				PLS_BIT_4msb;
wire			[7:0]				PLS_BIT_5msb;
wire			[7:0]				PLS_BIT_6msb;
wire			[7:0]				PLS_BIT_7msb;
wire			[7:0]				PLS_BIT_8msb;

wire			[7:0]				PLS_BIT_1msb_inv;
wire			[7:0]				PLS_BIT_2msb_inv;
wire			[7:0]				PLS_BIT_3msb_inv;
wire			[7:0]				PLS_BIT_4msb_inv;
wire			[7:0]				PLS_BIT_5msb_inv;
wire			[7:0]				PLS_BIT_6msb_inv;
wire			[7:0]				PLS_BIT_7msb_inv;
wire			[7:0]				PLS_BIT_8msb_inv;


assign	PLS_BIT_1msb = {Header_y_1msb[3],Header_y_1msb[3]^TYPE_BIT[1],Header_y_1msb[2],Header_y_1msb[2]^TYPE_BIT[1],Header_y_1msb[1],Header_y_1msb[1]^TYPE_BIT[1],Header_y_1msb[0],Header_y_1msb[0]^TYPE_BIT[1]};
assign	PLS_BIT_2msb = {Header_y_2msb[3],Header_y_2msb[3]^TYPE_BIT[1],Header_y_2msb[2],Header_y_2msb[2]^TYPE_BIT[1],Header_y_2msb[1],Header_y_2msb[1]^TYPE_BIT[1],Header_y_2msb[0],Header_y_2msb[0]^TYPE_BIT[1]};
assign	PLS_BIT_3msb = {Header_y_3msb[3],Header_y_3msb[3]^TYPE_BIT[1],Header_y_3msb[2],Header_y_3msb[2]^TYPE_BIT[1],Header_y_3msb[1],Header_y_3msb[1]^TYPE_BIT[1],Header_y_3msb[0],Header_y_3msb[0]^TYPE_BIT[1]};
assign	PLS_BIT_4msb = {Header_y_4msb[3],Header_y_4msb[3]^TYPE_BIT[1],Header_y_4msb[2],Header_y_4msb[2]^TYPE_BIT[1],Header_y_4msb[1],Header_y_4msb[1]^TYPE_BIT[1],Header_y_4msb[0],Header_y_4msb[0]^TYPE_BIT[1]};
assign	PLS_BIT_5msb = {Header_y_5msb[3],Header_y_5msb[3]^TYPE_BIT[1],Header_y_5msb[2],Header_y_5msb[2]^TYPE_BIT[1],Header_y_5msb[1],Header_y_5msb[1]^TYPE_BIT[1],Header_y_5msb[0],Header_y_5msb[0]^TYPE_BIT[1]};
assign	PLS_BIT_6msb = {Header_y_6msb[3],Header_y_6msb[3]^TYPE_BIT[1],Header_y_6msb[2],Header_y_6msb[2]^TYPE_BIT[1],Header_y_6msb[1],Header_y_6msb[1]^TYPE_BIT[1],Header_y_6msb[0],Header_y_6msb[0]^TYPE_BIT[1]};
assign	PLS_BIT_7msb = {Header_y_7msb[3],Header_y_7msb[3]^TYPE_BIT[1],Header_y_7msb[2],Header_y_7msb[2]^TYPE_BIT[1],Header_y_7msb[1],Header_y_7msb[1]^TYPE_BIT[1],Header_y_7msb[0],Header_y_7msb[0]^TYPE_BIT[1]};
assign	PLS_BIT_8msb = {Header_y_8msb[3],Header_y_8msb[3]^TYPE_BIT[1],Header_y_8msb[2],Header_y_8msb[2]^TYPE_BIT[1],Header_y_8msb[1],Header_y_8msb[1]^TYPE_BIT[1],Header_y_8msb[0],Header_y_8msb[0]^TYPE_BIT[1]};

assign	PLS_BIT_1msb_inv = {Header_y_1msb_inv[3],Header_y_1msb_inv[3]^TYPE_BIT[1],Header_y_1msb_inv[2],Header_y_1msb_inv[2]^TYPE_BIT[1],Header_y_1msb_inv[1],Header_y_1msb_inv[1]^TYPE_BIT[1],Header_y_1msb_inv[0],Header_y_1msb_inv[0]^TYPE_BIT[1]};
assign	PLS_BIT_2msb_inv = {Header_y_2msb_inv[3],Header_y_2msb_inv[3]^TYPE_BIT[1],Header_y_2msb_inv[2],Header_y_2msb_inv[2]^TYPE_BIT[1],Header_y_2msb_inv[1],Header_y_2msb_inv[1]^TYPE_BIT[1],Header_y_2msb_inv[0],Header_y_2msb_inv[0]^TYPE_BIT[1]};
assign	PLS_BIT_3msb_inv = {Header_y_3msb_inv[3],Header_y_3msb_inv[3]^TYPE_BIT[1],Header_y_3msb_inv[2],Header_y_3msb_inv[2]^TYPE_BIT[1],Header_y_3msb_inv[1],Header_y_3msb_inv[1]^TYPE_BIT[1],Header_y_3msb_inv[0],Header_y_3msb_inv[0]^TYPE_BIT[1]};
assign	PLS_BIT_4msb_inv = {Header_y_4msb_inv[3],Header_y_4msb_inv[3]^TYPE_BIT[1],Header_y_4msb_inv[2],Header_y_4msb_inv[2]^TYPE_BIT[1],Header_y_4msb_inv[1],Header_y_4msb_inv[1]^TYPE_BIT[1],Header_y_4msb_inv[0],Header_y_4msb_inv[0]^TYPE_BIT[1]};
assign	PLS_BIT_5msb_inv = {Header_y_5msb_inv[3],Header_y_5msb_inv[3]^TYPE_BIT[1],Header_y_5msb_inv[2],Header_y_5msb_inv[2]^TYPE_BIT[1],Header_y_5msb_inv[1],Header_y_5msb_inv[1]^TYPE_BIT[1],Header_y_5msb_inv[0],Header_y_5msb_inv[0]^TYPE_BIT[1]};
assign	PLS_BIT_6msb_inv = {Header_y_6msb_inv[3],Header_y_6msb_inv[3]^TYPE_BIT[1],Header_y_6msb_inv[2],Header_y_6msb_inv[2]^TYPE_BIT[1],Header_y_6msb_inv[1],Header_y_6msb_inv[1]^TYPE_BIT[1],Header_y_6msb_inv[0],Header_y_6msb_inv[0]^TYPE_BIT[1]};
assign	PLS_BIT_7msb_inv = {Header_y_7msb_inv[3],Header_y_7msb_inv[3]^TYPE_BIT[1],Header_y_7msb_inv[2],Header_y_7msb_inv[2]^TYPE_BIT[1],Header_y_7msb_inv[1],Header_y_7msb_inv[1]^TYPE_BIT[1],Header_y_7msb_inv[0],Header_y_7msb_inv[0]^TYPE_BIT[1]};
assign	PLS_BIT_8msb_inv = {Header_y_8msb_inv[3],Header_y_8msb_inv[3]^TYPE_BIT[1],Header_y_8msb_inv[2],Header_y_8msb_inv[2]^TYPE_BIT[1],Header_y_8msb_inv[1],Header_y_8msb_inv[1]^TYPE_BIT[1],Header_y_8msb_inv[0],Header_y_8msb_inv[0]^TYPE_BIT[1]};



wire			[63:0]				PLS_BIT_Array;      
wire			[63:0]				PLS_BIT_Array_inv;

assign	PLS_BIT_Array = {PLS_BIT_1msb,PLS_BIT_2msb,PLS_BIT_3msb,PLS_BIT_4msb,PLS_BIT_5msb,PLS_BIT_6msb,PLS_BIT_7msb,PLS_BIT_8msb};// matrix y1~y32
assign	PLS_BIT_Array_inv = {PLS_BIT_8msb_inv,PLS_BIT_7msb_inv,PLS_BIT_6msb_inv,PLS_BIT_5msb_inv,PLS_BIT_4msb_inv,PLS_BIT_3msb_inv,PLS_BIT_2msb_inv,PLS_BIT_1msb_inv};// matrix y32~y1


reg				[63:0]				PLS_BIT;

always @(posedge sys_clk)begin
	if(~rst_n)begin
		PLS_BIT <= 64'h0;
	end
	else if(fs_en == 1'b1)begin
		PLS_BIT <= PLS_BIT_Array^Header_Scramble_Arrays;    
		//PLS_BIT <= PLS_BIT_Array_inv^Header_Scramble_Arrays;

	end
	else begin
	end
end
	
	
wire			[89:0]				PL_HEADER;	

//assign	PL_HEADER = {26'h18D2E82,PLS_BIT^{64{(~TYPE_BIT[1])}}};

assign	PL_HEADER = {26'h18D2E82,PLS_BIT};

reg								symbol_cnt_reg;

always @(posedge sys_clk)begin
	if(~rst_n)begin
		symbol_cnt_reg <= 1'h0;
	end
	else if(fs_en == 1'b1)begin
		if(frame_head == 1'b1)begin
			symbol_cnt_reg <= 1'h1;
		end
		else if(frame_vld == 1'b1)begin
			symbol_cnt_reg <= ~symbol_cnt_reg;
		end
	end
	else begin
	end
end

reg			[89:0]				PL_HEADER_shift;

always @(posedge sys_clk)begin
	if(~rst_n)begin
		PL_HEADER_shift <= 90'h00;
	end
	else if(fs_en == 1'b1)begin
		if(frame_head == 1'b1)begin
			PL_HEADER_shift <= PL_HEADER;
		end
		else if(frame_vld == 1'b1)begin
			PL_HEADER_shift <= {PL_HEADER_shift[88:0],1'b0};           //thinking
		end
	end
	else begin
	end
end

reg								frame_vld_1dly;

always @(posedge sys_clk)begin
	if(~rst_n)begin
		frame_vld_1dly <= 1'h0;
		oe			   <= 1'h0;
	end
	else if(fs_en == 1'b1)begin
		frame_vld_1dly <= frame_vld;
		oe			   <= frame_vld_1dly;
	end
	else begin
	end
end

always @(posedge sys_clk)begin
	if(~rst_n)begin
		symbol_re_out <= 16'sh0000;
		symbol_im_out <= 16'sh0000;
	end
	else if(fs_en == 1'b1)begin
		if(frame_vld_1dly == 1'b1)begin
			if(symbol_cnt_reg == 1'b1)begin
				if(PL_HEADER_shift[89] == 1'b1)begin
					symbol_re_out <= 16'shf4b0;
					symbol_im_out <= 16'shf4b0;
				end
				else begin
					symbol_re_out <= 16'sh0b50;
					symbol_im_out <= 16'sh0b50;
				end
			end
			else begin
				if(PL_HEADER_shift[89] == 1'b0)begin
					symbol_re_out <= 16'shf4b0;
					symbol_im_out <= 16'sh0b50;
				end
				else begin
					symbol_re_out <= 16'sh0b50;
					symbol_im_out <= 16'shf4b0;
				end
			end
		end
	end
	else begin
	end
end


endmodule

