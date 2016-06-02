`include "timescale.v"
`include "i2cSlave_define.v"

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

		parameter integer ADDR_LSB = 0,
		parameter integer OPT_MEM_ADDR_BITS = 8,

		parameter integer IIC_SLAVE_DATA_WIDTH = 8,
		parameter integer IIC_SLAVE_ADDR_WIDTH = 8
	)
	(
		// Users to add ports here
		output wire wen,
		output wire [IIC_SLAVE_DATA_WIDTH - 1 : 0] wdata,
		output wire [OPT_MEM_ADDR_BITS - 1 : 0] waddr,

		output wire ren,
		input wire [IIC_SLAVE_DATA_WIDTH - 1 : 0] rdata,
		output wire [OPT_MEM_ADDR_BITS - 1 : 0] raddr,

		// User ports ends
		// Do not modify the ports beyond this line


		// Ports of iic-slave
		input clk,
		input rst_n,
		inout sda,
		input scl
	);


	// local wires and regs

	wire clearStartStopDet;
	wire sdaOut;
	wire sdaIn;

	assign sda = (sdaOut == 1'b0) ? 1'b0 : 1'bz;
	assign sdaIn = sda;



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
	wire rst_sync_to_clk;

	assign rst_n_sync_to_clk = rst_n_pipe[1];
	assign rst_sync_to_clk = ~rst_n_sync_to_clk;

	// debounce sda and scl
	reg [DEB_I2C_LEN - 1 : 0] sdaPipe;
	reg sdaDeb;
	reg [DEB_I2C_LEN - 1 : 0] sclPipe;
	reg sclDeb;
	always @(posedge clk) begin
		if(rst_n_sync_to_clk == 1'b0) begin
			sdaPipe <= {DEB_I2C_LEN{1'b1}};
			sdaDeb <= 1'b1;
			sclPipe <= {DEB_I2C_LEN{1'b1}};
			sclDeb <= 1'b1;
		end
		else begin
			if(&sclPipe[DEB_I2C_LEN - 1 : 1] == 1'b1) begin
				sclDeb <= 1'b1;
			end
			else if(|sclPipe[DEB_I2C_LEN - 1 : 1] == 1'b0) begin
				sclDeb <= 1'b0;
			end

			if(&sdaPipe[DEB_I2C_LEN - 1 : 1] == 1'b1) begin
				sdaDeb <= 1'b1;
			end
			else if(|sdaPipe[DEB_I2C_LEN - 1 : 1] == 1'b0) begin
				sdaDeb <= 1'b0;
			end

			sdaPipe <= {sdaPipe[DEB_I2C_LEN - 2 : 0], sdaIn};
			sclPipe <= {sclPipe[DEB_I2C_LEN - 2 : 0], scl};
		end
	end


	// delay scl and sda
	// sclDelayed is used as a delayed sampling clock
	// sdaDelayed is only used for start stop detection
	// Because sda hold time from scl falling is 0nS
	// sda must be delayed with respect to scl to avoid incorrect
	// detection of start/stop at scl falling edge. 
	reg [SCL_DEL_LEN - 1 : 0] sclDelayed;
	reg [SDA_DEL_LEN - 1 : 0] sdaDelayed;
	always @(posedge clk) begin
		if(rst_n_sync_to_clk == 1'b0) begin
			sclDelayed <= {SCL_DEL_LEN{1'b1}};
			sdaDelayed <= {SDA_DEL_LEN{1'b1}};
		end
		else begin
			sclDelayed <= {sclDelayed[SCL_DEL_LEN - 2 : 0], sclDeb};
			sdaDelayed <= {sdaDelayed[SDA_DEL_LEN - 2 : 0], sdaDeb};
		end
	end

	// start stop detection
	reg startEdgeDet;
	reg [1 : 0] startStopDetState;
	always @(posedge clk) begin
		if(rst_n_sync_to_clk == 1'b0) begin
			startEdgeDet <= 1'b0;
			startStopDetState <= `NULL_DET;
		end
		else begin
			if(sclDeb == 1'b1 && sdaDelayed[SDA_DEL_LEN - 2] == 1'b0 && sdaDelayed[SDA_DEL_LEN - 1] == 1'b1) begin
				startEdgeDet <= 1'b1;
			end
			else begin
				startEdgeDet <= 1'b0;
			end

			if(clearStartStopDet == 1'b1) begin
				startStopDetState <= `NULL_DET;
			end
			else begin
				if(sclDeb == 1'b1) begin
					if(sdaDelayed[SDA_DEL_LEN - 2] == 1'b1 && sdaDelayed[SDA_DEL_LEN - 1] == 1'b0) begin
						startStopDetState <= `STOP_DET;
					end
					else if(sdaDelayed[SDA_DEL_LEN - 2] == 1'b0 && sdaDelayed[SDA_DEL_LEN - 1] == 1'b1) begin
						startStopDetState <= `START_DET;
					end
				end
				else begin
				end
			end
		end
	end



	// Instantiation of iic-slave
	wire rst_intf;
	assign rst_intf = rst_sync_to_clk | startEdgeDet;

	wire [IIC_SLAVE_ADDR_WIDTH : 0] regAddr;
	wire [IIC_SLAVE_DATA_WIDTH : 0] dataToRegIF;
	wire writeEn;
	wire [IIC_SLAVE_DATA_WIDTH : 0] dataFromRegIF;

	assign wen = writeEn; //to reg
	assign wdata = dataToRegIF;
	assign waddr = regAddr;

	assign ren = ~writeEn; //to iic
	assign dataFromRegIF = rdata;
	assign raddr = regAddr;

	wire scl_delayed;
	assign scl_delayed = sclDelayed[SCL_DEL_LEN - 1];

	serialInterface # (
		.I2C_ADDRESS(I2C_ADDRESS)
		) u_serialInterface (
			.clk(clk), 
			.rst(rst_intf), 
			.dataIn(dataFromRegIF), 
			.dataOut(dataToRegIF), 
			.writeEn(writeEn),
			.regAddr(regAddr), 
			.scl(scl_delayed), 
			.sdaIn(sdaDeb), 
			.sdaOut(sdaOut), 
			.startStopDetState(startStopDetState),
			.clearStartStopDet(clearStartStopDet) 
		);
	// Add user logic here

	// User logic ends

	endmodule
