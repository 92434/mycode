`timescale 1ns / 1ps
module mapper(
input	          							sys_clk,
input											fs_en,
input	          							rst_n,
//////////////////////////////////////////////////////////////
input	          			[1:0]			mod_mode,
input	          			[3:0] 			ldpc_mode,
input	          							init_vld,
input	          							frame_vld,
input   			 		[4:0]     		byte_in,
//////////////////////////////////////////////////////////////
output  reg  								oe,
output  reg  	signed	   	[15:0]     		symbol_re_out,
output  reg  	signed	   	[15:0]     		symbol_im_out
//////////////////////////////////////////////////////////////
);


 reg  			signed		[15:0]     		qpsk_symbol_re_out;
 reg  			signed		[15:0]     		qpsk_symbol_im_out;

always @(posedge sys_clk)begin
	if(~rst_n)begin
		qpsk_symbol_re_out <= 16'sh0;
		qpsk_symbol_im_out <= 16'sh0;
	end
	else if(fs_en == 1'b1)begin
		case ({byte_in[4:3]})
			2'h0:begin
				qpsk_symbol_re_out <= 16'sh0b50;
				qpsk_symbol_im_out <= 16'sh0b50;
			end
			2'h1:begin
				qpsk_symbol_re_out <= 16'sh0b50;
				qpsk_symbol_im_out <= 16'shf4b0;
			end
			2'h2:begin
				qpsk_symbol_re_out <= 16'shf4b0;
				qpsk_symbol_im_out <= 16'sh0b50;
			end
			2'h3:begin
				qpsk_symbol_re_out <= 16'shf4b0;
				qpsk_symbol_im_out <= 16'shf4b0;
			end
			default:begin
				qpsk_symbol_re_out <= 16'sh0;
				qpsk_symbol_im_out <= 16'sh0;
			end
		endcase
	end
	else begin
	end
end


reg  			signed		[15:0]     		psk8_symbol_re_out;
reg  			signed		[15:0]     		psk8_symbol_im_out;

always @(posedge sys_clk)begin
	if(~rst_n)begin
		psk8_symbol_re_out <= 16'sh0;
		psk8_symbol_im_out <= 16'sh0;
	end
	else if(fs_en == 1'b1)begin
		case ({byte_in[4:2]})
			3'h0:begin
				psk8_symbol_re_out <= 16'sh0b50;
				psk8_symbol_im_out <= 16'sh0b50;
			end
			3'h1:begin
				psk8_symbol_re_out <= 16'sh1000;
				psk8_symbol_im_out <= 16'sh0000;
			end
			3'h2:begin
				psk8_symbol_re_out <= 16'shf000;
				psk8_symbol_im_out <= 16'sh0000;
			end
			3'h3:begin
				psk8_symbol_re_out <= 16'shf4b0;
				psk8_symbol_im_out <= 16'shf4b0;
			end
			3'h4:begin
				psk8_symbol_re_out <= 16'sh0000;
				psk8_symbol_im_out <= 16'sh1000;
			end
			3'h5:begin
				psk8_symbol_re_out <= 16'sh0b50;
				psk8_symbol_im_out <= 16'shf4b0;
			end
			3'h6:begin
				psk8_symbol_re_out <= 16'shf4b0;
				psk8_symbol_im_out <= 16'sh0b50;
			end
			3'h7:begin
				psk8_symbol_re_out <= 16'sh0000;
				psk8_symbol_im_out <= 16'shf000;
			end
			default:begin
				psk8_symbol_re_out <= 16'sh0000;
				psk8_symbol_im_out <= 16'sh0000;
			end
		endcase
	end
	else begin
	end
end


reg  			signed		[15:0]     		psk16_symbol_re_out;
reg  			signed		[15:0]     		psk16_symbol_im_out;

always @(posedge sys_clk)begin
	if(~rst_n)begin
		psk16_symbol_re_out <= 16'sh0;
		psk16_symbol_im_out <= 16'sh0;
	end
	else if(fs_en == 1'b1)begin
		case ({byte_in[4:1]})
			4'h0:begin
				psk16_symbol_re_out <= 16'sh0b50;
				psk16_symbol_im_out <= 16'sh0b50;
			end
			4'h1:begin
				psk16_symbol_re_out <= 16'sh0b50;
				psk16_symbol_im_out <= 16'shf4b0;
			end
			4'h2:begin
				psk16_symbol_re_out <= 16'shf4b0;
				psk16_symbol_im_out <= 16'sh0b50;
			end
			4'h3:begin
				psk16_symbol_re_out <= 16'shf4b0;
				psk16_symbol_im_out <= 16'shf4b0;
			end
			4'h4:begin
				psk16_symbol_re_out <= 16'sh0f74;
				psk16_symbol_im_out <= 16'sh0424;
			end
			4'h5:begin
				psk16_symbol_re_out <= 16'sh0f74;
				psk16_symbol_im_out <= 16'shfbdc;
			end
			4'h6:begin
				psk16_symbol_re_out <= 16'shf08c;
				psk16_symbol_im_out <= 16'sh0424;
			end
			4'h7:begin
				psk16_symbol_re_out <= 16'shf08c;
				psk16_symbol_im_out <= 16'shfbdc;
			end
			4'h8:begin
				psk16_symbol_re_out <= 16'sh0424;
				psk16_symbol_im_out <= 16'sh0f74;
			end
			4'h9:begin
				psk16_symbol_re_out <= 16'sh0424;
				psk16_symbol_im_out <= 16'shf08c;
			end
			4'ha:begin
				psk16_symbol_re_out <= 16'shfbdc;
				psk16_symbol_im_out <= 16'sh0f74;
			end
			4'hb:begin
				psk16_symbol_re_out <= 16'shfbdc;
				psk16_symbol_im_out <= 16'shf08c;
			end
			4'hc:begin
				psk16_symbol_re_out <= 16'sh0b50;
				psk16_symbol_im_out <= 16'sh0b50;
			end
			4'hd:begin
				psk16_symbol_re_out <= 16'sh0b50;
				psk16_symbol_im_out <= 16'shf4b0;
			end
			4'he:begin
				psk16_symbol_re_out <= 16'shf4b0;
				psk16_symbol_im_out <= 16'sh0b50;
			end
			4'hf:begin
				psk16_symbol_re_out <= 16'shf4b0;
				psk16_symbol_im_out <= 16'shf4b0;
			end
			default:begin
				psk16_symbol_re_out <= 16'sh0;
				psk16_symbol_im_out <= 16'sh0;
			end
		endcase
	end
	else begin
	end
end


reg  			signed		[15:0]     		psk16_R1_reg;
reg  			signed		[15:0]     		psk16_R2_reg;

always @(posedge sys_clk)begin
	if(~rst_n)begin
		psk16_R1_reg <= 16'sh0;
	end
	else begin
		case (ldpc_mode)
			4'h0 : psk16_R1_reg <= 16'sh05c5 ; 
			4'h1 : psk16_R1_reg <= 16'sh05c5 ; 
			4'h2 : psk16_R1_reg <= 16'sh05c5 ; 
			4'h3 : psk16_R1_reg <= 16'sh05c5 ; 
			4'h4 : psk16_R1_reg <= 16'sh05c5 ; 
			4'h5 : psk16_R1_reg <= 16'sh05c5 ; 
			4'h6 : psk16_R1_reg <= 16'sh065a ; 
			4'h7 : psk16_R1_reg <= 16'sh0693 ; 
			4'h8 : psk16_R1_reg <= 16'sh06b1 ; 
			4'h9 : psk16_R1_reg <= 16'sh06f0 ; 
			4'ha : psk16_R1_reg <= 16'sh0704 ; 
			default:psk16_R1_reg <= 16'sh0;
		endcase
	end
end

always @(posedge sys_clk)begin
	if(~rst_n)begin
		psk16_R2_reg <= 16'sh0;
	end
	else begin
		case (ldpc_mode)
			4'h0 : psk16_R2_reg <= 16'sh122c ;
			4'h1 : psk16_R2_reg <= 16'sh122c ;
			4'h2 : psk16_R2_reg <= 16'sh122c ;
			4'h3 : psk16_R2_reg <= 16'sh122c ;
			4'h4 : psk16_R2_reg <= 16'sh122c ;
			4'h5 : psk16_R2_reg <= 16'sh122c ;
			4'h6 : psk16_R2_reg <= 16'sh121b ;
			4'h7 : psk16_R2_reg <= 16'sh1215 ;
			4'h8 : psk16_R2_reg <= 16'sh1211 ;
			4'h9 : psk16_R2_reg <= 16'sh1209 ;
			4'ha : psk16_R2_reg <= 16'sh1207 ;
			default:psk16_R2_reg <= 16'sh0;
		endcase
	end
end

reg  			signed		[15:0]     		psk16_R_reg;

always @(posedge sys_clk)begin
	if(~rst_n)begin
		psk16_R_reg <= 16'sh0;
	end
	else if(fs_en == 1'b1)begin
		if(byte_in[4:3]==2'b11)begin
			psk16_R_reg <= psk16_R1_reg;
		end
		else begin
			psk16_R_reg <= psk16_R2_reg;
		end
	end
	else begin
	end
end


reg  			signed		[15:0]     		psk32_R1_reg;
reg  			signed		[15:0]     		psk32_R2_reg;
reg  			signed		[15:0]     		psk32_R3_reg;

always @(posedge sys_clk)begin
	if(~rst_n)begin
		psk32_R1_reg <= 16'sh0;
	end
	else begin
		case (ldpc_mode)
			4'h0 : psk32_R1_reg <= 16'sh03e0 ;
			4'h1 : psk32_R1_reg <= 16'sh03e0 ;
			4'h2 : psk32_R1_reg <= 16'sh03e0 ;
			4'h3 : psk32_R1_reg <= 16'sh03e0 ;
			4'h4 : psk32_R1_reg <= 16'sh03e0 ;
			4'h5 : psk32_R1_reg <= 16'sh03e0 ;
			4'h6 : psk32_R1_reg <= 16'sh03e0 ;
			4'h7 : psk32_R1_reg <= 16'sh042a ;
			4'h8 : psk32_R1_reg <= 16'sh045b ;
			4'h9 : psk32_R1_reg <= 16'sh04a2 ;
			4'ha : psk32_R1_reg <= 16'sh04aa ;
			default:psk32_R1_reg <= 16'sh0;
		endcase
	end
end

always @(posedge sys_clk)begin
	if(~rst_n)begin
		psk32_R2_reg <= 16'sh0;
	end
	else begin
		case (ldpc_mode)
			4'h0 : psk32_R2_reg <= 16'sh0b02 ;
			4'h1 : psk32_R2_reg <= 16'sh0b02 ;
			4'h2 : psk32_R2_reg <= 16'sh0b02 ;
			4'h3 : psk32_R2_reg <= 16'sh0b02 ;
			4'h4 : psk32_R2_reg <= 16'sh0b02 ;
			4'h5 : psk32_R2_reg <= 16'sh0b02 ;
			4'h6 : psk32_R2_reg <= 16'sh0b02 ;
			4'h7 : psk32_R2_reg <= 16'sh0b54 ;
			4'h8 : psk32_R2_reg <= 16'sh0b7f ;
			4'h9 : psk32_R2_reg <= 16'sh0bc6 ;
			4'ha : psk32_R2_reg <= 16'sh0bcd ;
			default:psk32_R2_reg <= 16'sh0;
		endcase
	end
end


always @(posedge sys_clk)begin
	if(~rst_n)begin
		psk32_R3_reg <= 16'sh0;
	end
	else begin
		case (ldpc_mode)
			4'h0 : psk32_R3_reg <= 16'sh146e ; 
			4'h1 : psk32_R3_reg <= 16'sh146e ; 
			4'h2 : psk32_R3_reg <= 16'sh146e ; 
			4'h3 : psk32_R3_reg <= 16'sh146e ; 
			4'h4 : psk32_R3_reg <= 16'sh146e ; 
			4'h5 : psk32_R3_reg <= 16'sh146e ; 
			4'h6 : psk32_R3_reg <= 16'sh146e ; 
			4'h7 : psk32_R3_reg <= 16'sh1449 ; 
			4'h8 : psk32_R3_reg <= 16'sh1434 ; 
			4'h9 : psk32_R3_reg <= 16'sh1411 ; 
			4'ha : psk32_R3_reg <= 16'sh140e ; 
			default:psk32_R3_reg <= 16'sh0;
		endcase
	end
end

reg  			signed		[15:0]     		psk32_R_reg;



reg  			signed		[15:0]     		psk32_symbol_re_out;
reg  			signed		[15:0]     		psk32_symbol_im_out;

always @(posedge sys_clk)begin
	if(~rst_n)begin
		psk32_symbol_re_out <= 16'sh0;
		psk32_symbol_im_out <= 16'sh0;
		psk32_R_reg <= 16'sh0;
	end
	else if(fs_en == 1'b1)begin
		case ({byte_in[4:0]})
		5'h00:begin                            
			psk32_symbol_re_out <= 16'sh0b50;
			psk32_symbol_im_out <= 16'sh0b50;
			psk32_R_reg <= psk32_R2_reg;    
		end                                   
		5'h01:begin                            
					psk32_symbol_re_out <= 16'sh0424;
					psk32_symbol_im_out <= 16'sh0f74;
					psk32_R_reg <= psk32_R2_reg;    
		end                                   
		5'h02:begin                            
					psk32_symbol_re_out <= 16'sh0b50;
					psk32_symbol_im_out <= 16'shf4b0;
					psk32_R_reg <= psk32_R2_reg;    
		end                                   
		5'h03:begin                            
					psk32_symbol_re_out <= 16'sh0424;
					psk32_symbol_im_out <= 16'shf08c;
					psk32_R_reg <= psk32_R2_reg;    
		end                                   
		5'h04:begin                            
					psk32_symbol_re_out <= 16'shf4b0;
					psk32_symbol_im_out <= 16'sh0b50;
					psk32_R_reg <= psk32_R2_reg;    
		end                                   
		5'h05:begin                            
					psk32_symbol_re_out <= 16'shfbdc;
					psk32_symbol_im_out <= 16'sh0f74;
					psk32_R_reg <= psk32_R2_reg;    
		end                                   
		5'h06:begin                            
					psk32_symbol_re_out <= 16'shf4b0;
					psk32_symbol_im_out <= 16'shf4b0;
					psk32_R_reg <= psk32_R2_reg;    
		end                                   
		5'h07:begin                            
					psk32_symbol_re_out <= 16'shfbdc;
					psk32_symbol_im_out <= 16'shf08c;
					psk32_R_reg <= psk32_R2_reg;    
		end                                   
		5'h08:begin                            
					psk32_symbol_re_out <= 16'sh0ec8;
					psk32_symbol_im_out <= 16'sh061f;
					psk32_R_reg <= psk32_R3_reg;    
		end                                   
		5'h09:begin                            
					psk32_symbol_re_out <= 16'sh061f;
					psk32_symbol_im_out <= 16'sh0ec8;
					psk32_R_reg <= psk32_R3_reg;    
		end                                   
		5'h0a:begin                            
					psk32_symbol_re_out <= 16'sh0b50;
					psk32_symbol_im_out <= 16'shf4b0;
					psk32_R_reg <= psk32_R3_reg;    
		end                                   
		5'h0b:begin                            
					psk32_symbol_re_out <= 16'sh0000;
					psk32_symbol_im_out <= 16'shf000;
					psk32_R_reg <= psk32_R3_reg;    
		end                                   
		5'h0c:begin                            
					psk32_symbol_re_out <= 16'shf4b0;
					psk32_symbol_im_out <= 16'sh0b50;
					psk32_R_reg <= psk32_R3_reg;    
		end                                   
		5'h0d:begin                            
					psk32_symbol_re_out <= 16'sh0000;
					psk32_symbol_im_out <= 16'sh1000;
					psk32_R_reg <= psk32_R3_reg;    
		end                                   
		5'h0e:begin                            
					psk32_symbol_re_out <= 16'shf138;
					psk32_symbol_im_out <= 16'shf9e1;
					psk32_R_reg <= psk32_R3_reg;    
		end                                   
		5'h0f:begin                            
					psk32_symbol_re_out <= 16'shf9e1;
					psk32_symbol_im_out <= 16'shf138;
					psk32_R_reg <= psk32_R1_reg;    
		end                                   
		5'h10:begin                            
					psk32_symbol_re_out <= 16'sh0f74;
					psk32_symbol_im_out <= 16'sh0424;
					psk32_R_reg <= psk32_R2_reg;    
		end                                   
		5'h11:begin                            
					psk32_symbol_re_out <= 16'sh0b50;
					psk32_symbol_im_out <= 16'sh0b50;
					psk32_R_reg <= psk32_R1_reg;    
		end                                   
		5'h12:begin                            
					psk32_symbol_re_out <= 16'sh0f74;
					psk32_symbol_im_out <= 16'shfbdc;
					psk32_R_reg <= psk32_R2_reg;    
		end                                   
		5'h13:begin                            
					psk32_symbol_re_out <= 16'sh0b50;
					psk32_symbol_im_out <= 16'shf4b0;
					psk32_R_reg <= psk32_R1_reg;    
		end                                   
		5'h14:begin                            
					psk32_symbol_re_out <= 16'shf08c;
					psk32_symbol_im_out <= 16'sh0424;
					psk32_R_reg <= psk32_R2_reg;    
		end                                   
		5'h15:begin                            
					psk32_symbol_re_out <= 16'shf4b0;
					psk32_symbol_im_out <= 16'sh0b50;
					psk32_R_reg <= psk32_R1_reg;    
		end                                   
		5'h16:begin                            
					psk32_symbol_re_out <= 16'shf08c;
					psk32_symbol_im_out <= 16'shfbdc;
					psk32_R_reg <= psk32_R2_reg;    
		end                                   
		5'h17:begin                            
					psk32_symbol_re_out <= 16'shf4b0;
					psk32_symbol_im_out <= 16'shf4b0;
					psk32_R_reg <= psk32_R1_reg;    
		end                                   
		5'h18:begin                            
					psk32_symbol_re_out <= 16'sh1000;
					psk32_symbol_im_out <= 16'sh0000;
					psk32_R_reg <= psk32_R3_reg;    
		end                                   
		5'h19:begin                            
					psk32_symbol_re_out <= 16'sh0b50;
					psk32_symbol_im_out <= 16'sh0b50;
					psk32_R_reg <= psk32_R3_reg;    
		end                                   
		5'h1a:begin                            
					psk32_symbol_re_out <= 16'sh0ec8;
					psk32_symbol_im_out <= 16'shf9e1;
					psk32_R_reg <= psk32_R3_reg;    
		end                                   
		5'h1b:begin                            
					psk32_symbol_re_out <= 16'sh061f;
					psk32_symbol_im_out <= 16'shf138;
					psk32_R_reg <= psk32_R3_reg;    
		end                                   
		5'h1c:begin                            
					psk32_symbol_re_out <= 16'shf138;
					psk32_symbol_im_out <= 16'sh061f;
					psk32_R_reg <= psk32_R3_reg;    
		end                                   
		5'h1d:begin                            
					psk32_symbol_re_out <= 16'shf9e1;
					psk32_symbol_im_out <= 16'sh0ec8;
					psk32_R_reg <= psk32_R3_reg;    
		end                                   
		5'h1e:begin                            
					psk32_symbol_re_out <= 16'shf000;
					psk32_symbol_im_out <= 16'sh0000;
					psk32_R_reg <= psk32_R1_reg;    
		end                                   
		5'h1f:begin                            
					psk32_symbol_re_out <= 16'shf4b0;
					psk32_symbol_im_out <= 16'shf4b0;
					psk32_R_reg <= psk32_R1_reg;    
		end        
		default:begin
				psk32_symbol_re_out <= 16'sh0;
				psk32_symbol_im_out <= 16'sh0;
				psk32_R_reg <= 16'sh0;
		end
		endcase
	end
	else begin
	end
end


reg  			signed		[31:0]     		psk16_re_mult_out;
reg  			signed		[31:0]     		psk16_im_mult_out;


always @(posedge sys_clk)begin
	if(~rst_n)begin
		psk16_re_mult_out <= 32'sh0;
		psk16_im_mult_out <= 32'sh0;
	end
	else if(fs_en == 1'b1)begin
		psk16_re_mult_out <= psk16_R_reg*psk16_symbol_re_out;
		psk16_im_mult_out <= psk16_R_reg*psk16_symbol_im_out;
	end
	else begin
	end
end

reg  			signed		[15:0]     		qpsk_symbol_re_out_1dly;
reg  			signed		[15:0]     		qpsk_symbol_im_out_1dly;
reg  			signed		[15:0]     		psk8_symbol_re_out_1dly;
reg  			signed		[15:0]     		psk8_symbol_im_out_1dly;


always @(posedge sys_clk)begin
	if(~rst_n)begin
		qpsk_symbol_re_out_1dly <= 16'sh0;
		qpsk_symbol_im_out_1dly <= 16'sh0;
		psk8_symbol_re_out_1dly <= 16'sh0;
		psk8_symbol_im_out_1dly <= 16'sh0;
	end
	else if(fs_en == 1'b1)begin
		qpsk_symbol_re_out_1dly <= qpsk_symbol_re_out;
		qpsk_symbol_im_out_1dly <= qpsk_symbol_im_out;
		psk8_symbol_re_out_1dly <= psk8_symbol_re_out;
		psk8_symbol_im_out_1dly <= psk8_symbol_im_out;
	end
	else begin
	end
end


reg  			signed		[31:0]     		psk32_re_mult_out;
reg  			signed		[31:0]     		psk32_im_mult_out;


always @(posedge sys_clk)begin
	if(~rst_n)begin
		psk32_re_mult_out <= 32'sh0;
		psk32_im_mult_out <= 32'sh0;
	end
	else if(fs_en == 1'b1)begin
		psk32_re_mult_out <= psk32_R_reg*psk32_symbol_re_out;
		psk32_im_mult_out <= psk32_R_reg*psk32_symbol_im_out;
	end
	else begin
	end
end


always @(posedge sys_clk)begin
	if(~rst_n)begin
		symbol_re_out <= 16'sh0;
		symbol_im_out <= 16'sh0;
	end
	else if(fs_en == 1'b1)begin
		case (mod_mode)
			2'h0:begin
				symbol_re_out <= qpsk_symbol_re_out_1dly;
				symbol_im_out <= qpsk_symbol_im_out_1dly;
			end
			2'h1:begin
				symbol_re_out <= psk8_symbol_re_out_1dly;
				symbol_im_out <= psk8_symbol_im_out_1dly;
			end
			2'h2:begin
				symbol_re_out <= $signed({psk16_re_mult_out[31],psk16_re_mult_out[26:12]});
				symbol_im_out <= $signed({psk16_im_mult_out[31],psk16_im_mult_out[26:12]});
			end
			2'h3:begin
				symbol_re_out <= $signed({psk32_re_mult_out[31],psk32_re_mult_out[26:12]});
				symbol_im_out <= $signed({psk32_im_mult_out[31],psk32_im_mult_out[26:12]});
			end
			default:begin
				symbol_re_out <= 16'sh0;
				symbol_im_out <= 16'sh0;
			end
		endcase
	end
	else begin
	end
end


reg											frame_vld_1dly_reg;
reg											frame_vld_2dly_reg;

always @(posedge sys_clk)begin
	if(~rst_n)begin
		frame_vld_1dly_reg <= 1'b0;
		frame_vld_2dly_reg <= 1'b0;
	end
	else if(fs_en == 1'b1)begin
		frame_vld_1dly_reg <= frame_vld;
		frame_vld_2dly_reg <= frame_vld_1dly_reg;
	end
	else begin
	end
end


always @(posedge sys_clk)begin
	if(~rst_n)begin
		oe <= 1'b0;
	end
	else if(fs_en == 1'b1)begin
		oe <= frame_vld_2dly_reg;
	end
	else begin
	end
end

endmodule



