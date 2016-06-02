`timescale 1 ns / 1 ps

module iic_slave_ram_wrap #
	(
		// Users to add parameters here

		// User parameters ends
		// Do not modify the parameters beyond this line


		// Parameters of iic-slave
		// i2c device address
		parameter integer I2C_ADDRESS = 32'h3c,

		// Debounce SCL and SDA over this many clock ticks
		// The rise time of SCL and SDA can be up to 1000nS (in standard mode)
		// so it is essential to debounce the inputs.
		// The spec requires 0.05V of hysteresis, but in practise
		// simply debouncing the inputs is sufficient
		// I2C spec requires suppresion of spikes of 
		// maximum duration 50nS, so this debounce time should be greater than 50nS
		// Also increases data hold time and decreases data setup time
		// during an I2C read operation
		// 10 ticks = 208nS @ 48MHz
		parameter integer DEB_I2C_LEN  = 10,

		// Delay SCL for use as internal sampling clock
		// Using delayed version of SCL to ensure that 
		// SDA is stable when it is sampled.
		// Not entirely citical, as according to I2C spec
		// SDA should have a minimum of 100nS of set up time
		// with respect to SCL rising edge. But with the very slow edge 
		// speeds used in I2C it is better to err on the side of caution.
		// This delay also has the effect of adding extra hold time to the data
		// with respect to SCL falling edge. I2C spec requires 0nS of data hold time.
		// 10 ticks = 208nS @ 48MHz
		parameter integer SCL_DEL_LEN = 10,

		// Delay SDA for use in start/stop detection
		// Use delayed SDA during start/stop detection to avoid
		// incorrect detection at SCL falling edge.
		// From I2C spec start/stop setup is 600nS with respect to SCL rising edge
		// and start/stop hold is 600nS wrt SCL falling edge.
		// So it is relatively easy to discriminate start/stop,
		// but data setup time is a minimum of 100nS with respect to SCL rising edge
		// and 0nS hold wrt to SCL falling edge.
		// So the tricky part is providing robust start/stop detection
		// in the presence of regular data transitions.
		// This delay time should be less than 100nS
		// 4 ticks = 83nS @ 48MHz
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

			.ADDR_LSB(ADDR_LSB),
			.OPT_MEM_ADDR_BITS(OPT_MEM_ADDR_BITS),

			.IIC_SLAVE_DATA_WIDTH(IIC_SLAVE_DATA_WIDTH),
			.IIC_SLAVE_ADDR_WIDTH(IIC_SLAVE_ADDR_WIDTH)
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
