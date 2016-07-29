`timescale 1ns / 1ps
module fs_transform(
input	          							hard_rst_n,// modified by 2014.09.22
input                  					    sys_clk,
input                  					    fs_0p5_en,
output  reg                                fs_en_1cycle,
output  reg                                fs_en_1cycle_1dly
);

reg                                         fs_0p5_en_1dly;
reg                                         fs_0p5_en_2dly;

always @(posedge sys_clk or negedge hard_rst_n)begin
	if(~hard_rst_n)begin
		fs_0p5_en_1dly <=  0;
		fs_0p5_en_2dly <=  0;
	end
	else begin
	   fs_0p5_en_1dly <=  fs_0p5_en;
       fs_0p5_en_2dly <=  fs_0p5_en_1dly;
	end
end

// assign fs_en_1cycle = (fs_0p5_en_1dly ==  1)&&(fs_0p5_en_2dly == 0);

always @(posedge sys_clk or negedge hard_rst_n)begin
	if(~hard_rst_n)begin
		fs_en_1cycle <=  0;
	end
	else if((fs_0p5_en_1dly ==  1)&&(fs_0p5_en_2dly == 0))begin
	   fs_en_1cycle <=  1;
	end
	else begin
	   fs_en_1cycle <=  0;
	end
end

always @(posedge sys_clk or negedge hard_rst_n)begin
	if(~hard_rst_n)begin
		fs_en_1cycle_1dly <=  0;
	end
	else begin
	   fs_en_1cycle_1dly <=  fs_en_1cycle;
	end
end

endmodule
