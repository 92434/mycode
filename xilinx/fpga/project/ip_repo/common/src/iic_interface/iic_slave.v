`timescale 1ns / 1ps
`include "iic_def.v"

module iic_slave #
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

		parameter integer IIC_OPT_MEM_ADDR_BITS = 8
	)
	(
		// Users to add ports here
		output wire wen,
		output wire [7 : 0] wdata,
		output wire [IIC_OPT_MEM_ADDR_BITS - 1 : 0] waddr,

		output wire ren,
		input wire [7 : 0] rdata,
		output wire [IIC_OPT_MEM_ADDR_BITS - 1 : 0] raddr,

		// User ports ends
		// Do not modify the ports beyond this line


		// Ports of iic-slave
		input wire clk,
		input wire rst_n,

		inout wire sda,
		input scl
	);


	// local wires and regs

	wire start_stop_det_enable;
	wire sda_in;
	wire sda_out_en;
	wire sda_out;

	assign sda = (sda_out_en == 1) ? sda_out : 1'bz;
	assign sda_in = sda;



	reg [1 : 0] rst_n_pipe;
	// sync rst_n rsing edge to clk
	always @(posedge clk) begin
		if(rst_n == 1'b0) begin
			rst_n_pipe <= 2'b00;
		end
		else begin
			rst_n_pipe <= {rst_n_pipe[0], 1'b1};
		end
	end

	wire rst_n_sync_to_clk;

	assign rst_n_sync_to_clk = rst_n_pipe[1];

	// debounce sda and scl
	reg [DEB_I2C_LEN - 1 : 0] sda_pipe;
	reg sda_deb;
	reg [DEB_I2C_LEN - 1 : 0] scl_pipe;
	reg scl_deb;
	always @(posedge clk) begin
		if(rst_n_sync_to_clk == 1'b0) begin
			sda_pipe <= {DEB_I2C_LEN{1'b1}};
			sda_deb <= 1'b1;
			scl_pipe <= {DEB_I2C_LEN{1'b1}};
			scl_deb <= 1'b1;
		end
		else begin
			if(&scl_pipe[DEB_I2C_LEN - 1 : 1] == 1'b1) begin
				scl_deb <= 1'b1;
			end
			else if(|scl_pipe[DEB_I2C_LEN - 1 : 1] == 1'b0) begin
				scl_deb <= 1'b0;
			end

			if(&sda_pipe[DEB_I2C_LEN - 1 : 1] == 1'b1) begin
				sda_deb <= 1'b1;
			end
			else if(|sda_pipe[DEB_I2C_LEN - 1 : 1] == 1'b0) begin
				sda_deb <= 1'b0;
			end

			sda_pipe <= {sda_pipe[DEB_I2C_LEN - 2 : 0], sda_in};
			scl_pipe <= {scl_pipe[DEB_I2C_LEN - 2 : 0], scl};
		end
	end


	// delay scl and sda
	// scl_delayed is used as a delayed sampling clock
	// sda_delayed is only used for start stop detection
	// Because sda hold time from scl falling is 0nS
	// sda must be delayed with respect to scl to avoid incorrect
	// detection of start/stop at scl falling edge. 
	reg [SCL_DEL_LEN - 1 : 0] scl_delayed;
	reg [SDA_DEL_LEN - 1 : 0] sda_delayed;
	always @(posedge clk) begin
		if(rst_n_sync_to_clk == 1'b0) begin
			scl_delayed <= {SCL_DEL_LEN{1'b1}};
			sda_delayed <= {SDA_DEL_LEN{1'b1}};
		end
		else begin
			scl_delayed <= {scl_delayed[SCL_DEL_LEN - 2 : 0], scl_deb};
			sda_delayed <= {sda_delayed[SDA_DEL_LEN - 2 : 0], sda_deb};
		end
	end

	// start stop detection
	wire scl_stay_high;
	wire sda_rise;
	wire sda_fall;
	assign scl_stay_high = (scl_delayed[SDA_DEL_LEN - 1] == 1'b1 && scl_delayed[SDA_DEL_LEN - 2] == 1'b1) ? 1 : 0;
	assign sda_rise = (sda_delayed[SDA_DEL_LEN - 1] == 1'b0 && sda_delayed[SDA_DEL_LEN - 2] == 1'b1) ? 1 : 0;
	assign sda_fall = (sda_delayed[SDA_DEL_LEN - 1] == 1'b1 && sda_delayed[SDA_DEL_LEN - 2] == 1'b0) ? 1 : 0;

	reg [1 : 0] start_stop_state = `NULL_DET;
	always @(posedge clk) begin
		if(rst_n_sync_to_clk == 1'b0) begin
			start_stop_state <= `NULL_DET;
		end
		else begin
			if(start_stop_det_enable == 1) begin
				if(scl_stay_high == 1) begin
					if(sda_rise == 1) begin
						start_stop_state <= `STOP_DET;
					end
					else if(sda_fall == 1) begin
						start_stop_state <= `START_DET;
					end
					else begin
					end
				end
				else begin
				end
			end
			else begin
				start_stop_state <= `NULL_DET;
			end
		end
	end

	// Instantiation of iic-slave
	
	wire fifo_wen;
	wire [7 : 0] fifo_wdata;

	wire fifo_ren;
	wire [7 : 0] fifo_rdata;
	iic_slave_interface #(
			.I2C_ADDRESS(I2C_ADDRESS)
		) iic_slave_interface_inst(
			.clk(clk),
			.rst_n(rst_n_sync_to_clk),

			.start_stop_det_enable(start_stop_det_enable),
			.start_stop_state(start_stop_state),

			.scl_in(scl_delayed[SCL_DEL_LEN - 1]),
			.sda_in(sda_delayed[SDA_DEL_LEN - 1]),
			.sda_out_en(sda_out_en),
			.sda_out(sda_out),

			.fifo_wen(fifo_wen),
			.fifo_wdata(fifo_wdata),

			.fifo_ren(fifo_ren),
			.fifo_rdata(fifo_rdata)
		);
	// Add user logic here
	localparam integer ADDR_BYTES_COUNT = IIC_OPT_MEM_ADDR_BITS / 8;

	reg [IIC_OPT_MEM_ADDR_BITS - 1 : 0] addr = 0;
	reg [7 : 0] addr_bytes_count = 0;
	reg [7 : 0] wdata_bytes_count = 0;
	reg [7 : 0] rdata_bytes_count = 0;
	always @(posedge clk) begin
		if(rst_n_sync_to_clk == 0) begin
			addr <= 0;
			addr_bytes_count <= 0;
			wdata_bytes_count <= 0;
			rdata_bytes_count <= 0;
		end
		else begin
			if(fifo_wen == 1) begin
				if(addr_bytes_count < ADDR_BYTES_COUNT) begin
					addr[(addr_bytes_count * 8) +: 8] <= fifo_wdata[7 : 0];//???????????????????

					addr_bytes_count <= addr_bytes_count + 1;
				end
				wdata_bytes_count <= wdata_bytes_count + 1;
			end
			else if(fifo_ren == 1) begin
				rdata_bytes_count <= rdata_bytes_count + 1;
			end
			else begin
			end
			
			if(start_stop_state == `START_DET) begin
				addr_bytes_count <= 0;
				wdata_bytes_count <= 0;
				rdata_bytes_count <= 0;
			end
			else begin
			end
		end
	end

	assign state_addr = (addr_bytes_count < ADDR_BYTES_COUNT) ? 1 : 0;

	assign wen = (state_addr == 0) ? fifo_wen : 0;
	assign wdata = (state_addr == 0) ? fifo_wdata : 0;
	assign waddr = addr + wdata_bytes_count - addr_bytes_count;

	assign ren = fifo_ren;
	assign fifo_rdata = rdata;
	assign raddr = addr + rdata_bytes_count;

	// User logic ends

	endmodule
