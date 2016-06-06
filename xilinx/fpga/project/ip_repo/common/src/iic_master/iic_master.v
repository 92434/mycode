`timescale 1ns / 1ps
`include "iic_def.v"

module iic_master #
	(
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

		parameter integer OPT_MEM_ADDR_BITS = 8
	)
	(
		// Users to add ports here
		output wire wen,
		output wire [8 - 1 : 0] wdata,
		output wire [OPT_MEM_ADDR_BITS - 1 : 0] waddr,

		output wire ren,
		input wire [8 - 1 : 0] rdata,
		output wire [OPT_MEM_ADDR_BITS - 1 : 0] raddr,

		// User ports ends
		// Do not modify the ports beyond this line


		// Ports of iic-slave
		input wire clk,
		input wire rst_n,

		inout wire sda,
		output reg scl
	);

	// local wires and regs
	wire sda_in;
	wire sda_out_en;
	wire sda_out;

	assign sda = (sda_out_en == 1) ? sda_out : 1'bz;
	assign sda_in = sda;


	// debounce sda
	reg [DEB_I2C_LEN - 1 : 0] sda_pipe;
	reg sda_deb;
	always @(posedge clk) begin
		if(rst_n == 1'b0) begin
			sda_pipe <= {DEB_I2C_LEN{1'b1}};
			sda_deb <= 1'b1;
		end
		else begin
			if(&sda_pipe[DEB_I2C_LEN - 1 : 1] == 1'b1) begin
				sda_deb <= 1'b1;
			end
			else if(|sda_pipe[DEB_I2C_LEN - 1 : 1] == 1'b0) begin
				sda_deb <= 1'b0;
			end

			sda_pipe <= {sda_pipe[DEB_I2C_LEN - 2 : 0], sda_in};
		end
	end

	// delay scl
	// scl_delayed is used as a delayed sampling clock
	// sda_delayed is only used for start stop detection
	// Because sda hold time from scl falling is 0nS
	// sda must be delayed with respect to scl to avoid incorrect
	// detection of start/stop at scl falling edge. 
	reg [SCL_DEL_LEN - 1 : 0] scl_delayed;
	always @(posedge clk) begin
		if(rst_n_sync_to_clk == 1'b0) begin
			scl_delayed <= {SCL_DEL_LEN{1'b1}};
		end
		else begin
			scl_delayed <= {scl_delayed[SCL_DEL_LEN - 2 : 0], scl};
		end
	end


	iic_slave_interface #
		(
		)
		(
			.clk(clk),
			.rst_n(rst_n),

			.scl_out(scl),
			.scl_out_delay(scl_out_delay),

			.sda_in(sda_in),
			.sda_out_en(sda_out_en),
			.sda_out(sda_out),

			.slave_addr(slave_addr),

			.fifo_wen(fifo_wen),
			.fifo_wdata(fifo_wdata),

			.fifo_ren(fifo_ren),
			.fifo_rdata(fifo_rdata),

			.start(start),

			.stop_request(stop_request),
			.status(status),
			.stop(stop)
		);
endmodule
