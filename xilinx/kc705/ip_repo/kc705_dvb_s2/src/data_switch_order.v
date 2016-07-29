
`timescale 1ns / 1ps
module data_switch_order(
input	          							sys_clk,
input											fs_en,
input	          							rst_n,
//////////////////////////////////////////////////////////////
input	          							switch_vld,
input	          			[359:0] 		ram_data,
input	          			[5:0] 		switch_array,//0~44
input	          			[2:0] 		order_array,//0~7
//////////////////////////////////////////////////////////////
output  		reg  							order_oe,
output      reg   		[7:0] 		byte_out
);

wire							[44:0]		ram0_data;
wire							[44:0]		ram1_data;
wire							[44:0]		ram2_data;
wire							[44:0]		ram3_data;
wire							[44:0]		ram4_data;
wire							[44:0]		ram5_data;
wire							[44:0]		ram6_data;
wire							[44:0]		ram7_data;


assign 		ram0_data = ram_data[359:315];
assign 		ram1_data = ram_data[314:270];
assign 		ram2_data = ram_data[269:225];
assign 		ram3_data = ram_data[224:180];
assign 		ram4_data = ram_data[179:135];
assign 		ram5_data = ram_data[134:90];
assign 		ram6_data = ram_data[89:45];
assign 		ram7_data = ram_data[44:0];

reg													switch_vld_1dly;
reg	          		[2:0] 		order_array_1dly;

always @(posedge sys_clk)begin
	if(~rst_n)begin
		switch_vld_1dly <= 1'b0;
		order_oe        <= 1'b0;
		order_array_1dly<= 3'b000;
	end
	else if(fs_en == 1'b1)begin
		switch_vld_1dly <= switch_vld;
		order_oe        <= switch_vld_1dly;
		order_array_1dly<= order_array;
	end
	else begin
	end
end


reg							[7:0] 		switch_data;

always @(posedge sys_clk)begin
	if(~rst_n)begin
		switch_data <= 8'h00;
	end
	else if(fs_en == 1'b1)begin
		case(switch_array)
		6'h00: switch_data <= {ram0_data[44],ram1_data[44],ram2_data[44],ram3_data[44],ram4_data[44],ram5_data[44],ram6_data[44],ram7_data[44]};
		6'h01: switch_data <= {ram0_data[43],ram1_data[43],ram2_data[43],ram3_data[43],ram4_data[43],ram5_data[43],ram6_data[43],ram7_data[43]};
		6'h02: switch_data <= {ram0_data[42],ram1_data[42],ram2_data[42],ram3_data[42],ram4_data[42],ram5_data[42],ram6_data[42],ram7_data[42]};
		6'h03: switch_data <= {ram0_data[41],ram1_data[41],ram2_data[41],ram3_data[41],ram4_data[41],ram5_data[41],ram6_data[41],ram7_data[41]};
		6'h04: switch_data <= {ram0_data[40],ram1_data[40],ram2_data[40],ram3_data[40],ram4_data[40],ram5_data[40],ram6_data[40],ram7_data[40]};
		6'h05: switch_data <= {ram0_data[39],ram1_data[39],ram2_data[39],ram3_data[39],ram4_data[39],ram5_data[39],ram6_data[39],ram7_data[39]};
		6'h06: switch_data <= {ram0_data[38],ram1_data[38],ram2_data[38],ram3_data[38],ram4_data[38],ram5_data[38],ram6_data[38],ram7_data[38]};
		6'h07: switch_data <= {ram0_data[37],ram1_data[37],ram2_data[37],ram3_data[37],ram4_data[37],ram5_data[37],ram6_data[37],ram7_data[37]};
		6'h08: switch_data <= {ram0_data[36],ram1_data[36],ram2_data[36],ram3_data[36],ram4_data[36],ram5_data[36],ram6_data[36],ram7_data[36]};
		6'h09: switch_data <= {ram0_data[35],ram1_data[35],ram2_data[35],ram3_data[35],ram4_data[35],ram5_data[35],ram6_data[35],ram7_data[35]};
		6'h0a: switch_data <= {ram0_data[34],ram1_data[34],ram2_data[34],ram3_data[34],ram4_data[34],ram5_data[34],ram6_data[34],ram7_data[34]};
		6'h0b: switch_data <= {ram0_data[33],ram1_data[33],ram2_data[33],ram3_data[33],ram4_data[33],ram5_data[33],ram6_data[33],ram7_data[33]};
		6'h0c: switch_data <= {ram0_data[32],ram1_data[32],ram2_data[32],ram3_data[32],ram4_data[32],ram5_data[32],ram6_data[32],ram7_data[32]};
		6'h0d: switch_data <= {ram0_data[31],ram1_data[31],ram2_data[31],ram3_data[31],ram4_data[31],ram5_data[31],ram6_data[31],ram7_data[31]};
		6'h0e: switch_data <= {ram0_data[30],ram1_data[30],ram2_data[30],ram3_data[30],ram4_data[30],ram5_data[30],ram6_data[30],ram7_data[30]};
		6'h0f: switch_data <= {ram0_data[29],ram1_data[29],ram2_data[29],ram3_data[29],ram4_data[29],ram5_data[29],ram6_data[29],ram7_data[29]};
		6'h10: switch_data <= {ram0_data[28],ram1_data[28],ram2_data[28],ram3_data[28],ram4_data[28],ram5_data[28],ram6_data[28],ram7_data[28]};
		6'h11: switch_data <= {ram0_data[27],ram1_data[27],ram2_data[27],ram3_data[27],ram4_data[27],ram5_data[27],ram6_data[27],ram7_data[27]};
		6'h12: switch_data <= {ram0_data[26],ram1_data[26],ram2_data[26],ram3_data[26],ram4_data[26],ram5_data[26],ram6_data[26],ram7_data[26]};
		6'h13: switch_data <= {ram0_data[25],ram1_data[25],ram2_data[25],ram3_data[25],ram4_data[25],ram5_data[25],ram6_data[25],ram7_data[25]};
		6'h14: switch_data <= {ram0_data[24],ram1_data[24],ram2_data[24],ram3_data[24],ram4_data[24],ram5_data[24],ram6_data[24],ram7_data[24]};
		6'h15: switch_data <= {ram0_data[23],ram1_data[23],ram2_data[23],ram3_data[23],ram4_data[23],ram5_data[23],ram6_data[23],ram7_data[23]};
		6'h16: switch_data <= {ram0_data[22],ram1_data[22],ram2_data[22],ram3_data[22],ram4_data[22],ram5_data[22],ram6_data[22],ram7_data[22]};
		6'h17: switch_data <= {ram0_data[21],ram1_data[21],ram2_data[21],ram3_data[21],ram4_data[21],ram5_data[21],ram6_data[21],ram7_data[21]};
		6'h18: switch_data <= {ram0_data[20],ram1_data[20],ram2_data[20],ram3_data[20],ram4_data[20],ram5_data[20],ram6_data[20],ram7_data[20]};
		6'h19: switch_data <= {ram0_data[19],ram1_data[19],ram2_data[19],ram3_data[19],ram4_data[19],ram5_data[19],ram6_data[19],ram7_data[19]};
		6'h1a: switch_data <= {ram0_data[18],ram1_data[18],ram2_data[18],ram3_data[18],ram4_data[18],ram5_data[18],ram6_data[18],ram7_data[18]};
		6'h1b: switch_data <= {ram0_data[17],ram1_data[17],ram2_data[17],ram3_data[17],ram4_data[17],ram5_data[17],ram6_data[17],ram7_data[17]};
		6'h1c: switch_data <= {ram0_data[16],ram1_data[16],ram2_data[16],ram3_data[16],ram4_data[16],ram5_data[16],ram6_data[16],ram7_data[16]};
		6'h1d: switch_data <= {ram0_data[15],ram1_data[15],ram2_data[15],ram3_data[15],ram4_data[15],ram5_data[15],ram6_data[15],ram7_data[15]};
		6'h1e: switch_data <= {ram0_data[14],ram1_data[14],ram2_data[14],ram3_data[14],ram4_data[14],ram5_data[14],ram6_data[14],ram7_data[14]};
		6'h1f: switch_data <= {ram0_data[13],ram1_data[13],ram2_data[13],ram3_data[13],ram4_data[13],ram5_data[13],ram6_data[13],ram7_data[13]};
		6'h20: switch_data <= {ram0_data[12],ram1_data[12],ram2_data[12],ram3_data[12],ram4_data[12],ram5_data[12],ram6_data[12],ram7_data[12]};
		6'h21: switch_data <= {ram0_data[11],ram1_data[11],ram2_data[11],ram3_data[11],ram4_data[11],ram5_data[11],ram6_data[11],ram7_data[11]};
		6'h22: switch_data <= {ram0_data[10],ram1_data[10],ram2_data[10],ram3_data[10],ram4_data[10],ram5_data[10],ram6_data[10],ram7_data[10]};
		6'h23: switch_data <= {ram0_data[9],ram1_data[9],ram2_data[9],ram3_data[9],ram4_data[9],ram5_data[9],ram6_data[9],ram7_data[9]};
		6'h24: switch_data <= {ram0_data[8],ram1_data[8],ram2_data[8],ram3_data[8],ram4_data[8],ram5_data[8],ram6_data[8],ram7_data[8]};
		6'h25: switch_data <= {ram0_data[7],ram1_data[7],ram2_data[7],ram3_data[7],ram4_data[7],ram5_data[7],ram6_data[7],ram7_data[7]};
		6'h26: switch_data <= {ram0_data[6],ram1_data[6],ram2_data[6],ram3_data[6],ram4_data[6],ram5_data[6],ram6_data[6],ram7_data[6]};
		6'h27: switch_data <= {ram0_data[5],ram1_data[5],ram2_data[5],ram3_data[5],ram4_data[5],ram5_data[5],ram6_data[5],ram7_data[5]};
		6'h28: switch_data <= {ram0_data[4],ram1_data[4],ram2_data[4],ram3_data[4],ram4_data[4],ram5_data[4],ram6_data[4],ram7_data[4]};
		6'h29: switch_data <= {ram0_data[3],ram1_data[3],ram2_data[3],ram3_data[3],ram4_data[3],ram5_data[3],ram6_data[3],ram7_data[3]};
		6'h2a: switch_data <= {ram0_data[2],ram1_data[2],ram2_data[2],ram3_data[2],ram4_data[2],ram5_data[2],ram6_data[2],ram7_data[2]};
		6'h2b: switch_data <= {ram0_data[1],ram1_data[1],ram2_data[1],ram3_data[1],ram4_data[1],ram5_data[1],ram6_data[1],ram7_data[1]};
		6'h2c: switch_data <= {ram0_data[0],ram1_data[0],ram2_data[0],ram3_data[0],ram4_data[0],ram5_data[0],ram6_data[0],ram7_data[0]};	
		default:switch_data <= 8'h00;
		endcase
	end
	else begin
	end
end


always @(posedge sys_clk)begin
	if(~rst_n)begin
		byte_out <= 8'h00;
	end
	else if(fs_en == 1'b1)begin
		case(order_array_1dly)
		3'h0: byte_out <= switch_data;
		3'h1: byte_out <= {switch_data[6:0],switch_data[7]};
		3'h2: byte_out <= {switch_data[5:0],switch_data[7:6]};
		3'h3: byte_out <= {switch_data[4:0],switch_data[7:5]};
		3'h4: byte_out <= {switch_data[3:0],switch_data[7:4]};
		3'h5: byte_out <= {switch_data[2:0],switch_data[7:3]};
		3'h6: byte_out <= {switch_data[1:0],switch_data[7:2]};
		3'h7: byte_out <= {switch_data[0],switch_data[7:1]};
		default:byte_out <= 8'h00;
		endcase
	end
	else begin
	end
end


endmodule
