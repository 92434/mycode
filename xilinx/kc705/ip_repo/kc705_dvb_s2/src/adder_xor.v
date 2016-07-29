`timescale 1ns / 1ps
module adder_xor(
input	          							sys_clk,
input											fs_en,
input	          							rst_n,
//////////////////////////////////////////////////////////////
input	          			[359:0] 		data_in,
input	          			[359:0] 		ram_rd_dataout,
input	          			[359:0] 		ram_wr_datain,
input	          							equal_flag,
//////////////////////////////////////////////////////////////

output      	   		[359:0] 		adder_out
);

reg							[359:0]     ram_rd_data_get;
reg							[359:0]     ram_rd_data_get_1dly;

always @(posedge sys_clk)begin
	if(~rst_n)begin
		ram_rd_data_get <= 360'h00;
	end
	else if(fs_en == 1'b1)begin
		if(equal_flag)begin
			ram_rd_data_get <= ram_wr_datain;
		end
	end
	else begin
	end
end

always @(posedge sys_clk)begin
	if(~rst_n)begin
		ram_rd_data_get_1dly <= 360'h00;
	end
	else if(fs_en == 1'b1)begin
		ram_rd_data_get_1dly <= ram_rd_data_get;
	end
	else begin
	end
end

reg	          							equal_flag_1dly;
reg	          							equal_flag_2dly;

always @(posedge sys_clk)begin
	if(~rst_n)begin
		equal_flag_1dly <= 1'b0;
		equal_flag_2dly <= 1'b0;
	end
	else if(fs_en == 1'b1)begin
		equal_flag_1dly <= equal_flag;
		equal_flag_2dly <= equal_flag_1dly;
	end
	else begin
	end
end


wire							[359:0]     ram_rd_data;

assign	ram_rd_data = (equal_flag_2dly == 1'b1)?ram_rd_data_get_1dly:ram_rd_dataout;

assign adder_out = data_in^ram_rd_data;//{data_in[359]^ram_rd_data[359],data_in[358]^ram_rd_data[358],data_in[357]^ram_rd_data[357],data_in[356]^ram_rd_data[356],data_in[355]^ram_rd_data[355],data_in[354]^ram_rd_data[354],data_in[353]^ram_rd_data[353],data_in[352]^ram_rd_data[352],data_in[351]^ram_rd_data[351],data_in[350]^ram_rd_data[350],data_in[349]^ram_rd_data[349],data_in[348]^ram_rd_data[348],data_in[347]^ram_rd_data[347],data_in[346]^ram_rd_data[346],data_in[345]^ram_rd_data[345],data_in[344]^ram_rd_data[344],data_in[343]^ram_rd_data[343],data_in[342]^ram_rd_data[342],data_in[341]^ram_rd_data[341],data_in[340]^ram_rd_data[340],data_in[339]^ram_rd_data[339],data_in[338]^ram_rd_data[338],data_in[337]^ram_rd_data[337],data_in[336]^ram_rd_data[336],data_in[335]^ram_rd_data[335],data_in[334]^ram_rd_data[334],data_in[333]^ram_rd_data[333],data_in[332]^ram_rd_data[332],data_in[331]^ram_rd_data[331],data_in[330]^ram_rd_data[330],data_in[329]^ram_rd_data[329],data_in[328]^ram_rd_data[328],data_in[327]^ram_rd_data[327],data_in[326]^ram_rd_data[326],data_in[325]^ram_rd_data[325],data_in[324]^ram_rd_data[324],data_in[323]^ram_rd_data[323],data_in[322]^ram_rd_data[322],data_in[321]^ram_rd_data[321],data_in[320]^ram_rd_data[320],data_in[319]^ram_rd_data[319],data_in[318]^ram_rd_data[318],data_in[317]^ram_rd_data[317],data_in[316]^ram_rd_data[316],data_in[315]^ram_rd_data[315],data_in[314]^ram_rd_data[314],data_in[313]^ram_rd_data[313],data_in[312]^ram_rd_data[312],data_in[311]^ram_rd_data[311],data_in[310]^ram_rd_data[310],data_in[309]^ram_rd_data[309],data_in[308]^ram_rd_data[308],data_in[307]^ram_rd_data[307],data_in[306]^ram_rd_data[306],data_in[305]^ram_rd_data[305],data_in[304]^ram_rd_data[304],data_in[303]^ram_rd_data[303],data_in[302]^ram_rd_data[302],data_in[301]^ram_rd_data[301],data_in[300]^ram_rd_data[300],data_in[299]^ram_rd_data[299],data_in[298]^ram_rd_data[298],data_in[297]^ram_rd_data[297],data_in[296]^ram_rd_data[296],data_in[295]^ram_rd_data[295],data_in[294]^ram_rd_data[294],data_in[293]^ram_rd_data[293],data_in[292]^ram_rd_data[292],data_in[291]^ram_rd_data[291],data_in[290]^ram_rd_data[290],data_in[289]^ram_rd_data[289],data_in[288]^ram_rd_data[288],data_in[287]^ram_rd_data[287],data_in[286]^ram_rd_data[286],data_in[285]^ram_rd_data[285],data_in[284]^ram_rd_data[284],data_in[283]^ram_rd_data[283],data_in[282]^ram_rd_data[282],data_in[281]^ram_rd_data[281],data_in[280]^ram_rd_data[280],data_in[279]^ram_rd_data[279],data_in[278]^ram_rd_data[278],data_in[277]^ram_rd_data[277],data_in[276]^ram_rd_data[276],data_in[275]^ram_rd_data[275],data_in[274]^ram_rd_data[274],data_in[273]^ram_rd_data[273],data_in[272]^ram_rd_data[272],data_in[271]^ram_rd_data[271],data_in[270]^ram_rd_data[270],data_in[269]^ram_rd_data[269],data_in[268]^ram_rd_data[268],data_in[267]^ram_rd_data[267],data_in[266]^ram_rd_data[266],data_in[265]^ram_rd_data[265],data_in[264]^ram_rd_data[264],data_in[263]^ram_rd_data[263],data_in[262]^ram_rd_data[262],data_in[261]^ram_rd_data[261],data_in[260]^ram_rd_data[260],data_in[259]^ram_rd_data[259],data_in[258]^ram_rd_data[258],data_in[257]^ram_rd_data[257],data_in[256]^ram_rd_data[256],data_in[255]^ram_rd_data[255],data_in[254]^ram_rd_data[254],data_in[253]^ram_rd_data[253],data_in[252]^ram_rd_data[252],data_in[251]^ram_rd_data[251],data_in[250]^ram_rd_data[250],data_in[249]^ram_rd_data[249],data_in[248]^ram_rd_data[248],data_in[247]^ram_rd_data[247],data_in[246]^ram_rd_data[246],data_in[245]^ram_rd_data[245],data_in[244]^ram_rd_data[244],data_in[243]^ram_rd_data[243],data_in[242]^ram_rd_data[242],data_in[241]^ram_rd_data[241],data_in[240]^ram_rd_data[240],data_in[239]^ram_rd_data[239],data_in[238]^ram_rd_data[238],data_in[237]^ram_rd_data[237],data_in[236]^ram_rd_data[236],data_in[235]^ram_rd_data[235],data_in[234]^ram_rd_data[234],data_in[233]^ram_rd_data[233],data_in[232]^ram_rd_data[232],data_in[231]^ram_rd_data[231],data_in[230]^ram_rd_data[230],data_in[229]^ram_rd_data[229],data_in[228]^ram_rd_data[228],data_in[227]^ram_rd_data[227],data_in[226]^ram_rd_data[226],data_in[225]^ram_rd_data[225],data_in[224]^ram_rd_data[224],data_in[223]^ram_rd_data[223],data_in[222]^ram_rd_data[222],data_in[221]^ram_rd_data[221],data_in[220]^ram_rd_data[220],data_in[219]^ram_rd_data[219],data_in[218]^ram_rd_data[218],data_in[217]^ram_rd_data[217],data_in[216]^ram_rd_data[216],data_in[215]^ram_rd_data[215],data_in[214]^ram_rd_data[214],data_in[213]^ram_rd_data[213],data_in[212]^ram_rd_data[212],data_in[211]^ram_rd_data[211],data_in[210]^ram_rd_data[210],data_in[209]^ram_rd_data[209],data_in[208]^ram_rd_data[208],data_in[207]^ram_rd_data[207],data_in[206]^ram_rd_data[206],data_in[205]^ram_rd_data[205],data_in[204]^ram_rd_data[204],data_in[203]^ram_rd_data[203],data_in[202]^ram_rd_data[202],data_in[201]^ram_rd_data[201],data_in[200]^ram_rd_data[200],data_in[199]^ram_rd_data[199],data_in[198]^ram_rd_data[198],data_in[197]^ram_rd_data[197],data_in[196]^ram_rd_data[196],data_in[195]^ram_rd_data[195],data_in[194]^ram_rd_data[194],data_in[193]^ram_rd_data[193],data_in[192]^ram_rd_data[192],data_in[191]^ram_rd_data[191],data_in[190]^ram_rd_data[190],data_in[189]^ram_rd_data[189],data_in[188]^ram_rd_data[188],data_in[187]^ram_rd_data[187],data_in[186]^ram_rd_data[186],data_in[185]^ram_rd_data[185],data_in[184]^ram_rd_data[184],data_in[183]^ram_rd_data[183],data_in[182]^ram_rd_data[182],data_in[181]^ram_rd_data[181],data_in[180]^ram_rd_data[180],data_in[179]^ram_rd_data[179],data_in[178]^ram_rd_data[178],data_in[177]^ram_rd_data[177],data_in[176]^ram_rd_data[176],data_in[175]^ram_rd_data[175],data_in[174]^ram_rd_data[174],data_in[173]^ram_rd_data[173],data_in[172]^ram_rd_data[172],data_in[171]^ram_rd_data[171],data_in[170]^ram_rd_data[170],data_in[169]^ram_rd_data[169],data_in[168]^ram_rd_data[168],data_in[167]^ram_rd_data[167],data_in[166]^ram_rd_data[166],data_in[165]^ram_rd_data[165],data_in[164]^ram_rd_data[164],data_in[163]^ram_rd_data[163],data_in[162]^ram_rd_data[162],data_in[161]^ram_rd_data[161],data_in[160]^ram_rd_data[160],data_in[159]^ram_rd_data[159],data_in[158]^ram_rd_data[158],data_in[157]^ram_rd_data[157],data_in[156]^ram_rd_data[156],data_in[155]^ram_rd_data[155],data_in[154]^ram_rd_data[154],data_in[153]^ram_rd_data[153],data_in[152]^ram_rd_data[152],data_in[151]^ram_rd_data[151],data_in[150]^ram_rd_data[150],data_in[149]^ram_rd_data[149],data_in[148]^ram_rd_data[148],data_in[147]^ram_rd_data[147],data_in[146]^ram_rd_data[146],data_in[145]^ram_rd_data[145],data_in[144]^ram_rd_data[144],data_in[143]^ram_rd_data[143],data_in[142]^ram_rd_data[142],data_in[141]^ram_rd_data[141],data_in[140]^ram_rd_data[140],data_in[139]^ram_rd_data[139],data_in[138]^ram_rd_data[138],data_in[137]^ram_rd_data[137],data_in[136]^ram_rd_data[136],data_in[135]^ram_rd_data[135],data_in[134]^ram_rd_data[134],data_in[133]^ram_rd_data[133],data_in[132]^ram_rd_data[132],data_in[131]^ram_rd_data[131],data_in[130]^ram_rd_data[130],data_in[129]^ram_rd_data[129],data_in[128]^ram_rd_data[128],data_in[127]^ram_rd_data[127],data_in[126]^ram_rd_data[126],data_in[125]^ram_rd_data[125],data_in[124]^ram_rd_data[124],data_in[123]^ram_rd_data[123],data_in[122]^ram_rd_data[122],data_in[121]^ram_rd_data[121],data_in[120]^ram_rd_data[120],data_in[119]^ram_rd_data[119],data_in[118]^ram_rd_data[118],data_in[117]^ram_rd_data[117],data_in[116]^ram_rd_data[116],data_in[115]^ram_rd_data[115],data_in[114]^ram_rd_data[114],data_in[113]^ram_rd_data[113],data_in[112]^ram_rd_data[112],data_in[111]^ram_rd_data[111],data_in[110]^ram_rd_data[110],data_in[109]^ram_rd_data[109],data_in[108]^ram_rd_data[108],data_in[107]^ram_rd_data[107],data_in[106]^ram_rd_data[106],data_in[105]^ram_rd_data[105],data_in[104]^ram_rd_data[104],data_in[103]^ram_rd_data[103],data_in[102]^ram_rd_data[102],data_in[101]^ram_rd_data[101],data_in[100]^ram_rd_data[100],data_in[99]^ram_rd_data[99],data_in[98]^ram_rd_data[98],data_in[97]^ram_rd_data[97],data_in[96]^ram_rd_data[96],data_in[95]^ram_rd_data[95],data_in[94]^ram_rd_data[94],data_in[93]^ram_rd_data[93],data_in[92]^ram_rd_data[92],data_in[91]^ram_rd_data[91],data_in[90]^ram_rd_data[90],data_in[89]^ram_rd_data[89],data_in[88]^ram_rd_data[88],data_in[87]^ram_rd_data[87],data_in[86]^ram_rd_data[86],data_in[85]^ram_rd_data[85],data_in[84]^ram_rd_data[84],data_in[83]^ram_rd_data[83],data_in[82]^ram_rd_data[82],data_in[81]^ram_rd_data[81],data_in[80]^ram_rd_data[80],data_in[79]^ram_rd_data[79],data_in[78]^ram_rd_data[78],data_in[77]^ram_rd_data[77],data_in[76]^ram_rd_data[76],data_in[75]^ram_rd_data[75],data_in[74]^ram_rd_data[74],data_in[73]^ram_rd_data[73],data_in[72]^ram_rd_data[72],data_in[71]^ram_rd_data[71],data_in[70]^ram_rd_data[70],data_in[69]^ram_rd_data[69],data_in[68]^ram_rd_data[68],data_in[67]^ram_rd_data[67],data_in[66]^ram_rd_data[66],data_in[65]^ram_rd_data[65],data_in[64]^ram_rd_data[64],data_in[63]^ram_rd_data[63],data_in[62]^ram_rd_data[62],data_in[61]^ram_rd_data[61],data_in[60]^ram_rd_data[60],data_in[59]^ram_rd_data[59],data_in[58]^ram_rd_data[58],data_in[57]^ram_rd_data[57],data_in[56]^ram_rd_data[56],data_in[55]^ram_rd_data[55],data_in[54]^ram_rd_data[54],data_in[53]^ram_rd_data[53],data_in[52]^ram_rd_data[52],data_in[51]^ram_rd_data[51],data_in[50]^ram_rd_data[50],data_in[49]^ram_rd_data[49],data_in[48]^ram_rd_data[48],data_in[47]^ram_rd_data[47],data_in[46]^ram_rd_data[46],data_in[45]^ram_rd_data[45],data_in[44]^ram_rd_data[44],data_in[43]^ram_rd_data[43],data_in[42]^ram_rd_data[42],data_in[41]^ram_rd_data[41],data_in[40]^ram_rd_data[40],data_in[39]^ram_rd_data[39],data_in[38]^ram_rd_data[38],data_in[37]^ram_rd_data[37],data_in[36]^ram_rd_data[36],data_in[35]^ram_rd_data[35],data_in[34]^ram_rd_data[34],data_in[33]^ram_rd_data[33],data_in[32]^ram_rd_data[32],data_in[31]^ram_rd_data[31],data_in[30]^ram_rd_data[30],data_in[29]^ram_rd_data[29],data_in[28]^ram_rd_data[28],data_in[27]^ram_rd_data[27],data_in[26]^ram_rd_data[26],data_in[25]^ram_rd_data[25],data_in[24]^ram_rd_data[24],data_in[23]^ram_rd_data[23],data_in[22]^ram_rd_data[22],data_in[21]^ram_rd_data[21],data_in[20]^ram_rd_data[20],data_in[19]^ram_rd_data[19],data_in[18]^ram_rd_data[18],data_in[17]^ram_rd_data[17],data_in[16]^ram_rd_data[16],data_in[15]^ram_rd_data[15],data_in[14]^ram_rd_data[14],data_in[13]^ram_rd_data[13],data_in[12]^ram_rd_data[12],data_in[11]^ram_rd_data[11],data_in[10]^ram_rd_data[10],data_in[9]^ram_rd_data[9],data_in[8]^ram_rd_data[8],data_in[7]^ram_rd_data[7],data_in[6]^ram_rd_data[6],data_in[5]^ram_rd_data[5],data_in[4]^ram_rd_data[4],data_in[3]^ram_rd_data[3],data_in[2]^ram_rd_data[2],data_in[1]^ram_rd_data[1],data_in[0]^ram_rd_data[0]};

endmodule
