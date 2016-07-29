`timescale 1 ns / 1 ps

module myip_testbench #
	(
		parameter integer COUNT = 32
	)
	(
	);

	wire clk;
	reg rst;
	reg [7 : 0] in_data = 0;
	wire [7 : 0] result;

	initial begin
		rst = 0;
		#2
		rst = 1;
	end

	// Add user logic here
	clkgen #(.clk_period(1)) xiaofeiclk(.clk(clk));
	
	multi_to_one #(.COUNT(COUNT)) inst (
	                               .clk(clk),
	                               .rst(rst),
	                               .in_data(in_data),
	                               .result(result)
	                               );

    always @(posedge clk) begin
        if(rst == 0) begin
            in_data <= 0;
        end
        else begin
            if(in_data == 35) begin
                in_data <= 0;
            end
            else begin
                in_data <= in_data + 1;
            end
        end
    end

	// User logic ends

endmodule
