`timescale 1ns / 1ps
module ts_interface(
input								sys_clk,
input								fs_en,
input								glb_rst_n,
input				[31:0]			SYS_Baud_Num,
input				[31:0]		    SYS_Freq_Num,//32'd10000 --> 100M System Clock,
input				[7:0]			ts_din,
input								ts_syn,
output	reg					glb_start_en,
input								ts_rd_vld,
input								bbscramb_frame_vld,
//input								clk_1x,
output	reg					ts_byte_en,//delay: 10 cycles at clk_1x cmp bbscramb_frame_vld
output	reg	[7:0]			ts_byte_in,//delay: 10 cycles at clk_1x cmp bbscramb_frame_vld
input				[1:0]			srrc_mode,//00:0.35; 01:0.25; 00:0.20 
input	         [1:0]			mod_mode,
input	         [3:0] 		ldpc_mode,
input	          				frame_mode,
input   			 				pilot_mode,
output	reg					ts_clk
);

//parameter SYS_Freq_Num = 32'd10000;// modified by 2014.09.22///////////////////////////////????????????????????????????
//parameter SYS_Baud_Num = 32'd756;// modified by 2014.09.22

//wire	[31:0] SYS_Baud_Num;// = 32'd756;// modified by 2014.09.22

//assign SYS_Baud_Num = (SYS_Baud_mode == 2'b01)?32'd2500:32'd1000;

//always@(*)begin// modified by 2014.09.22
//case(SYS_Baud_mode)
//	2'b00:SYS_Baud_Num <= 32'd1000;///////////////////////////////????????????????????????????
//	2'b01:SYS_Baud_Num <= 32'd2500;///////////////////////////////????????????????????????????
//	2'b10:SYS_Baud_Num <= 32'd1000;///////////////////////////////????????????????????????????
//default:SYS_Baud_Num = 32'd1000;
//endcase
//end

reg											ts_rd_vld_1dly;//1dly
reg											ts_rd_vld_2dly;//2dly
reg											ts_rd_vld_3dly;//3dly
reg											ts_rd_vld_4dly;//4dly
reg											ts_rd_vld_5dly;//5dly
reg											ts_rd_vld_6dly;//6dly
reg											ts_rd_vld_7dly;//7dly
reg											ts_rd_vld_8dly;//8dly
reg											ts_rd_vld_9dly;//9dly
reg											ts_rd_vld_10dly;//10dly
reg											ts_rd_vld_11dly;//11dly
reg											ts_rd_vld_12dly;//12dly
reg											ts_rd_vld_13dly;//13dly
reg											ts_rd_vld_14dly;//14dly
reg											ts_rd_vld_15dly;//15dly
reg											ts_rd_vld_16dly;//16dly
reg											ts_rd_vld_17dly;//17dly
reg											ts_rd_vld_18dly;//18dly
reg											ts_rd_vld_19dly;//19dly



always @(posedge sys_clk)begin
	if(~glb_rst_n)begin	
		ts_rd_vld_1dly <= 1'b0;//1dly
		ts_rd_vld_2dly <= 1'b0;//2dly
		ts_rd_vld_3dly <= 1'b0;//3dly
		ts_rd_vld_4dly <= 1'b0;//4dly
		ts_rd_vld_5dly <= 1'b0;//5dly
		ts_rd_vld_6dly <= 1'b0;//6dly
		ts_rd_vld_7dly <= 1'b0;//7dly
		ts_rd_vld_8dly <= 1'b0;//8dly
		ts_rd_vld_9dly <= 1'b0;//9dly
		ts_rd_vld_10dly <= 1'b0;//10dly
		ts_rd_vld_11dly <= 1'b0;//11dly
		ts_rd_vld_12dly <= 1'b0;//12dly
		ts_rd_vld_13dly <= 1'b0;//13dly
		ts_rd_vld_14dly <= 1'b0;//14dly
		ts_rd_vld_15dly <= 1'b0;//15dly
		ts_rd_vld_16dly <= 1'b0;//16dly
		ts_rd_vld_17dly <= 1'b0;//17dly
		ts_rd_vld_18dly <= 1'b0;//18dly
		ts_rd_vld_19dly <= 1'b0;//19dly
	end
	else if(fs_en == 1'b1)begin
		ts_rd_vld_1dly <= ts_rd_vld;//1dly
		ts_rd_vld_2dly <= ts_rd_vld_1dly;//2dly
		ts_rd_vld_3dly <= ts_rd_vld_2dly;//3dly
		ts_rd_vld_4dly <= ts_rd_vld_3dly;//4dly
		ts_rd_vld_5dly <= ts_rd_vld_4dly;//5dly
		ts_rd_vld_6dly <= ts_rd_vld_5dly;//6dly
		ts_rd_vld_7dly <= ts_rd_vld_6dly;//7dly
		ts_rd_vld_8dly <= ts_rd_vld_7dly;//8dly
		ts_rd_vld_9dly <= ts_rd_vld_8dly;//9dly
		ts_rd_vld_10dly <= ts_rd_vld_9dly;//10dly
		ts_rd_vld_11dly <= ts_rd_vld_10dly;//11dly
		ts_rd_vld_12dly <= ts_rd_vld_11dly;//12dly
		ts_rd_vld_13dly <= ts_rd_vld_12dly;//13dly
		ts_rd_vld_14dly <= ts_rd_vld_13dly;//14dly
		ts_rd_vld_15dly <= ts_rd_vld_14dly;//15dly
		ts_rd_vld_16dly <= ts_rd_vld_15dly;//16dly
		ts_rd_vld_17dly <= ts_rd_vld_16dly;//17dly
		ts_rd_vld_18dly <= ts_rd_vld_17dly;//18dly
		ts_rd_vld_19dly <= ts_rd_vld_18dly;//19dly
	end
	else begin
	end
end


reg								bbscramb_frame_vld_1dly;//1dly
reg								bbscramb_frame_vld_2dly;//2dly
reg								bbscramb_frame_vld_3dly;//3dly
reg								bbscramb_frame_vld_4dly;//4dly
reg								bbscramb_frame_vld_5dly;//5dly
reg								bbscramb_frame_vld_6dly;//6dly
reg								bbscramb_frame_vld_7dly;//7dly
reg								bbscramb_frame_vld_8dly;//8dly
reg								bbscramb_frame_vld_9dly;//9dly
reg								bbscramb_frame_vld_10dly;//10dly
reg								bbscramb_frame_vld_11dly;//11dly
reg								bbscramb_frame_vld_12dly;//12dly
reg								bbscramb_frame_vld_13dly;//13dly
reg								bbscramb_frame_vld_14dly;//14dly
reg								bbscramb_frame_vld_15dly;//15dly
reg								bbscramb_frame_vld_16dly;//16dly
//reg								bbscramb_frame_vld_17dly;//17dly
//reg								bbscramb_frame_vld_18dly;//18dly
//reg								bbscramb_frame_vld_19dly;//19dly

wire		[7:0]					fifo_out;

always @(posedge sys_clk)begin
	if(~glb_rst_n)begin
		bbscramb_frame_vld_1dly <= 1'b0;//1dly
		bbscramb_frame_vld_2dly <= 1'b0;//2dly
		bbscramb_frame_vld_3dly <= 1'b0;//3dly
		bbscramb_frame_vld_4dly <= 1'b0;//4dly
		bbscramb_frame_vld_5dly <= 1'b0;//5dly
		bbscramb_frame_vld_6dly <= 1'b0;//6dly
		bbscramb_frame_vld_7dly <= 1'b0;//7dly
		bbscramb_frame_vld_8dly <= 1'b0;//8dly
		bbscramb_frame_vld_9dly <= 1'b0;//9dly
		
		bbscramb_frame_vld_10dly <= 1'b0;//10dly
		bbscramb_frame_vld_11dly <= 1'b0;//11dly
		bbscramb_frame_vld_12dly <= 1'b0;//12dly
		bbscramb_frame_vld_13dly <= 1'b0;//13dly
		bbscramb_frame_vld_14dly <= 1'b0;//14dly
		bbscramb_frame_vld_15dly <= 1'b0;//15dly
		bbscramb_frame_vld_16dly <= 1'b0;//16dly
		//bbscramb_frame_vld_17dly <= 1'b0;//17dly
		//bbscramb_frame_vld_18dly <= 1'b0;//18dly
		//bbscramb_frame_vld_19dly <= 1'b0;//19dly
		//ts_byte_en <= 1'b0;//10dly	
		//ts_byte_in <= 8'h00;//2dly

	end
	else if(fs_en == 1'b1) begin
		bbscramb_frame_vld_1dly <= bbscramb_frame_vld;//1dly
		bbscramb_frame_vld_2dly <= bbscramb_frame_vld_1dly;//2dly
		bbscramb_frame_vld_3dly <= bbscramb_frame_vld_2dly;//3dly
		bbscramb_frame_vld_4dly <= bbscramb_frame_vld_3dly;//4dly
		bbscramb_frame_vld_5dly <= bbscramb_frame_vld_4dly;//5dly
		bbscramb_frame_vld_6dly <= bbscramb_frame_vld_5dly;//6dly
		bbscramb_frame_vld_7dly <= bbscramb_frame_vld_6dly;//7dly
		bbscramb_frame_vld_8dly <= bbscramb_frame_vld_7dly;//8dly
		bbscramb_frame_vld_9dly <= bbscramb_frame_vld_8dly;//9dly	
		bbscramb_frame_vld_10dly <= bbscramb_frame_vld_9dly;//10dly
		bbscramb_frame_vld_11dly <= bbscramb_frame_vld_10dly;//11dly
		bbscramb_frame_vld_12dly <= bbscramb_frame_vld_11dly;//12dly
		bbscramb_frame_vld_13dly <= bbscramb_frame_vld_12dly;//13dly
		bbscramb_frame_vld_14dly <= bbscramb_frame_vld_13dly;//14dly
		bbscramb_frame_vld_15dly <= bbscramb_frame_vld_14dly;//15dly
		bbscramb_frame_vld_16dly <= bbscramb_frame_vld_15dly;//16dly
		//bbscramb_frame_vld_17dly <= bbscramb_frame_vld_16dly;//17dly
		//bbscramb_frame_vld_18dly <= bbscramb_frame_vld_17dly;//18dly
		//bbscramb_frame_vld_19dly <= bbscramb_frame_vld_18dly;//19dly
		//ts_byte_en <= bbscramb_frame_vld_9dly;//10dly	
		//ts_byte_in <= fifo_out;//2dly
	end
	else begin
	end
end


reg			[7:0]		bbheader_din;
reg						bbheader_syn_1pre;	
reg						bbheader_syn;	
reg						bbheader_head;	// before 1 cycles bbheader_syn
reg			[15:0]	bbheader_syncd;
wire			[15:0]	bbheader_dfl_cnt;
reg			[15:0]	bbheader_dfl_phase;

reg 			[12:0] 	TS_Bytes_Num_Frm;

//always @(posedge clk_1x)begin
//	if(~glb_rst_n)begin
//		bbheader_dfl_cnt <= 16'h0000;
//	end
//	else if(ts_rd_vld_6dly == 1'b1)begin
//			bbheader_dfl_cnt <= bbheader_dfl_phase + 8;
//	end
//	else begin
//		
//	end
//end

assign 	bbheader_dfl_cnt		= (ts_rd_vld_8dly == 1'b1)?(bbheader_dfl_phase+8):bbheader_dfl_phase;

always @(posedge sys_clk)begin
	if(~glb_rst_n)begin
		bbheader_dfl_phase <= 16'h0000;
	end
	else if(fs_en == 1'b1)begin
		if(bbheader_dfl_cnt >= 16'h5E0)begin
				bbheader_dfl_phase <= bbheader_dfl_cnt - 16'h5E0;
		end
		else begin
			bbheader_dfl_phase <= bbheader_dfl_cnt;
		end
	end
	else begin
	end
end

always @(posedge sys_clk)begin
	if(~glb_rst_n)begin
		bbheader_syncd <= 16'h0000;
	end
	else if((fs_en == 1'b1)&&(bbheader_head == 1'b1))begin
		if(bbheader_dfl_phase == 0)begin
			bbheader_syncd <= 16'h0000;
		end
		else begin
			bbheader_syncd <= 16'h5E0 - bbheader_dfl_phase;
		end
	end
	else begin
	end
end

//assign 	byte_en_cnt		= byte_en_phase+byte_en_step;
//
//always @(posedge sys_clk)begin
//if(~glb_rst_n)
//	byte_en_phase	<=0;
//else if(byte_en_cnt>=clk_period)
//	byte_en_phase<=byte_en_cnt-clk_period;
//else
//	byte_en_phase<=byte_en_cnt;
//end



parameter				BBHEADER_IDLE 		= 10'b0000000001;
parameter				BBHEADER_MATYPE1 	= 10'b0000000010;
parameter				BBHEADER_MATYPE2 	= 10'b0000000100;
parameter				BBHEADER_UPL1 		= 10'b0000001000;
parameter				BBHEADER_UPL2 		= 10'b0000010000;
parameter				BBHEADER_DFL1 		= 10'b0000100000;
parameter				BBHEADER_DFL2 		= 10'b0001000000;
parameter				BBHEADER_SYNC 		= 10'b0010000000;
parameter				BBHEADER_SYNCD1	= 10'b0100000000;
parameter				BBHEADER_SYNCD2	= 10'b1000000000;

reg			[9:0]		bbheader_state;

always @(posedge sys_clk)begin
	if(~glb_rst_n)begin
		bbheader_state <= BBHEADER_IDLE;
		bbheader_din <= 8'h00;
	end
	else begin
	case(bbheader_state)
	BBHEADER_IDLE:begin
		if(fs_en == 1'b1)begin
			if(bbheader_syn_1pre == 1'b1)begin
				bbheader_state <= BBHEADER_MATYPE1;
				bbheader_din <= {6'B111100,srrc_mode};
			end
			else begin
				bbheader_state <= BBHEADER_IDLE;
				bbheader_din <= 8'h00;
			end
		end
		else begin

		end
	end
	BBHEADER_MATYPE1:begin
		if(fs_en == 1'b1)begin
			bbheader_state <= BBHEADER_MATYPE2;
			bbheader_din <= 8'h00;
		end
		else begin
			
		end
	end
	BBHEADER_MATYPE2:begin
		if(fs_en == 1'b1)begin
			bbheader_state <= BBHEADER_UPL1;
			bbheader_din <= 8'h05;
		end
		else begin
			
		end
	end
	BBHEADER_UPL1:begin
		if(fs_en == 1'b1)begin
			bbheader_state <= BBHEADER_UPL2;
			bbheader_din <= 8'hE0;
		end
		else begin
			
		end
	end
	BBHEADER_UPL2:begin
		if(fs_en == 1'b1)begin
			bbheader_state <= BBHEADER_DFL1;
			bbheader_din <= TS_Bytes_Num_Frm[12:5];
		end
		else begin
			
		end
	end
	BBHEADER_DFL1:begin
		if(fs_en == 1'b1)begin
			bbheader_state <= BBHEADER_DFL2;
			bbheader_din <= {TS_Bytes_Num_Frm[4:0],3'b000};
		end
		else begin
			
		end
	end
	BBHEADER_DFL2:begin
		if(fs_en == 1'b1)begin
			bbheader_state <= BBHEADER_SYNC;
			bbheader_din <= 8'H47;
		end
		else begin
			
		end
	end
	BBHEADER_SYNC:begin
		if(fs_en == 1'b1)begin
			bbheader_state <= BBHEADER_SYNCD1;
			bbheader_din <= bbheader_syncd[15:8];
		end
		else begin
			
		end
	end
	BBHEADER_SYNCD1:begin
		if(fs_en == 1'b1)begin
			bbheader_state <= BBHEADER_SYNCD2;
			bbheader_din <= bbheader_syncd[7:0];
		end
		else begin
			
		end
	end
	BBHEADER_SYNCD2:begin
		if(fs_en == 1'b1)begin
			bbheader_state <= BBHEADER_IDLE;
			bbheader_din <= 8'h00;
		end
		else begin
			
		end
	end
	default:begin
		bbheader_state <= BBHEADER_IDLE;
		bbheader_din <= 8'h00;
	end
	endcase
	end
end


always @(posedge sys_clk)begin
	if(~glb_rst_n)begin
		bbheader_syn_1pre <= 1'b0;
		bbheader_syn <= 1'b0;
	end
	else if(fs_en == 1'b1)begin
		bbheader_syn_1pre <= (bbscramb_frame_vld_7dly == 1'b1)&&(bbscramb_frame_vld_16dly == 1'b0);
		bbheader_syn <= bbheader_syn_1pre;
	end
	else begin
	end
end

always @(posedge sys_clk)begin
	if(~glb_rst_n)begin
		bbheader_head <= 1'b0;
	end
	else if(fs_en == 1'b1)begin
		bbheader_head <= (bbscramb_frame_vld_7dly == 1'b1)&&(bbscramb_frame_vld_8dly == 1'b0);
	end
	else begin
	end
end

wire						bbheader_crc8_oe;
wire			[7:0]		bbheader_crc_byte_out;
wire			[7:0]		bbheader_crc;

crc8 uut_crc8_bbheader(
.sys_clk					(sys_clk),
.glb_rst_n				(glb_rst_n),
//////////////////////////////////////////////////////////////
.ts_in					(bbheader_din),// @ sys_clk
.ts_syn					(bbheader_syn&&fs_en),// @ sys_clk
.ts_head					(bbheader_head&&fs_en),// @ sys_clk
//////////////////////////////////////////////////////////////
.crc8_cync_en			(),//(crc8_cync_en),
.crc8_oe					(),//(bbheader_crc8_oe),
.crc						(bbheader_crc),//(crc),
.crc_byte_out			()//(bbheader_crc_byte_out)
//////////////////////////////////////////////////////////////
);


parameter				TS_OUT_IDLE 			= 4'b0001;
parameter				TS_OUT_BBHEADER 		= 4'b0010;
parameter				TS_OUT_BBHEADER_CRC 	= 4'b0100;
parameter				TS_OUT_DATA 			= 4'b1000;

reg				[3:0]	ts_out_state;
reg					    fifo_rd_fst_flg;

always @(posedge sys_clk)begin
	if(~glb_rst_n)begin
		ts_byte_en <= 0;
		ts_byte_in <= 8'h00;
		ts_out_state <= TS_OUT_IDLE;
		fifo_rd_fst_flg <= 0;
	end
	else begin
		case(ts_out_state)
		TS_OUT_IDLE:begin
			if(fs_en == 1'b1)begin
				if(bbheader_syn == 1)begin
					ts_byte_en <= 1;
					ts_byte_in <= bbheader_din;
					ts_out_state <= TS_OUT_BBHEADER;
				end
				else begin
					ts_byte_en <= 0;
					ts_byte_in <= 8'h00;
					ts_out_state <= TS_OUT_IDLE;
				end
			end
			else begin
			end
		end
		TS_OUT_BBHEADER:begin
			if(fs_en == 1'b1)begin
				if(bbheader_syn == 1)begin
					ts_byte_en <= 1;
					ts_byte_in <= bbheader_din;
					ts_out_state <= TS_OUT_BBHEADER;
				end
				else begin
					ts_byte_en <= 1;
					ts_byte_in <= bbheader_crc;
					ts_out_state <= TS_OUT_BBHEADER_CRC; 
				end
			end
			else begin
			end
		end
//		TS_OUT_BBHEADER_CRC:begin
//			if(fs_en == 1'b1)begin

//				if(fifo_rd_fst_flg == 0)begin
//				    fifo_rd_fst_flg <= 1;
//				    ts_byte_en <= 0;
//				    ts_byte_in <= 0;//8'h47;//fifo_out; 2015.09.23
//			    end
//				else begin	
//				    ts_byte_en <= ts_rd_vld_19dly;			    
//				    fifo_rd_fst_flg <= 1;
//				    ts_byte_in <= fifo_out; //2015.09.23
//				end
//				ts_out_state <= TS_OUT_DATA;
//			end
//			else begin
//			end
//		end
		TS_OUT_BBHEADER_CRC:begin
                    if(fs_en == 1'b1)begin
                        ts_byte_en <= ts_rd_vld_19dly;
                        if(fifo_rd_fst_flg == 0)begin
                            fifo_rd_fst_flg <= 1;
                            ts_byte_in <= 8'h47;//fifo_out; 2015.09.23
                        end
                        else begin                    
                            fifo_rd_fst_flg <= 1;
                            ts_byte_in <= fifo_out; //2015.09.23
                        end
                        ts_out_state <= TS_OUT_DATA;
                    end
                    else begin
                    end
                end
		TS_OUT_DATA:begin
			if(fs_en == 1'b1)begin
				if(ts_rd_vld_19dly == 1'b1)begin
					ts_byte_en <= ts_rd_vld_19dly;
					ts_byte_in <= fifo_out;
					ts_out_state <= TS_OUT_DATA;
				end
				else begin
					ts_byte_en <= 0;
					ts_byte_in <= 8'h00;
					ts_out_state <= TS_OUT_IDLE;
				end
			end
			else begin
			end
		end
		default:begin
			ts_byte_en <= 0;
			ts_byte_in <= 8'h00;
			ts_out_state <= TS_OUT_IDLE;
		end
		endcase
	end
end



//output	  rdempty;
//output	  wrfull;
wire			[13:0]  			wrusedw;

//ts_input_fifo uut_ts_input_fifo(
//.aclr							(~glb_rst_n),
//.data							(ts_din),
//.rdclk						(sys_clk),
//.rdreq						(ts_rd_vld_18dly&&fs_en),//(bbscramb_frame_vld_8dly),
//.wrclk						(sys_clk),
//.wrreq						(ts_syn),
//.q								(fifo_out),
//.rdempty						(),
//.wrfull						(),
//.wrusedw						(wrusedw)
//);

mepg2_ts_fifo uut_ts_input_fifo (
  .rst(~glb_rst_n),                      // input wire rst
  .wr_clk(sys_clk),                // input wire wr_clk
  .rd_clk(sys_clk),                // input wire rd_clk
  .din(ts_din),                      // input wire [7 : 0] din
  .wr_en(ts_syn),                  // input wire wr_en
  .rd_en(ts_rd_vld_18dly&&fs_en),                  // input wire rd_en
  .dout(fifo_out),                    // output wire [7 : 0] dout
  .full(),                    // output wire full
  .wr_ack(),                // output wire wr_ack
  .overflow(),            // output wire overflow
  .empty(),                  // output wire empty
  .valid(),                  // output wire valid
  .underflow(),          // output wire underflow
  .rd_data_count(),  // output wire [12 : 0] rd_data_count
  .wr_data_count(wrusedw)  // output wire [12 : 0] wr_data_count
);


parameter				IDLE_STATE = 2'b01;
parameter				WORK_STATE = 2'b10;

reg				[1:0]				state;


always @(posedge sys_clk)begin
	if(~glb_rst_n)begin
		glb_start_en	<= 1'b0;
		state <= IDLE_STATE;
	end
	else begin
		case(state)
		IDLE_STATE:begin
			if(fs_en == 1'b1)begin
				if(wrusedw == TS_Bytes_Num_Frm)begin//12'h00ff)begin
					state <= WORK_STATE;
					glb_start_en	<= 1'b1;
				end
				else begin
					state <= IDLE_STATE;
				end
			end
			else begin
				
			end
		end
		WORK_STATE:begin
			if(fs_en == 1'b1)begin
				state <= WORK_STATE;
				glb_start_en	<= 1'b1;
			end
			else begin
			end
		end
		default:begin
			glb_start_en	<= 1'b0;
			state <= IDLE_STATE;
		end
		endcase
	end
end


always @(posedge sys_clk)begin
	if(~glb_rst_n)begin
		TS_Bytes_Num_Frm <= 13'd0;
	end
	else begin
		case({frame_mode,ldpc_mode})
		5'h00:begin
			TS_Bytes_Num_Frm <= 13'd1991;
		end
		5'h01:begin
			TS_Bytes_Num_Frm <= 13'd2666;
		end
		5'h02:begin
			TS_Bytes_Num_Frm <= 13'd3206;
		end
		5'h03:begin
			TS_Bytes_Num_Frm <= 13'd4016;
		end
		5'h04:begin
			TS_Bytes_Num_Frm <= 13'd4826;
		end
		5'h05:begin
			TS_Bytes_Num_Frm <= 13'd5370;
		end
		5'h06:begin
			TS_Bytes_Num_Frm <= 13'd6041;
		end
		5'h07:begin
			TS_Bytes_Num_Frm <= 13'd6446;
		end
		5'h08:begin
			TS_Bytes_Num_Frm <= 13'd6720;
		end
		5'h09:begin
			TS_Bytes_Num_Frm <= 13'd7174;
		end
		5'h0a:begin
			TS_Bytes_Num_Frm <= 13'd7264;
		end
		5'h10:begin
			TS_Bytes_Num_Frm <= 13'd374;
		end
		5'h11:begin
			TS_Bytes_Num_Frm <= 13'd644;
		end
		5'h12:begin
			TS_Bytes_Num_Frm <= 13'd779;
		end
		5'h13:begin
			TS_Bytes_Num_Frm <= 13'd869;
		end
		5'h14:begin
			TS_Bytes_Num_Frm <= 13'd1184;
		end
		5'h15:begin
			TS_Bytes_Num_Frm <= 13'd1319;
		end
		5'h16:begin
			TS_Bytes_Num_Frm <= 13'd1454;
		end
		5'h17:begin
			TS_Bytes_Num_Frm <= 13'd1544;
		end
		5'h18:begin
			TS_Bytes_Num_Frm <= 13'd1634;
		end
		5'h19:begin
			TS_Bytes_Num_Frm <= 13'd1769;
		end
		default:begin
			TS_Bytes_Num_Frm <= 13'd0;
		end
		endcase
	end
end


reg			[15:0]		PL_Bytes_Num_Frm;

always @(posedge sys_clk)begin
	if(~glb_rst_n)begin
		PL_Bytes_Num_Frm <= 16'd0;
	end
	else begin
		case({frame_mode,pilot_mode,mod_mode})
		4'h0:begin
			PL_Bytes_Num_Frm <= 16'd32490;
		end
		4'h1:begin
			PL_Bytes_Num_Frm <= 16'd21690;
		end
		4'h2:begin
			PL_Bytes_Num_Frm <= 16'd16290;
		end
		4'h3:begin
			PL_Bytes_Num_Frm <= 16'd13050;
		end
		4'h4:begin
			PL_Bytes_Num_Frm <= 16'd33282;
		end
		4'h5:begin
			PL_Bytes_Num_Frm <= 16'd22194;
		end
		4'h6:begin
			PL_Bytes_Num_Frm <= 16'd16686;
		end
		4'h7:begin
			PL_Bytes_Num_Frm <= 16'd13338;
		end
		4'h8:begin
			PL_Bytes_Num_Frm <= 16'd8190;
		end
		4'h9:begin
			PL_Bytes_Num_Frm <= 16'd5490;
		end
		4'ha:begin
			PL_Bytes_Num_Frm <= 16'd4140;
		end
		4'hb:begin
			PL_Bytes_Num_Frm <= 16'd3330;
		end
		4'hc:begin
			PL_Bytes_Num_Frm <= 16'd8370;
		end
		4'hd:begin
			PL_Bytes_Num_Frm <= 16'd5598;
		end
		4'he:begin
			PL_Bytes_Num_Frm <= 16'd4212;
		end
		4'hf:begin
			PL_Bytes_Num_Frm <= 16'd3402;
		end
		default:begin
			PL_Bytes_Num_Frm <= 16'd0;
		end
		endcase
	end
end


//reg			[16:0]		PL_minus_TS_Bytes;
//
//always @(posedge sys_clk)begin
//	if(~glb_rst_n)begin
//		PL_minus_TS_Bytes <= 17'd0;
//	end
//	else begin
//		PL_minus_TS_Bytes <= PL_Bytes_Num_Frm - TS_Bytes_Num_Frm;
//	end
//	end
//
//reg			[16:0]		Bytes_Cnt_Frm;
//
//always @(posedge sys_clk)begin
//	if(~glb_rst_n)begin
//		Bytes_Cnt_Frm <= 17'd0;
//		ts_clk <= 1'b0;
//	end
//	else if(fs_en)begin
//		if(Bytes_Cnt_Frm >= (PL_minus_TS_Bytes - 1))begin
//			Bytes_Cnt_Frm <= Bytes_Cnt_Frm - PL_minus_TS_Bytes;
//			ts_clk <= 1'b1;
//		end
//		else begin
//			Bytes_Cnt_Frm <= Bytes_Cnt_Frm + TS_Bytes_Num_Frm;
//			ts_clk <= 1'b0;
//		end
//	end
//	else begin
//		ts_clk <= 1'b0;
//	end
//	end
	
	
//	//ts_clk 

reg [31:0]			byte_en_step;
reg [31:0]			clk_period;
wire [31:0]			byte_en_cnt;
reg  [31:0]			byte_en_phase;

reg         clk_period_rdy;

always @(posedge sys_clk)begin
if(~glb_rst_n)begin
	byte_en_step   <= 0;
	clk_period 	 	<= 0;
	clk_period_rdy <= 0;
end
else begin
	byte_en_step   <= TS_Bytes_Num_Frm*SYS_Baud_Num;//32'd3036096 ;
	clk_period 	 	<= PL_Bytes_Num_Frm*SYS_Freq_Num;//32'd259920000;
	clk_period_rdy <= 1;
end
end

reg											clk_period_rdy_1dly;//1dly
reg											clk_period_rdy_2dly;//2dly
reg											clk_period_rdy_3dly;//3dly
reg											clk_period_rdy_4dly;//4dly
reg											clk_period_rdy_5dly;//5dly
reg											clk_period_rdy_6dly;//6dly
reg											clk_period_rdy_7dly;//7dly


always @(posedge sys_clk)begin
	if(~glb_rst_n)begin	
		clk_period_rdy_1dly <= 1'b0;//1dly
		clk_period_rdy_2dly <= 1'b0;//2dly
		clk_period_rdy_3dly <= 1'b0;//3dly
		clk_period_rdy_4dly <= 1'b0;//4dly
		clk_period_rdy_5dly <= 1'b0;//5dly
		clk_period_rdy_6dly <= 1'b0;//6dly
		clk_period_rdy_7dly <= 1'b0;//7dly
	end
	else begin
		clk_period_rdy_1dly <= clk_period_rdy;//1dly
        clk_period_rdy_2dly <= clk_period_rdy_1dly;//2dly
        clk_period_rdy_3dly <= clk_period_rdy_2dly;//3dly
        clk_period_rdy_4dly <= clk_period_rdy_3dly;//4dly
        clk_period_rdy_5dly <= clk_period_rdy_4dly;//5dly
        clk_period_rdy_6dly <= clk_period_rdy_5dly;//6dly
        clk_period_rdy_7dly <= clk_period_rdy_6dly;//7dly
	end
end

//assign 	byte_en_step   = TS_Bytes_Num_Frm*SYS_Baud_Num;//32'd3036096 ;
//assign	clk_period 	 	= PL_Bytes_Num_Frm*SYS_Freq_Num;//32'd259920000;
assign 	byte_en_cnt		= byte_en_phase+byte_en_step;

always @(posedge sys_clk)begin
if(~glb_rst_n)
	byte_en_phase	<=0;
else if((byte_en_cnt>=clk_period)&&(clk_period_rdy_7dly))
	byte_en_phase<=byte_en_cnt-clk_period;
else
	byte_en_phase<=byte_en_cnt;
end

reg        ts_clk_pre; 

always @(posedge sys_clk)begin
if(~glb_rst_n)
	ts_clk_pre<=1'b0;
else if((byte_en_cnt>=clk_period)&&(clk_period_rdy_7dly))
	ts_clk_pre<=1'b1;
else
	ts_clk_pre<=1'b0;
end



reg											ts_clk_pre_1dly;//1dly
reg											ts_clk_pre_2dly;//2dly
reg											ts_clk_pre_3dly;//3dly
reg											ts_clk_pre_4dly;//4dly
reg											ts_clk_pre_5dly;//5dly
reg											ts_clk_pre_6dly;//6dly
reg											ts_clk_pre_7dly;//7dly


always @(posedge sys_clk)begin
	if(~glb_rst_n)begin	
		ts_clk_pre_1dly <= 1'b0;//1dly
		ts_clk_pre_2dly <= 1'b0;//2dly
		ts_clk_pre_3dly <= 1'b0;//3dly
		ts_clk_pre_4dly <= 1'b0;//4dly
		ts_clk_pre_5dly <= 1'b0;//5dly
		ts_clk_pre_6dly <= 1'b0;//6dly
		ts_clk_pre_7dly <= 1'b0;//7dly
		ts_clk <= 1'b0;//8dly
	end
	else begin
		ts_clk_pre_1dly <= ts_clk_pre;//1dly
        ts_clk_pre_2dly <= ts_clk_pre_1dly;//2dly
        ts_clk_pre_3dly <= ts_clk_pre_2dly;//3dly
        ts_clk_pre_4dly <= ts_clk_pre_3dly;//4dly
        ts_clk_pre_5dly <= ts_clk_pre_4dly;//5dly
        ts_clk_pre_6dly <= ts_clk_pre_5dly;//6dly
        ts_clk_pre_7dly <= ts_clk_pre_6dly;//7dly
        ts_clk <= ts_clk_pre_7dly;//8dly
	end
end


	
//	//byte_en 
//assign 	byte_en_step   =32'd3036096 ;
//assign	clk_period 	 =32'd259920000;
//assign 	byte_en_cnt=byte_en_phase+byte_en_step;
//always @ (posedge sys_clk)begin
//if(reset)
//byte_en_phase	<=0;
//else if(byte_en_cnt>=clk_period)
//byte_en_phase<=byte_en_cnt-clk_period;
//else
//byte_en_phase<=byte_en_cnt;
//end
//always @ (posedge sys_clk)begin
//if(reset)
//byte_en<=1'b0;
//else if(byte_en_cnt>=clk_period)
//byte_en<=1'b1;
//else
//byte_en<=1'b0;
//end
	
	
	
endmodule
