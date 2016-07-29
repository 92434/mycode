
`timescale 1ns / 1ps
module cyclic_shift #(parameter IND = 1,//Dwidth of channel
					  parameter CHD = 360)//Channel of data
					  (
input	          									sys_clk,
input													fs_en,
input	          									rst_n,
//////////////////////////////////////////////////////////////
//input	          									shift_vld,
input	          		[8:0]						shift_index,// cyclic shift derication is up
input	          		[CHD - 1:0]				din,// MSB is the first Bit

//output	  reg     								shift_oe,
output	  reg     		[CHD*IND - 1:0]	shift_out
);
reg						[(CHD + 255)*IND - 1:0] 	s1;
reg						[(CHD + 127)*IND - 1:0] 	s2;
reg						[(CHD +  63)*IND - 1:0] 	s3;
reg						[(CHD +  31)*IND - 1:0] 	s4;
reg						[(CHD +  15)*IND - 1:0] 	s5;
reg						[(CHD +   7)*IND - 1:0] 	s6;
reg						[(CHD +   3)*IND - 1:0] 	s7;
reg						[(CHD +   1)*IND - 1:0] 	s8;
reg						[(CHD +   0)*IND - 1:0] 	s9;

wire					[CHD*3*IND - 1:0]			din_buff;

assign	din_buff = {din,din,din};

always @(*)begin
	if(shift_index[8] == 1'b1)begin
		s1[(CHD + 255)*IND - 1:0] = din_buff[((CHD*3 - 256)*IND - 1):(CHD*3 - (CHD + 511))*IND];
	end
	else begin
		s1[(CHD + 255)*IND - 1:0] = din_buff[(CHD*3*IND - 1):(CHD*3 - (CHD + 255))*IND];
	end
end

always @(*)begin
	if(shift_index[7] == 1'b0)begin
		s2[(CHD + 127)*IND - 1:0] = s1[((CHD + 255)*IND - 1):128*IND];
	end
	else begin
		s2[(CHD + 127)*IND - 1:0] = s1[((CHD + 127)*IND - 1):0*IND];
	end
end

always @(*)begin
	if(shift_index[6] == 1'b0)begin
		s3[(CHD + 63)*IND - 1:0] = s2[((CHD + 127)*IND - 1):64*IND];
	end
	else begin
		s3[(CHD + 63)*IND - 1:0] = s2[((CHD + 63)*IND - 1):0*IND];
	end
end

always @(*)begin
	if(shift_index[5] == 1'b0)begin
		s4[(CHD + 31)*IND - 1:0] = s3[((CHD + 63)*IND - 1):32*IND];
	end
	else begin
		s4[(CHD + 31)*IND - 1:0] = s3[((CHD + 31)*IND - 1):0*IND];
	end
end

always @(*)begin
	if(shift_index[4] == 1'b0)begin
		s5[(CHD + 15)*IND - 1:0] = s4[((CHD + 31)*IND - 1):16*IND];
	end
	else begin
		s5[(CHD + 15)*IND - 1:0] = s4[((CHD + 15)*IND - 1):0*IND];
	end
end

always @(*)begin
	if(shift_index[3] == 1'b0)begin
		s6[(CHD + 7)*IND - 1:0] = s5[((CHD + 15)*IND - 1):8*IND];
	end
	else begin
		s6[(CHD + 7)*IND - 1:0] = s5[((CHD + 7)*IND - 1):0*IND];
	end
end

always @(*)begin
	if(shift_index[2] == 1'b0)begin
		s7[(CHD + 3)*IND - 1:0] = s6[((CHD + 7)*IND - 1):4*IND];
	end
	else begin
		s7[(CHD + 3)*IND - 1:0] = s6[((CHD + 3)*IND - 1):0*IND];
	end
end

always @(*)begin
	if(shift_index[1] == 1'b0)begin
		s8[(CHD + 1)*IND - 1:0] = s7[((CHD + 3)*IND - 1):2*IND];
	end
	else begin
		s8[(CHD + 1)*IND - 1:0] = s7[((CHD + 1)*IND - 1):0*IND];
	end
end

always @(*)begin
	if(shift_index[0] == 1'b0)begin
		s9[(CHD + 0)*IND - 1:0] = s8[((CHD + 1)*IND - 1):1*IND];
	end
	else begin
		s9[(CHD + 0)*IND - 1:0] = s8[((CHD + 0)*IND - 1):0*IND];
	end
end

always @(posedge sys_clk)begin
	if(~rst_n)begin
		//shift_oe <= 1'b0;
		shift_out <= 360'h0;
	end
	else if(fs_en == 1'b1)begin//if(shift_vld)begin////????
		//shift_oe <= 1'b1;
		shift_out <= s9;
	end
	else begin
	end
	//else begin
	//	shift_oe <= 1'b0;
	//end
end

endmodule
