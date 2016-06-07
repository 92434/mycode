`timescale 1 ns / 1 ps

module tb #
	(
		// Users to add parameters here

		// User parameters ends
		// Do not modify the parameters beyond this line

	)
	(
		// Users to add ports here

		// User ports ends
		// Do not modify the ports beyond this line

		// Ports of iic-slave
	);

	wire clk;
	clkgen #(.clk_period(1)) xiaofeiclk1(.clk(clk));

	reg rst_n = 1;
	initial begin
		#2;
		rst_n = 0;
		#10;
		rst_n = 1;
	end

	wire sda;
	wire scl;

	localparam integer DATA_M_LENGTH = 1 << 8;
	reg [7 : 0] data_m [0 : DATA_M_LENGTH - 1];
	reg [7 : 0] read_count = 0;
	reg [7 : 0] write_count = 0;
	reg [7 : 0] offset = 0;
	reg [7 : 0] slave_addr = 8'h78;//011 1100 0
	reg request = 0;
	integer state_m = 0;
	wire iic_ready;
	always @(posedge clk) begin
		if(rst_n == 0) begin
			read_count <= 0;
			write_count <= 0;
			offset <= 0;
			slave_addr <= 8'h78;
			request <= 0;
			state_m <= 0;
		end
		else begin
			request <= 0;

			case(state_m)
				0: begin//write (0x01, 0x02, 0x03, 0x04, 0x05) to addr (0x00 0x00)
					if(iic_ready == 1) begin
						slave_addr[0] <= 0;
						data_m[0] <= 0;
						data_m[1] <= 0;
						data_m[2] <= 1;
						data_m[3] <= 2;
						data_m[4] <= 3;
						data_m[5] <= 4;
						data_m[6] <= 5;
						read_count <= 7;

						state_m <= 1;
					end
					else begin
					end
				end
				1: begin
						request <= 1;

						state_m <= 2;
				end
				2: begin
					if(iic_ready == 1) begin
					end
					else begin
					end
				end
				default: begin
				end
			endcase
		end
	end

	wire ren_m;
	reg [7 : 0] rdata_m = 0;
	wire [7 : 0] raddr_m;

	wire wen_m;
	wire [7 : 0] wdata_m;
	wire [7 : 0] waddr_m;

	always @(posedge clk) begin
		if(rst_n == 0) begin
			rdata_m <= 0;
		end
		else begin
			if(ren_m == 1) begin
				rdata_m <= data_m[raddr_m];
			end
			else begin
			end

			if(wen_m == 1) begin
				data_m[waddr_m] <= wdata_m;
			end
			else begin
			end
		end
	end

	localparam integer DEB_I2C_LEN  = 10;
	localparam integer SCL_DEL_LEN = 10;
	iic_master #
		(
			.DEB_I2C_LEN(DEB_I2C_LEN),
			.SCL_DEL_LEN(SCL_DEL_LEN)
		)
		iic_master_inst (
			.request(request),

			.wcount(write_count),
			.wen(wen_m),
			.wdata(wdata_m),
			.waddr(waddr_m),

			.rcount(read_count),
			.ren(ren_m),
			.rdata(rdata_m),
			.raddr(raddr_m),

			.slave_addr(slave_addr),

			.iic_ready(iic_ready),
			
			.clk(clk),
			.rst_n(rst_n),
			.sda(sda),
			.scl(scl)
		);


	localparam integer I2C_ADDRESS = 32'h3c;
	localparam integer SDA_DEL_LEN = 4;
	localparam integer IIC_SLAVE_ADDR_WIDTH = 16;
	function integer clogb2(input integer bit_depth);
		begin
			for(clogb2=0; bit_depth>0; clogb2=clogb2+1)
				bit_depth = bit_depth >> 1;
		end
	endfunction
	localparam integer ADDR_LSB = clogb2(0);
	localparam integer OPT_MEM_ADDR_BITS = IIC_SLAVE_ADDR_WIDTH - ADDR_LSB;

	wire wen;
	wire [7 : 0] wdata;
	wire [OPT_MEM_ADDR_BITS - 1 : 0] waddr;

	wire ren;
	wire [7 : 0] rdata;
	wire [OPT_MEM_ADDR_BITS - 1 : 0] raddr;

	// Instantiation of Axi Bus Interface S00_AXI
	iic_slave # ( 
			.I2C_ADDRESS(I2C_ADDRESS),
			.DEB_I2C_LEN(DEB_I2C_LEN),
			.SCL_DEL_LEN(SCL_DEL_LEN),
			.SDA_DEL_LEN(SDA_DEL_LEN),

			.OPT_MEM_ADDR_BITS(OPT_MEM_ADDR_BITS)
		) iic_slave_inst (
			.wen(wen),
			.wdata(wdata),
			.waddr(waddr),

			.ren(ren),
			.rdata(rdata),
			.raddr(raddr),

			.clk(clk),
			.rst_n(rst_n),
			.sda(sda),
			.scl(scl)
		);

	iic_slave_ram #(
			.OPT_MEM_ADDR_BITS(OPT_MEM_ADDR_BITS)
		) iic_slave_ram_inst (
			.clk(clk),
			.rst_n(rst_n),

			.wen(wen),
			.wdata(wdata),
			.waddr(waddr),

			.ren(ren),
			.rdata(rdata),
			.raddr(raddr)
		);

	// Add user logic here

	// User logic ends

	endmodule
