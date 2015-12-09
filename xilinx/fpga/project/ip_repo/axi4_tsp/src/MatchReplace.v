module match_replace_module #(
  parameter FILTER_MAX_NUM = 32
)
(
    input [7:0] mpeg_data,
    input mpeg_clk,
    input mpeg_valid,
    input mpeg_sync,
    input rst,
	input axi_clk,
	input axi_wren,
    input replace_add,
    input replace_del,
    input monitor_add,
    input monitor_del,
    input [12:0] pid_in,
	input [7:0] slot_idx,
    output ts_clk,
    output ts_valid,
    output ts_sync,
    output [7:0] ts_out,
    output [31:0] dump_flag,
    output [7:0] dump_data
    );

parameter PACK_BYTE_SIZE = 188;
parameter FIFO_MAX = 2'h3;

  //match reg
  wire [7:0] din_8b;
  reg [7:0] dout_8b;
  reg [7:0] req_data;//map the param inout [7:0] data_io
  reg [7:0] buf_4B[3:0];//store 4B stream
  reg [1:0] write_ptr;
  reg [1:0] read_pos1,read_pos2;
  reg [7:0] counter;//max 188
  reg sync_flag,ts_valid_reg,ts_sync_reg;
  
  integer  k;
  //replace reg
  reg [12:0] REPLACE_PID[FILTER_MAX_NUM-1:0],
    MONITOR_PID[FILTER_MAX_NUM-1:0],LIVE_PID;
  reg[7:0] ts_replace[FILTER_MAX_NUM-1:0][187:0];
  reg[7:0] ts_cursor,add_cursor,dump_data_cursor,dump_data_reg;
  reg[FILTER_MAX_NUM-1:0] match_ret,monitor_ret;
  reg[7:0] replace_pid_num,replace_pid_idx,match_replace_slot_idx;
  reg[7:0] monitor_pid_num,monitor_pid_idx;
  
  assign din_8b = mpeg_data;
  assign ts_clk=mpeg_clk;
  assign ts_out=dout_8b;
  assign ts_valid=ts_valid_reg;
  assign ts_sync=ts_sync_reg;
  assign dump_flag=monitor_ret;
  assign dump_data=dump_data_reg;
  
  always @(posedge axi_clk) begin
	if(rst) begin
		add_cursor<=0;
		replace_pid_num<=0;
		replace_pid_idx<=0;
		monitor_pid_num<=0;
		monitor_pid_idx<=0;
	end
	else if(axi_wren) begin
		if(replace_add==1) begin//init replace reg
			if(replace_pid_num<=FILTER_MAX_NUM) begin
				if(add_cursor==0)
					replace_pid_idx=slot_idx;
				else begin
				  ts_replace[replace_pid_idx][add_cursor-1]=pid_in[7:0];
				  //$display("[input]ts_replace[%d][%d]:%x", 
				    //replace_pid_idx,add_cursor-1,ts_replace[replace_pid_idx][add_cursor-1]);
				end
				add_cursor=add_cursor+1;
				if(add_cursor==PACK_BYTE_SIZE+1) begin
					add_cursor<=0;
					REPLACE_PID[replace_pid_idx]=((ts_replace[replace_pid_idx][1]&5'h1F)<<8)+ts_replace[replace_pid_idx][2];   
					$display("ADD REPLACE PID:%h", REPLACE_PID[replace_pid_idx]);
					replace_pid_num=replace_pid_num+1;		
				end
			end
		end
		else if(replace_del==1)begin
			if(replace_pid_num>0) begin
				replace_pid_idx=slot_idx;
				$display("DEL REPLACE PID[%d]:%h", replace_pid_idx,REPLACE_PID[replace_pid_idx]);
				REPLACE_PID[replace_pid_idx]=0;
				ts_replace[replace_pid_idx][0]=0;
				replace_pid_num=replace_pid_num-1;
			end
		end
		else if(monitor_add==1) begin//read monitor pid
			if(monitor_pid_num<=FILTER_MAX_NUM) begin
				monitor_pid_idx=slot_idx;
				MONITOR_PID[monitor_pid_idx]=pid_in;
				$display("ADD MONITOR PID:%h", MONITOR_PID[monitor_pid_idx]);
				monitor_pid_num=monitor_pid_num+1;				
			end
		end
		else if(monitor_del==1) begin
			if(monitor_pid_num>0) begin
				monitor_pid_idx=slot_idx;
				$display("DEL MONITOR PID[%d]:%h", monitor_pid_idx,MONITOR_PID[monitor_pid_idx]);
				MONITOR_PID[monitor_pid_idx]=0;
				monitor_pid_num=monitor_pid_num-1;
			  end
		  end
	  end
	end	
	
  
  always @(posedge mpeg_clk) begin     
    if(rst) begin
		$display("rst:%d", rst);
		write_ptr<=0;
		counter<=0;
		ts_valid_reg<=0;
		ts_sync_reg<=0;
		sync_flag<=0;
		ts_cursor<=1;
		match_replace_slot_idx<=0;
		req_data<=0;
		dump_data_cursor<=0;
		match_ret<=0;
		monitor_ret<=0;
		for(k=0;k<=FILTER_MAX_NUM-1;k=k+1) begin             
			REPLACE_PID[k]<=0;
			ts_replace[k][0]<=0;
		end
    end
    else begin
      
	    
	    //write into fifo
      if(mpeg_valid == 1'b1) begin
        buf_4B[write_ptr]= din_8b;
              
        if(mpeg_sync == 1'b1) begin          
          sync_flag=1;
          counter=0;
          $display("mpeg_sync:%d", mpeg_sync);
        end
        ts_sync_reg=0;
        if(sync_flag==1) begin
          if(counter==FIFO_MAX) begin
            read_pos1=(write_ptr>2)?write_ptr-2:write_ptr+2;
            read_pos2=(write_ptr>1)?write_ptr-1:write_ptr+3;
            LIVE_PID=((buf_4B[read_pos1]&5'h1F)<<8)+buf_4B[read_pos2];
            monitor_ret=0;
            ts_sync_reg=1;
            for(k=0;k<=FILTER_MAX_NUM-1;k=k+1) begin
              if(replace_pid_num>0&&LIVE_PID==REPLACE_PID[k]) begin
                match_ret[k]<=1; 
                match_replace_slot_idx=k;
              end
              if(monitor_pid_num>0&&LIVE_PID==MONITOR_PID[k]) begin
                monitor_ret[k]=1;
              end
            end
            
            $display("pid now:%h",LIVE_PID);
            sync_flag<=0;
          end
        end
        
        
        $display("[MatchModule]counter:%d buf_4B[%d]:%x,match:%b,monitor_ret:%b", 
          counter,write_ptr,buf_4B[write_ptr],match_ret,monitor_ret);
        write_ptr=(write_ptr==FIFO_MAX)?0:write_ptr+1;
        counter=counter+1;
        if(counter>=PACK_BYTE_SIZE) begin          
          counter=0;
        end
        
        
        //read from fifo
        if(match_ret==0) begin //send live ts pack while not match
          ts_valid_reg=1;
          dout_8b=buf_4B[write_ptr];
        end
        else if(match_ret>0) begin
          ts_valid_reg=1;
          dout_8b=ts_replace[match_replace_slot_idx][ts_cursor];
          ts_cursor=ts_cursor+1;
          if(ts_cursor==PACK_BYTE_SIZE) begin
            ts_cursor<=1;//send the second byte of ts_replace data
            match_ret=0;
          end
        end
        
        //dump monitor data
        if(monitor_ret>0) begin
          dump_data_reg=buf_4B[write_ptr];
          //$display("[monitor output]idx:%d :%h", dump_data_cursor,dump_data_reg);
          dump_data_cursor=dump_data_cursor+1;
          if(dump_data_cursor>=PACK_BYTE_SIZE) begin
            dump_data_cursor=0;
          end
        end
      end
    else
      ts_valid_reg<=0;
      
    end
  end
endmodule