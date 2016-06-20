`timescale 1 ns / 1 ps

module axi4_stream_master_v1_0_M00_AXIS #
	(
		// Users to add parameters here
		parameter integer NUMBER_OF_OUTPUT_WORDS = 16,

		parameter integer BULK_OF_DATA = 16,
		parameter integer BULK_DEPTH = 16,

		// User parameters ends
		// Do not modify the parameters beyond this line

		// Width of S_AXIS address bus. The slave accepts the read and write addresses of width C_M_AXIS_TDATA_WIDTH.
		parameter integer C_M_AXIS_TDATA_WIDTH = 32,
		// Start count is the numeber of clock cycles the master will wait before initiating/issuing any transaction.
		parameter integer C_M_START_COUNT = 32
	)
	(
		// Users to add ports here

		input wire wclk,
		input wire wen,
		input wire [C_M_AXIS_TDATA_WIDTH - 1 : 0] wdata,

		output wire r_ready,
		output wire error_full,
		output wire error_empty,

		// User ports ends
		// Do not modify the ports beyond this line

		// Global ports
		input wire M_AXIS_ACLK,
		//
		input wire M_AXIS_ARESETN,
		// Master Stream Ports. TVALID indicates that the master is driving a valid transfer, A transfer takes place when both TVALID and TREADY are asserted.
		output wire M_AXIS_TVALID,
		// TDATA is the primary payload that is used to provide the data that is passing across the interface from the master.
		output wire [C_M_AXIS_TDATA_WIDTH - 1 : 0] M_AXIS_TDATA,
		// TSTRB is the byte qualifier that indicates whether the content of the associated byte of TDATA is processed as a data byte or a position byte.
		output wire [(C_M_AXIS_TDATA_WIDTH / 8) - 1 : 0] M_AXIS_TSTRB,
		// TLAST indicates the boundary of a packet.
		output wire M_AXIS_TLAST,
		// TREADY indicates that the slave can accept a transfer in the current cycle.
		input wire M_AXIS_TREADY
	);
	//Total number of output data.
	// Total number of output data
	// function called clogb2 that returns an integer which has the
	// value of the ceiling of the log base 2.
	function integer clogb2(input integer bit_depth);
		begin
			for(clogb2 = 0; bit_depth>0; clogb2 = clogb2 + 1)
				bit_depth = bit_depth >> 1;
		end
	endfunction

	// WAIT_COUNT_BITS is the width of the wait counter.
	localparam integer WAIT_COUNT_BITS = clogb2(C_M_START_COUNT - 1);

	//localparam integer NUMBER_OF_OUTPUT_WORDS = 16;

	// bit_num gives the minimum number of bits needed to address 'depth' size of FIFO.
	localparam bit_num = clogb2(NUMBER_OF_OUTPUT_WORDS);

	// Define the states of state machine
	// The control state machine oversees the writing of input streaming data to the FIFO,
	// and outputs the streaming data from the FIFO
	localparam [1:0] IDLE = 2'b00; // This is the initial/idle state

	localparam [1:0] INIT_COUNTER = 2'b01; // This state initializes the counter, ones the counter reaches C_M_START_COUNT count, the state machine changes state to INIT_WRITE
	localparam [1:0] SEND_STREAM = 2'b10; // In this state the stream data is output through M_AXIS_TDATA

	// State variable
	reg [1:0] mst_exec_state = IDLE;

	// Example design FIFO read pointer
	reg [bit_num - 1:0] read_pointer;

	// AXI Stream internal signals
	//wait counter. The master waits for the user defined number of clock cycles before initiating a transfer.
	reg [WAIT_COUNT_BITS : 0] count;
	//streaming data valid
	wire axis_tvalid;
	wire axis_tlast;

	//The master has issued all the streaming data stored in FIFO
	//reg tx_done = 0;

	wire [C_M_AXIS_TDATA_WIDTH - 1 : 0] rdata;
	//reg [C_M_AXIS_TDATA_WIDTH - 1 : 0] rdata_R = 0;

	// I/O Connections assignments

	assign M_AXIS_TVALID = axis_tvalid;
	//assign M_AXIS_TDATA = stream_data_out;
	assign M_AXIS_TDATA = rdata;
	assign M_AXIS_TLAST = axis_tlast;
	assign M_AXIS_TSTRB = {(C_M_AXIS_TDATA_WIDTH / 8){1'b1}};

	wire ren;
	// Control state machine implementation
	always @(posedge M_AXIS_ACLK) begin
		// Synchronous reset (active low)
		if (!M_AXIS_ARESETN) begin
			mst_exec_state <= IDLE;
		end
		else begin
			case (mst_exec_state)
				// The slave starts accepting tdata when
				// there tvalid is asserted to mark the
				// presence of valid streaming data
				IDLE: begin
					if(r_ready == 1) begin
						count <= 0;
						mst_exec_state <= INIT_COUNTER;
					end
					else begin
					end
				end

				// The slave starts accepting tdata when
				// there tvalid is asserted to mark the
				// presence of valid streaming data
				INIT_COUNTER: begin
					if(count == C_M_START_COUNT - 1) begin
						mst_exec_state <= SEND_STREAM;
					end
					else begin
						count <= count + 1;
						mst_exec_state <= INIT_COUNTER;
					end
				end

				SEND_STREAM: begin
					// The example design streaming master functionality starts
					// when the master drives output tdata from the FIFO and the slave
					// has finished storing the S_AXIS_TDATA
					if(axis_tlast == 1) begin
						mst_exec_state <= IDLE;
					end
					else begin
					end
				end
				default: begin
				end
			endcase
		end
	end


	//tvalid generation
	//axis_tvalid is asserted when the control state machine's state is SEND_STREAM and
	//number of output streaming data is less than the NUMBER_OF_OUTPUT_WORDS.
	assign axis_tvalid = (mst_exec_state == SEND_STREAM) ? 1 : 0;
	assign ren = (axis_tvalid == 1 && M_AXIS_TREADY == 1) ? 1 : 0;

	// AXI tlast generation
	// axis_tlast is asserted number of output streaming data is NUMBER_OF_OUTPUT_WORDS - 1
	// (0 to NUMBER_OF_OUTPUT_WORDS - 1)
	assign axis_tlast = (ren == 1 && read_pointer == NUMBER_OF_OUTPUT_WORDS - 1) ? 1 : 0;

	//read_pointer pointer
	always@(posedge M_AXIS_ACLK) begin
		if(!M_AXIS_ARESETN) begin
			read_pointer <= 0;
		end
		else begin
			// read pointer is incremented after every read from the FIFO
			// when FIFO read signal is enabled.
			if (ren == 1) begin
				if(read_pointer == (NUMBER_OF_OUTPUT_WORDS - 1)) begin
					read_pointer <= 0;
				end
				else begin
					read_pointer <= read_pointer + 1;
				end
			end
			else begin
			end
		end
	end


	// Add user logic here
	////////////////////////////////////////////////////////////////////////////////////////////////

	my_fifo #(
			.DATA_WIDTH(C_M_AXIS_TDATA_WIDTH),
			.BULK_OF_DATA(BULK_OF_DATA),
			.BULK_DEPTH(BULK_DEPTH)
		) my_fifo_inst (
			.rst_n(M_AXIS_ARESETN),
			.wclk(wclk),
			.rclk(M_AXIS_ACLK),
			.wdata(wdata),
			.rdata(rdata),
			.w_enable(wen),
			.r_enable(ren),
			.r_ready(r_ready),
			.error_full(error_full),
			.error_empty(error_empty)
		);
	////////////////////////////////////////////////////////////////////////////////////////////////
	// User logic ends

endmodule
