module  create_null
			(
							sys_clk							,
							glb_rst_n						,
							byte_en							,
							dout								,
							wren						
			);
input						sys_clk							;
input						glb_rst_n						;
input						byte_en							;
output[8:0]				dout								;
output					wren								;


reg[8:0]					dout								;
reg						wren								;
reg[7:0]					byte_cnt							;

reg[3:0]					null_idx							;

always@(posedge sys_clk)begin
	if(~glb_rst_n)
		begin
			dout<=9'h000;
			wren<=1'b0;
			byte_cnt<=8'd0;
			null_idx<=4'h0;
		end
	else	if(byte_en)
		begin
			wren<=1'b1;
			if(byte_cnt==8'd187)
				byte_cnt<=8'd0;
			else
				byte_cnt<=byte_cnt+8'd1;
			if(byte_cnt==8'd0)
				begin
					dout<=9'h147;
				end
			else if(byte_cnt==8'd1)
				begin
					dout<=9'h01f;
				end
			else if(byte_cnt==8'd2)
				begin
					dout<=9'h0ff;
				end
			else if(byte_cnt==8'd3)
				begin
					dout<={1'b0,4'h1,null_idx};
					null_idx <= null_idx + 1;
				end
			else 
				begin
					dout<={1'b0,8'hff};
				end
		
		end
	else	
		begin
			wren<=1'b0;
			dout<=dout;
		end
end

endmodule
