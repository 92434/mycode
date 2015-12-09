`timescale 1ns / 1ns
`include "logic_ram.v"

module logic_ram_tp #(
		parameter integer C_S_AXI_DATA_WIDTH = 32,
		parameter integer OPT_MEM_ADDR_BITS = 3,
		parameter PACK_BYTE_SIZE = 8'hBC,
		parameter FILTER_MAX_NUM = 5'h1F,
		//mem addr define
		parameter INPUT_PARAM_OFFSET=0,
		parameter REPLACE_TS_OFFSET=INPUT_PARAM_OFFSET+4,
		parameter MONITOR_READY_OFFSET=REPLACE_TS_OFFSET+PACK_BYTE_SIZE,
		parameter MONITOR_TS_OFFSET=MONITOR_READY_OFFSET+128
	);
  //parameter PACK_BYTE_SIZE = 8'hBC;
  reg	mpeg_clk,axi_clk; 
  reg[7:0] mpeg_in[1316:0];
  reg[7:0] ts_in;
  reg valid,sync,rst,replace_add,replace_del,monitor_add,monitor_del;
  reg[7:0] filter[188*2+1:0],req_data_buf[187:0];
  reg[10:0] cursor,add_cursor,init_step;
  
  reg [(C_S_AXI_DATA_WIDTH/8)-1 : 0] S_AXI_WSTRB;
  reg [C_S_AXI_DATA_WIDTH-1 : 0] S_AXI_WDATA;
  reg mem_rden,mem_wren;
  reg [OPT_MEM_ADDR_BITS:0] mem_address;
  wire [C_S_AXI_DATA_WIDTH-1 : 0] axi_rdata;
  
  always #5 axi_clk=~axi_clk;
  always #10 mpeg_clk=~mpeg_clk;
  logic_ram LR1(
		.mpeg_data(ts_in),
		.mpeg_clk(mpeg_clk),
		.mpeg_valid(valid),
		.mpeg_sync(sync),
		.rst(rst),
		.S_AXI_ACLK(axi_clk),
		.S_AXI_WSTRB(S_AXI_WSTRB),
		.S_AXI_WDATA(S_AXI_WDATA),
		.mem_rden(mem_rden),
		.mem_wren(mem_wren),
		.mem_address(mem_address),
		.axi_rdata(axi_rdata)
	);
	
  initial begin
    $readmemh("filter1.v",filter);
    $readmemh("filter2.v",filter,188);
    $readmemh("ts4.txt",mpeg_in,0);
    cursor=0;
    add_cursor=0;
    sync=0;
    axi_clk=0;
    mpeg_clk=0;
    rst=1;
    valid=1;
    init_step=0;
    #20 rst=0;
	end
	
	always @(negedge mpeg_clk) begin
    if(rst==0) begin
      if(mpeg_in[cursor]==8'h47)
				sync<=1;
			else if(sync)
				sync<=0;
			ts_in<=mpeg_in[cursor];
			cursor<=cursor+1;
			//$display("[MatchModule]cursor:%d ts_out:%x", cursor,mpeg_in[cursor]);
			if(cursor>=PACK_BYTE_SIZE*7+10)
				$finish;
		end  
  end
  
  

	always @(negedge axi_clk) begin
	  if(rst==0) begin
      if(init_step==0) begin
        S_AXI_WDATA=0;
        S_AXI_WDATA[2]=1;//monitor_add
        S_AXI_WDATA[16:4]=13'h157f;
        S_AXI_WDATA[27:20]=1;
        mem_address=INPUT_PARAM_OFFSET;
        mem_wren=1;
        init_step=init_step+1;
        $display("[write param]monitor_add,slot_idx:%d,pid:%x",S_AXI_WDATA[27:20],S_AXI_WDATA[16:4]);
      end
      else if(init_step==1) begin
        S_AXI_WDATA=0;
        S_AXI_WDATA[2]=1;//monitor_add
        S_AXI_WDATA[16:4]=13'h0191;
        S_AXI_WDATA[27:20]=2;
        mem_address=INPUT_PARAM_OFFSET;
        mem_wren=1;
        init_step=init_step+1;
        $display("[write param]monitor_add,slot_idx:%d,pid:%x",S_AXI_WDATA[27:20],S_AXI_WDATA[16:4]);
      end
      else if(init_step==2) begin  
        S_AXI_WDATA=0;
        S_AXI_WDATA[0]=1;//replace_add
        S_AXI_WDATA[27:20]=1;//slot idx
        mem_address=INPUT_PARAM_OFFSET;
        mem_wren=1;
        init_step=init_step+1;
        $display("[write param]replace_add,slot_idx:%d",S_AXI_WDATA[27:20]);
      end
      else if(init_step==3) begin
        mem_address=REPLACE_TS_OFFSET;
        mem_wren=1;
        S_AXI_WDATA[7:0]=filter[add_cursor];
        add_cursor=add_cursor+1;
				if(add_cursor==PACK_BYTE_SIZE) begin
					//mem_wren=0;
					init_step=init_step+1;
				end
				//$display("[write data]filter[%d]:%x",add_cursor,filter[add_cursor]);
      end
      else if(init_step==4) begin
        S_AXI_WDATA=0;
        S_AXI_WDATA[0]=1;//replace_add
        S_AXI_WDATA[27:20]=4;
        mem_address=INPUT_PARAM_OFFSET;
        mem_wren=1;
        init_step=init_step+1;
        $display("[write param]replace_add,slot_idx:%d",S_AXI_WDATA[27:20]);
      end
      else if(init_step==5) begin
        mem_address=REPLACE_TS_OFFSET;
        mem_wren=1;
        S_AXI_WDATA[7:0]=filter[add_cursor];
        add_cursor=add_cursor+1;
				if(add_cursor==PACK_BYTE_SIZE*2) begin
				  add_cursor=0;
					//mem_wren=0;
					init_step=init_step+1;
				end
				//$display("[write data]filter[%d]:%x",add_cursor,filter[add_cursor]);
      end
      else if(init_step==6) begin
        S_AXI_WDATA=0;
        mem_address=INPUT_PARAM_OFFSET;
        mem_wren=1;
        init_step=init_step+1;
        #1880;
      end
      else if(init_step==7) begin
        S_AXI_WDATA=0;
        S_AXI_WDATA[1]=1;//replace_del
        S_AXI_WDATA[27:20]=1;
        mem_address=INPUT_PARAM_OFFSET;
        mem_wren=1;
        init_step=init_step+1;
        $display("[write param]replace_del,slot_idx:%d",S_AXI_WDATA[27:20]);
      end
      else if(init_step==8) begin
        S_AXI_WDATA=0;
        S_AXI_WDATA[1]=1;//replace_del
        S_AXI_WDATA[27:20]=4;
        mem_address=INPUT_PARAM_OFFSET;
        mem_wren=1;
        init_step=init_step+1;
        $display("[write param]replace_del,slot_idx:%d",S_AXI_WDATA[27:20]);
      end
      else if(init_step==9) begin
        S_AXI_WDATA=0;
        S_AXI_WDATA[3]=1;//monitor_del
        S_AXI_WDATA[27:20]=1;
        mem_address=INPUT_PARAM_OFFSET;
        mem_wren=1;
        init_step=init_step+1;
        $display("[write param]monitor_del,slot_idx:%d",S_AXI_WDATA[27:20]);
      end
      else if(init_step==10) begin
        S_AXI_WDATA=0;
        S_AXI_WDATA[3]=1;//monitor_del
        S_AXI_WDATA[27:20]=2;
        mem_address=INPUT_PARAM_OFFSET;
        mem_wren=1;
        init_step=init_step+1;
        $display("[write param]monitor_del,slot_idx:%d",S_AXI_WDATA[27:20]);
      end
      else if(init_step==11) begin
        S_AXI_WDATA=0;
        mem_address=INPUT_PARAM_OFFSET;
        mem_wren=1;
        init_step=init_step+1;
      end
    end
	end
	
	
endmodule