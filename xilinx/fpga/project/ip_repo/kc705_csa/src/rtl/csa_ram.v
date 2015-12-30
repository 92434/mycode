`timescale 1ns / 1ps

module csa_ram #(
		parameter integer MAX_CAL_TIMES = 3,

		parameter integer C_S_AXI_DATA_WIDTH = 32,
		parameter integer OPT_MEM_ADDR_BITS = 12
	)
	(
		input wire S_AXI_ACLK,
		input wire rst,

		input wire [(C_S_AXI_DATA_WIDTH/8)-1 : 0] S_AXI_WSTRB,
		input wire wen,
		input wire [C_S_AXI_DATA_WIDTH-1 : 0] S_AXI_WDATA,
		input wire [OPT_MEM_ADDR_BITS:0] waddr,

		input wire ren,
		output reg [C_S_AXI_DATA_WIDTH-1 : 0] rdata,
		input wire [12:0] raddr,

		input wire fpga_clk,
		output reg [48-1:0] byte_ram_out,
		output wire ready
	);

	localparam integer CAL_DATA_ITEM_NUM = 1;

	localparam integer STEP_REQ_STUFF=0;
	localparam integer STEP_STUFFING_DATA=1;
	localparam integer STEP_FIN_STUFF=2;
	
	integer cal_times_limit=1;
	// implement Block RAM(s)
	reg [C_S_AXI_DATA_WIDTH-1:0] byte_ram_in [0 : 9];//(CAL_DATA_ITEM_NUM*5)/4-1];
	integer i,j,stuff_cursor,calc_item_cursor,calc_item_total,loop_times;
	reg csa_begin_calc,ck_valid;
	reg output_ready;
	reg [8*8-1:0] ck,cb_bakup;
	reg [8*8-1:0] sb;
	wire [8*8-1:0] cb;
	integer current_end_pos = 0;
	
	assign ready=output_ready;
	
	always @(posedge S_AXI_ACLK) begin
		if(rst) begin
			current_end_pos <= 0;
		end
		else begin
			if (wen) begin
				case(waddr)
					STEP_REQ_STUFF:begin
						stuff_cursor<=0;
						csa_begin_calc<=0;
						calc_item_total<=0;
						current_end_pos <= 0;
					end
					STEP_STUFFING_DATA:begin
						byte_ram_in[stuff_cursor]<=S_AXI_WDATA;
						current_end_pos <= stuff_cursor;
						stuff_cursor<=stuff_cursor+1;
					end
					STEP_FIN_STUFF:begin
						csa_begin_calc<=1;
						cal_times_limit<=S_AXI_WDATA;
						calc_item_total<=(stuff_cursor*4)/5;
					end
				endcase
			end
		end
	end
	/**/
	`define CSA_DEBUG
	`ifdef CSA_DEBUG
		localparam integer DEBUG_BUF_SIZE = 300;
		reg [C_S_AXI_DATA_WIDTH-1:0] status_out[0:DEBUG_BUF_SIZE-1];
		reg [C_S_AXI_DATA_WIDTH-1:0] sta_cursor = 0;
	`endif
	
	always @(posedge S_AXI_ACLK) begin
		if(rst == 1) begin
		end
		else begin
			if (ren == 1) begin
				if((raddr >= 256)&&(raddr < 256 + DEBUG_BUF_SIZE)) begin//for debug
				`ifdef CSA_DEBUG
					rdata <= status_out[raddr-256];
				`endif
				end
				else if((raddr >= 0) && (raddr <= current_end_pos)) begin
					rdata <= byte_ram_in[raddr];
				end
				else begin
					rdata <= {16'hE000, {(16 - OPT_MEM_ADDR_BITS - 1){1'b0}}, raddr};
				end
			end
		end
	end
	/*
	wire [7:0] v0,v1,v2,v3,v4,v5,v7;
	assign v0=byte_ram_in[(5*calc_item_cursor)/4][(((5*calc_item_cursor)%4)*8 + 7) -: 8];
	assign v1=byte_ram_in[((5*calc_item_cursor)+1)/4][((((5*calc_item_cursor)+1)%4)*8 + 7) -: 8];
	assign v2=byte_ram_in[((5*calc_item_cursor)+2)/4][((((5*calc_item_cursor)+2)%4)*8 + 7) -: 8];
	assign v3=v0+v1+v2;
	assign v4=byte_ram_in[((5*calc_item_cursor)+3)/4][((((5*calc_item_cursor)+3)%4)*8 + 7) -: 8];
	assign v5=byte_ram_in[((5*calc_item_cursor)+4)/4][((((5*calc_item_cursor)+4)%4)*8 + 7) -: 8];
	assign v7=v4+v5;
	
	wire [7:0] k0,k1,k2,k3,k4,k5,k6,k7;
	assign k0=((cb[7:0]^8'hB5)+loop_times)%256;
	assign k1=((cb[15:8]^8'h93)+loop_times)%256;
	assign k2=((cb[23:16]^8'h5E)+loop_times)%256;
	assign k3=k0+k1+k2;
	assign k4=((cb[31:24]^8'hD6)+loop_times)%256;
	assign k5=((cb[39:32]^8'hB0)+loop_times)%256;
	assign k6=((cb[47:40]^8'hD7)+loop_times)%256;
	assign k7=k4+k5+k6;
	
	always @(posedge fpga_clk) begin
		if(rst) begin
			`ifdef CSA_DEBUG
				sta_cursor<=0;
			`endif
			ck<=0;
			loop_times<=0;
			calc_item_cursor<=0;
			output_ready<=0;
			ck_valid<=0;
			sb <=64'hE613DB6DC11C4524;
		end
		else if(csa_begin_calc) begin
			output_ready<=0;
			if(loop_times==0&&output_ready==0) begin	//prepare new ck value	
				sb <=64'hE613DB6DC11C4524;
				ck[7:0]  <=8'h1a;
				ck[15:8] <=8'h91;
				ck[23:16]<=8'h01;
				ck[31:24]<=8'hac;
				ck[39:32]<=8'h47;
				ck[47:40]<=8'h69;
				ck[55:48]<=0;
				ck[63:56]<=8'hb0;
				ck_valid<=1;
				`ifdef CSA_DEBUG
					sta_cursor<=0;
				`endif
			end			
			if(ck_valid) begin
				//$display("loop_times:%d,[V:%h,%h,%h,%h,%h,%h,%h]\n",loop_times,v0,v1,v2,v3,v4,v5,v7);
				$display("ck:%h,cb=%h, \n",ck,cb);
				`ifdef CSA_DEBUG
					if((sta_cursor + 2) <= DEBUG_BUF_SIZE) begin
						status_out[sta_cursor]<=sta_cursor;
						status_out[sta_cursor+1][7:0]  <=ck[7:0]  ;
						status_out[sta_cursor+1][15:8] <=ck[63:56];
						status_out[sta_cursor+1][23:16]<=cb[7:0];
						status_out[sta_cursor+1][31:24]<=cb[63:56];
						sta_cursor<=sta_cursor+2;
					end
					else begin
						sta_cursor<=0;
					end
				`endif
				if(loop_times+1<cal_times_limit) begin
					sb <=64'hE613DB6DC11C4524;
					ck <=cb;
					loop_times<=loop_times+1;
				end
				else begin
					byte_ram_out<=cb;
					loop_times<=0;
					calc_item_cursor<=calc_item_cursor+1;
					ck_valid<=0;
					output_ready<=1;
					if(calc_item_cursor+1>=calc_item_total) begin//calc finished,need to notify app
						calc_item_cursor<=0;
					end
				end
			end
		end
	end
*/	
reg do_csa;
`ifdef CSA_DEBUG
	always @(posedge fpga_clk) begin
		if(rst) begin
			loop_times<=0;
			calc_item_cursor<=0;
			output_ready<=0;
			ck_valid<=0;
			sb <=64'hE613DB6DC11C4524;
			ck<=0;
			do_csa<=1;
			cb_bakup<=0;
			`ifdef CSA_DEBUG
				sta_cursor<=0;
			`endif
		end
		else if(csa_begin_calc) begin
			if(do_csa)begin
				if((sta_cursor + 2) >= DEBUG_BUF_SIZE) begin
					sta_cursor<=0;
				end
				else begin
					sta_cursor<=sta_cursor+2;
				end
				status_out[sta_cursor]<=loop_times;
				status_out[sta_cursor+1][7:0]  <=ck[7:0]  ;
				status_out[sta_cursor+1][15:8] <=ck[63:56];
				status_out[sta_cursor+1][23:16]<=cb[7:0]  ;
				status_out[sta_cursor+1][31:24]<=cb[63:56];
				$display("ck:%h,cb=%h,cb_bakup=%h\n",ck,cb,cb_bakup);
				
				
				if(loop_times+1<cal_times_limit) begin
					loop_times<=loop_times+1;
					ck <=cb_bakup;
				end
				else begin
					loop_times<=0;
					output_ready<=1;
					byte_ram_out[7:0]  <=cb[7:0];
				  byte_ram_out[15:8] <=cb[15:8];
				  byte_ram_out[23:16]<=cb[23:16];
				  byte_ram_out[31:24]<=cb[31:24];
				  byte_ram_out[39:32]<=cb[39:32];
				  byte_ram_out[47:40]<=cb[47:40];
					ck[7:0]  <=byte_ram_in[0][7:0]  ;
					ck[15:8] <=byte_ram_in[0][15:8] ;
					ck[23:16]<=byte_ram_in[0][23:16];
					ck[31:24]<=byte_ram_in[0][31:24];
					ck[39:32]<=byte_ram_in[1][7:0]  ;
					ck[47:40]<=byte_ram_in[1][15:8] ;
					ck[55:48]<=byte_ram_in[1][23:16];
					ck[63:56]<=byte_ram_in[1][31:24];
				end
			end
			else begin
				cb_bakup<=cb;
				output_ready<=0;
				
			end
			do_csa=~do_csa;
		end
	end
`endif

	
	stream_cypher b(
	.sb   (sb)
	,.ck    (ck)
	,.cb    (cb)
	);
endmodule
