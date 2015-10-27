module logic_ram #(
		parameter integer C_S_AXI_DATA_WIDTH = 32,
		parameter integer OPT_MEM_ADDR_BITS = 3,
		parameter FILTER_MAX_NUM = 32
	)
	(
		input wire [7:0] mpeg_data,
		input wire mpeg_clk,
		input wire mpeg_valid,
		input wire mpeg_sync,
		input wire rst,
		input wire S_AXI_ACLK,
		input wire [(C_S_AXI_DATA_WIDTH/8)-1 : 0] S_AXI_WSTRB,
		input wire [C_S_AXI_DATA_WIDTH-1 : 0] S_AXI_WDATA,
		input wire mem_rden,
		input wire mem_wren,
		input wire [OPT_MEM_ADDR_BITS:0] mem_address,
		output reg [C_S_AXI_DATA_WIDTH-1 : 0] axi_rdata,
		output wire [7:0] ts_out,
		output wire ts_out_clk,
		output wire ts_out_valid,
		output wire ts_out_sync,
		output wire slot0_out_dump_flag
	);
		
	parameter PACK_BYTE_SIZE = 188;
    //mem addr define
    parameter INPUT_PARAM_OFFSET=0;
    parameter REPLACE_TS_OFFSET=INPUT_PARAM_OFFSET+4;
    parameter MONITOR_READY_OFFSET=REPLACE_TS_OFFSET+PACK_BYTE_SIZE;
    parameter MONITOR_TS_OFFSET=MONITOR_READY_OFFSET+FILTER_MAX_NUM*4*2;
	
	// implement Block RAM(s)
	reg [C_S_AXI_DATA_WIDTH-1:0] byte_ram [0 : 1024*2 - 1];
	reg replace_add,replace_del,monitor_add,monitor_del;
	reg [12:0] pid_in;
	reg [7:0] slot_idx,input_cursor;
	reg [7:0] monitor_cursor[FILTER_MAX_NUM-1:0] ,monitor_slot;
	
	wire[FILTER_MAX_NUM-1:0] dump_flag;
	wire[7:0] dump_data;
	//wire ts_out_sync;
	integer i,j;
	reg[FILTER_MAX_NUM-1:0] start_recv_monitor;
	
	reg [FILTER_MAX_NUM-1:0] begin_read;
	wire [7:0] copy_data[FILTER_MAX_NUM-1:0];
	wire df_ready[FILTER_MAX_NUM-1:0];
	
	assign slot0_out_dump_flag=dump_flag[0];
	//set filter param and replace data
	always @(posedge S_AXI_ACLK or posedge mpeg_clk) begin
		if (S_AXI_ACLK&mem_wren) begin
			for(i = 0; i < (C_S_AXI_DATA_WIDTH / 8); i = i + 1) begin
				if(S_AXI_WSTRB[i] == 1) begin
					byte_ram[mem_address][(8 * i + 7) -: 8] <= S_AXI_WDATA[(8 * i + 7) -: 8];
				end
			end
			
			case(mem_address*4)
				INPUT_PARAM_OFFSET:begin
					replace_add=S_AXI_WDATA[0];
					replace_del=S_AXI_WDATA[1];
					monitor_add=S_AXI_WDATA[2];
					monitor_del=S_AXI_WDATA[3];
					slot_idx=S_AXI_WDATA[27:20];
					if(monitor_add) begin
						pid_in=S_AXI_WDATA[16:4];
					end
					input_cursor=0;
				end
				REPLACE_TS_OFFSET:begin
					pid_in[7:0]=S_AXI_WDATA[7:0];//only use low 8bit each clk
					input_cursor=input_cursor+1;
					if(input_cursor>=PACK_BYTE_SIZE) begin
						input_cursor=0;
					end
				end
			endcase
		end
		
		if(mpeg_clk) begin
			//read dump data here
			if(dump_flag>0&&ts_out_valid==1&&ts_out_sync==1) begin
				for(i=0;i<=FILTER_MAX_NUM-1;i=i+1) begin
					if(dump_flag[i]) monitor_slot=i;
				end
				monitor_cursor[monitor_slot]=0;
				if(byte_ram[MONITOR_READY_OFFSET+monitor_slot]==0)
					start_recv_monitor[monitor_slot]=1;
			end
			  
			if(start_recv_monitor[monitor_slot]) begin
				j=MONITOR_TS_OFFSET+monitor_slot*PACK_BYTE_SIZE+monitor_cursor[monitor_slot];
				byte_ram[j/4][((j%4)*8 + 7) -: 8]= dump_data;
				$display("[logic_ram dump]monitor_slot:%d :%h", monitor_slot,byte_ram[j/4][((j%4)*8 + 7) -: 8]);
				monitor_cursor[monitor_slot]=monitor_cursor[monitor_slot]+1;
				if(monitor_cursor[monitor_slot]>=PACK_BYTE_SIZE) begin
					monitor_cursor[monitor_slot]=0;
					byte_ram[MONITOR_READY_OFFSET+monitor_slot]=1;
					start_recv_monitor[monitor_slot]=0;
				end
			end
		end
	end
	
	//always	@(posedge mpeg_clk) begin
		
		
	//end

	
	match_replace_module#(.FILTER_MAX_NUM( FILTER_MAX_NUM)) A1(
    .mpeg_data(mpeg_data),
    .mpeg_clk(mpeg_clk),
    .mpeg_valid(mpeg_valid),
    .mpeg_sync(mpeg_sync),
    .rst(rst),
    .axi_clk(S_AXI_ACLK),
    .axi_wren(mem_wren),
    .replace_add(replace_add),
    .replace_del(replace_del),
    .monitor_add(monitor_add),
    .monitor_del(monitor_del),
    .pid_in(pid_in),
    .slot_idx(slot_idx),
    .ts_clk(ts_out_clk),
    .ts_valid(ts_out_valid),
    .ts_sync(ts_out_sync),
    .ts_out(ts_out),
    .dump_flag(dump_flag),
    .dump_data(dump_data)
    );
    
	always @(posedge S_AXI_ACLK) begin
		if (mem_rden) begin
			axi_rdata <= byte_ram[mem_address];
		end
	end


	
endmodule
