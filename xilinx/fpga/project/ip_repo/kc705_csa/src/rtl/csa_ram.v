`timescale 1ns / 1ps

module csa_ram #(
		parameter integer MAX_CAL_TIMES = 10,

		parameter integer C_S_AXI_DATA_WIDTH = 32,
		parameter integer OPT_MEM_ADDR_BITS = 3
	)
	(
		input wire S_AXI_ACLK,
		input wire rst,

		input wire [(C_S_AXI_DATA_WIDTH/8)-1 : 0] S_AXI_WSTRB,
		input wire wen,
		input wire [C_S_AXI_DATA_WIDTH-1 : 0] S_AXI_WDATA,
		input wire [OPT_MEM_ADDR_BITS:0] waddr,

		input wire fpga_clk,
		output reg [48-1:0] byte_ram_out,
		output wire ready
	);

	localparam integer CAL_DATA_ITEM_NUM = 16;

	localparam integer STEP_REQ_STUFF=0;
	localparam integer STEP_STUFFING_DATA=1;
	localparam integer STEP_FIN_STUFF=2;

	// implement Block RAM(s)
	reg [C_S_AXI_DATA_WIDTH-1:0] byte_ram_in [0 : (CAL_DATA_ITEM_NUM*5)/4-1];
	//reg [C_S_AXI_DATA_WIDTH-1:0] byte_ram_out[0 : (CAL_DATA_ITEM_NUM*6)/4-1];
	integer i,j,stuff_cursor,calc_item_cursor,calc_item_total,loop_times;
	reg csa_init,csa_begin_calc,ck_valid;
	reg [14:0]read_cursor;
	reg output_ready;
	reg [8*8-1:0] ck;
	reg [8*8-1:0] sb;
	wire [8*8-1:0] cb;
	
	assign ready=output_ready;
	
	always @(posedge S_AXI_ACLK) begin
		if(rst) begin
			read_cursor<=0;
			ck<=0;
		end
		else begin
			if (wen) begin
				case(waddr)
					STEP_REQ_STUFF:begin
						stuff_cursor<=0;
						csa_begin_calc<=0;
						calc_item_total<=0;
					end
					STEP_STUFFING_DATA:begin
						byte_ram_in[stuff_cursor]<=S_AXI_WDATA;
						stuff_cursor<=stuff_cursor+1;
					end
					STEP_FIN_STUFF:begin
						csa_begin_calc<=1;
						calc_item_total<=(stuff_cursor*4)/5;
					end
				endcase
			end
			
			/*if (mem_rden) begin
				axi_rdata 	<= byte_ram_out[read_cursor];
				read_cursor	<= read_cursor+1;
				if(read_cursor>=(calc_item_total*6)/4) begin
					read_cursor<=0;
					read_fin<=1;
				end
			end
			*/
		end
	end
	
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
			csa_init<=1;
			loop_times<=0;
			calc_item_cursor<=0;
			output_ready<=0;
			ck_valid<=0;
			sb =64'hE613DB6DC11C4524;
		end
		else if(csa_begin_calc) begin
			csa_init<=0;
			output_ready<=0;
			if(loop_times==0&&output_ready==0) begin	//prepare new ck value	
				ck[7:0]<=v0;
				ck[15:8]<=v1;
				ck[23:16]<=v2;
				ck[31:24]<=v3;
				ck[39:32]<=v4;
				ck[47:40]<=v5;
				ck[55:48]<=0;
				ck[63:56]<=v7;
				ck_valid<=1;
			end			
			if(ck_valid) begin
				$display("loop_times:%d,[V:%h,%h,%h,%h,%h,%h,%h]\n",loop_times,v0,v1,v2,v3,v4,v5,v7);
				$display("ck:%h,cb=%h, \n",ck,cb);
				if(loop_times+1<MAX_CAL_TIMES) begin
					ck[7:0]<=k0;
					ck[15:8]<=k1;
					ck[23:16]<=k2;
					ck[31:24]<=k3;
					ck[39:32]<=k4;
					ck[47:40]<=k5;
					ck[55:48]<=k6;
					ck[63:56]<=k7;
					loop_times<=loop_times+1;
				end
				else begin
					byte_ram_out[7:0]<=cb[7:0]^8'hB5;
					byte_ram_out[15:8]<=cb[15:8]^8'h93;
					byte_ram_out[23:16]<=cb[23:16]^8'h5E;
					byte_ram_out[31:24]<=cb[31:24]^8'hD6;
					byte_ram_out[39:32]<=cb[39:32]^8'hB0;
					byte_ram_out[47:40]<=cb[47:40]^8'hD7;
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
	
	stream_cypher b(
	.sb   (sb)
	,.ck    (ck)
	,.cb    (cb)
	);

endmodule
