`timescale 1 ns / 1 ps

module iic_slave_ram_wrap #
	(
		// Users to add parameters here

		// User parameters ends
		// Do not modify the parameters beyond this line

		parameter integer I2C_ADDRESS = 32'h3c,
		parameter integer DEB_I2C_LEN  = 10,
		parameter integer SCL_DEL_LEN = 10,
		parameter integer SDA_DEL_LEN = 4,

		parameter integer IIC_SLAVE_DATA_WIDTH = 8,
		parameter integer IIC_SLAVE_ADDR_WIDTH = 8
	)
	(
		// Users to add ports here

		// User ports ends
		// Do not modify the ports beyond this line

		// Ports of iic-slave
		input clk,
		input rst_n,
		inout sda,
		input scl
	);

	function integer clogb2(input integer bit_depth);
		begin
			for(clogb2=0; bit_depth>0; clogb2=clogb2+1)
				bit_depth = bit_depth >> 1;
		end
	endfunction

	localparam integer ADDR_LSB = clogb2((IIC_SLAVE_DATA_WIDTH / 8) - 1);

	localparam integer OPT_MEM_ADDR_BITS = IIC_SLAVE_ADDR_WIDTH - ADDR_LSB;

	wire wen;
	wire [IIC_SLAVE_DATA_WIDTH - 1 : 0] wdata;
	wire [OPT_MEM_ADDR_BITS - 1 : 0] waddr;

	wire ren;
	wire [IIC_SLAVE_DATA_WIDTH - 1 : 0] rdata;
	wire [OPT_MEM_ADDR_BITS - 1 : 0] raddr;

	// Instantiation of Axi Bus Interface S00_AXI
	iic_slave # ( 
			.I2C_ADDRESS(I2C_ADDRESS),
			.DEB_I2C_LEN(DEB_I2C_LEN),
			.SCL_DEL_LEN(SCL_DEL_LEN),
			.SDA_DEL_LEN(SDA_DEL_LEN),

			.OPT_MEM_ADDR_BITS(OPT_MEM_ADDR_BITS),

			.IIC_SLAVE_DATA_WIDTH(IIC_SLAVE_DATA_WIDTH)
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
			.IIC_SLAVE_DATA_WIDTH(IIC_SLAVE_DATA_WIDTH),
			.OPT_MEM_ADDR_BITS(OPT_MEM_ADDR_BITS)
		) iic_slave_ram_inst(
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
