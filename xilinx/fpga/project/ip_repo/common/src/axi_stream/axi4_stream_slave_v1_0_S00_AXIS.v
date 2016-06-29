`timescale 1 ns / 1 ps

module axi4_stream_slave_v1_0_S00_AXIS #
	(
		// Users to add parameters here
		parameter integer NUMBER_OF_INPUT_WORDS = 16,

		parameter integer BULK_OF_DATA = 16,
		parameter integer BULK_DEPTH = 16,
		// User parameters ends
		// Do not modify the parameters beyond this line

		// AXI4Stream sink: Data Width
		parameter integer C_S_AXIS_TDATA_WIDTH = 32
	)
	(
		// Users to add ports here
		input wire rclk,
		input wire ren,
		output wire [C_S_AXIS_TDATA_WIDTH - 1 : 0] rdata,
		output wire r_ready,
		output wire error_full,
		output wire error_empty,

		// User ports ends
		// Do not modify the ports beyond this line

		// AXI4Stream sink: Clock
		input wire S_AXIS_ACLK,
		// AXI4Stream sink: Reset
		input wire S_AXIS_ARESETN,
		// Ready to accept data in
		output wire S_AXIS_TREADY,
		// Data in
		input wire [C_S_AXIS_TDATA_WIDTH - 1 : 0] S_AXIS_TDATA,
		// Byte qualifier
		input wire [(C_S_AXIS_TDATA_WIDTH / 8) - 1 : 0] S_AXIS_TSTRB,
		// Indicates boundary of last packet
		input wire S_AXIS_TLAST,
		// Data is in valid
		input wire S_AXIS_TVALID
	);

	// function called clogb2 that returns an integer which has the 
	// value of the ceiling of the log base 2.
	function integer clogb2 (input integer bit_depth);
		begin
			for(clogb2 = 0; bit_depth > 0; clogb2 = clogb2 + 1) begin
				bit_depth = bit_depth >> 1;
			end
		end
	endfunction

	// Total number of input data.
	//localparam NUMBER_OF_INPUT_WORDS = 16;
	// bit_num gives the minimum number of bits needed to address 'NUMBER_OF_INPUT_WORDS' size of FIFO.
	localparam bit_num = clogb2(NUMBER_OF_INPUT_WORDS);
	// Define the states of state machine
	// The control state machine oversees the writing of input streaming data to the FIFO,
	// and outputs the streaming data from the FIFO
	localparam [1 : 0] IDLE = 1'b0; //This is the initial/idle state In this state FIFO is written with the input stream data S_AXIS_TDATA 
	localparam [1 : 0] WRITE_FIFO = 1'b1; //This is the initial/idle state In this state FIFO is written with the input stream data S_AXIS_TDATA 

	wire axis_tready;
	// State variable
	reg mst_exec_state = 0;

	// FIFO write pointer
	reg [bit_num - 1 : 0] write_pointer = 0;
	// sink has accepted all the streaming data and stored in FIFO
	//
	wire writes_done;
	// I/O Connections assignments

	assign S_AXIS_TREADY = axis_tready;


	// Control state machine implementation
	always @(posedge S_AXIS_ACLK) begin
		// Synchronous reset (active low)
		if (!S_AXIS_ARESETN) begin
			mst_exec_state <= IDLE;
		end
		else begin
			case (mst_exec_state)
				IDLE: begin
					// The sink starts accepting tdata when 
					// there tvalid is asserted to mark the
					// presence of valid streaming data 
					if (S_AXIS_TVALID) begin
						mst_exec_state <= WRITE_FIFO;
					end
					else begin
						mst_exec_state <= IDLE;
					end
				end
				WRITE_FIFO: begin
					// When the sink has accepted all the streaming input data,
					// the interface swiches functionality to a streaming master
					if(writes_done == 1) begin
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

	// AXI Streaming Sink 
	// 
	// The example design sink is always ready to accept the S_AXIS_TDATA until
	// the FIFO is not filled with NUMBER_OF_INPUT_WORDS number of input words.
	
	assign axis_tready = ((S_AXIS_TVALID == 1) && (mst_exec_state == WRITE_FIFO) && (error_full == 0));

	wire wen;
	assign wen = axis_tready;

	wire s_axis_wlast;
	assign s_axis_wlast = ((wen == 1) && (write_pointer >= NUMBER_OF_INPUT_WORDS - 1)) ? 1 : 0;
	assign writes_done = ((s_axis_wlast == 1) || (S_AXIS_TLAST == 1)) ? 1 : 0;

	always@(posedge S_AXIS_ACLK) begin
		if(!S_AXIS_ARESETN) begin
			write_pointer <= 0;
		end
		else  begin
			// write pointer is incremented after every write to the FIFO
			// when FIFO write signal is enabled.
			if(wen == 1) begin
				if(write_pointer >= NUMBER_OF_INPUT_WORDS - 1) begin
					write_pointer <= 0;
				end
				else begin
					write_pointer <= write_pointer + 1;
				end
			end
			else begin
			end
		end
	end

	// Add user logic here
	my_fifo #(
			.DATA_WIDTH(C_S_AXIS_TDATA_WIDTH),
			.BULK_OF_DATA(BULK_OF_DATA),
			.BULK_DEPTH(BULK_DEPTH)
		) my_fifo_inst (
			.rst_n(S_AXIS_ARESETN),
			.wclk(S_AXIS_ACLK),
			.rclk(rclk),
			.wdata(S_AXIS_TDATA),
			.rdata(rdata),
			.w_enable(wen),
			.r_enable(ren),
			.r_ready(r_ready),
			.error_full(error_full),
			.error_empty(error_empty)
		);

	// User logic ends

	endmodule
