`timescale 1ns / 1ns
//`include "logic_ram_base.v"
`include "../rtl/csa_ram.v"
module logic_ram_tp #(
		parameter integer C_S_AXI_DATA_WIDTH = 32,
		parameter integer OPT_MEM_ADDR_BITS = 3,
		parameter integer CAL_DATA_ITEM_NUM = 1,
		
		parameter integer STEP_REQ_STUFF=0,
		parameter integer STEP_STUFFING_DATA=1,
		parameter integer STEP_FIN_STUFF=2
	);
  //parameter PACK_BYTE_SIZE = 8'hBC;
  reg axi_clk,fpga_clk; 
  reg [(C_S_AXI_DATA_WIDTH/8)-1 : 0] S_AXI_WSTRB;
  reg [C_S_AXI_DATA_WIDTH-1 : 0] S_AXI_WDATA;
  reg mem_rden,mem_wren;
  reg [OPT_MEM_ADDR_BITS:0] waddr;
  reg [12:0]raddr;
  wire ready;
  reg rst;
  reg [C_S_AXI_DATA_WIDTH-1 : 0] in_data[(CAL_DATA_ITEM_NUM*5)/4-1 : 0];
  reg [7 : 0]ram_out[CAL_DATA_ITEM_NUM*6-1 : 0] ;
  wire [48-1:0] byte_ram_out;
  wire [C_S_AXI_DATA_WIDTH-1 : 0] rdata;
  integer init_step,stuff_cursor,read_cursor;
  always #5 axi_clk=~axi_clk;
  always #10 fpga_clk=~fpga_clk;
  csa_ram LR3(
		.S_AXI_ACLK(axi_clk),
		.S_AXI_WSTRB(S_AXI_WSTRB),
		.S_AXI_WDATA(S_AXI_WDATA),
		.wen(mem_wren),
		.waddr(waddr),
		
		.ren(mem_rden),
		.rdata(rdata),
		.raddr(raddr),
		
		.rst(rst),
		.fpga_clk(axi_clk),
		.byte_ram_out(byte_ram_out),
		.ready(ready)
	);
  initial begin
	$readmemh("../test_dat/in_data.copy",in_data);
    axi_clk=0;
    fpga_clk=0;
	init_step=0;
	read_cursor=0;
	raddr=256;
    rst=1;
    #20 rst=0;
	end
	
	
  
  localparam integer DEBUG_BUF_SIZE = 6;
/*
	always @(negedge axi_clk) begin
		if(rst==0) begin
			if(init_step==0) begin
				S_AXI_WDATA=0;
				waddr=STEP_REQ_STUFF;
				mem_wren=1;
				stuff_cursor=0;
				init_step=init_step+1;
				$display("STEP_REQ_STUFF\n");
			end
			else if(init_step==1) begin
				waddr=STEP_STUFFING_DATA;
				mem_wren=1;
				S_AXI_WDATA=in_data[stuff_cursor];
				$display("stuff_cursor:%d,S_AXI_WDATA:%h\n",stuff_cursor,S_AXI_WDATA);
				stuff_cursor=stuff_cursor+1;
				if(stuff_cursor==(CAL_DATA_ITEM_NUM*5)/4) begin
					stuff_cursor=0;
					init_step=init_step+1;
				end	
			end
			else if(init_step==2) begin  
				S_AXI_WDATA=0;
				waddr=STEP_FIN_STUFF;
				mem_wren=1;
				init_step=init_step+1;
				$display("STEP_FIN_STUFF\n");
			end
			else if(init_step==3) begin
			 mem_wren=0;
			 if(ready==1) begin
				mem_rden=1;
				init_step=init_step+1;
				raddr=256;
			 end
		  end
		end
	end	
	*/
	always @(negedge axi_clk) begin
	 if(rst==0) begin
		if(init_step==0) begin
			if(ready==1) begin
				ram_out[read_cursor]=byte_ram_out[7:0];
				ram_out[read_cursor+1]=byte_ram_out[15:8];
				ram_out[read_cursor+2]=byte_ram_out[23:16];
				ram_out[read_cursor+3]=byte_ram_out[31:24];
				ram_out[read_cursor+4]=byte_ram_out[39:32];
				ram_out[read_cursor+5]=byte_ram_out[47:40];
				$display("ram_out[%d]:%h,%h,%h,%h,%h,%h\n",read_cursor,ram_out[read_cursor],ram_out[read_cursor+1],ram_out[read_cursor+2],ram_out[read_cursor+3],ram_out[read_cursor+4],ram_out[read_cursor+5]);
				read_cursor=read_cursor+6;
				if(read_cursor>=CAL_DATA_ITEM_NUM*6) begin
					read_cursor=0;
					init_step=init_step+1;
					mem_rden<=1;
				end
				
			end
		end
		else if(init_step==1) begin
			mem_rden=1;
			$display("rdata[%d]:%h",raddr,rdata);
			raddr=raddr+1;
			if(raddr>=256+DEBUG_BUF_SIZE) begin
			  raddr=256;
			  //init_step=init_step+1;
			end
		end
		else if(init_step==2) begin
			mem_rden=0;
			#20 $finish;
		end
	end
end
	
	
endmodule