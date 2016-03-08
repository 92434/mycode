`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2015/12/05 08:42:09
// Design Name: 
// Module Name: gen_fs_en
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module gen_fs_en(
    input                                           sys_clk,
    input                                           glb_rst_n,
    input							[31:0]		    SYS_Baud_Num,//32'd2500 --> 25M BaudRate   SYS_Baud_mode,
    output          reg                             fs_en,
    output          reg                             fs_en2
    );
    
    parameter SYS_Freq_Num = 32'd10000;
    
    reg [31:0]			       byte_en_step;
    reg [31:0]                clk_period;
    wire [31:0]               byte_en_cnt;
    reg  [31:0]            byte_en_phase;
    
    reg         clk_period_rdy;
    
    always @(posedge sys_clk)begin
    if(~glb_rst_n)begin
        byte_en_step   <= 0;
        clk_period          <= 0;
        clk_period_rdy <= 0;
    end
    else begin
        byte_en_step   <= 2*SYS_Baud_Num;//32'd3036096 ;
        clk_period          <= SYS_Freq_Num;//32'd259920000;
        clk_period_rdy <= 1;
    end
    end
    
    reg                                            clk_period_rdy_1dly;//1dly
    reg                                            clk_period_rdy_2dly;//2dly
    reg                                            clk_period_rdy_3dly;//3dly
    reg                                            clk_period_rdy_4dly;//4dly
    reg                                            clk_period_rdy_5dly;//5dly
    reg                                            clk_period_rdy_6dly;//6dly
    reg                                            clk_period_rdy_7dly;//7dly
    
    
    always @(posedge sys_clk)begin
        if(~glb_rst_n)begin    
            clk_period_rdy_1dly <= 1'b0;//1dly
            clk_period_rdy_2dly <= 1'b0;//2dly
            clk_period_rdy_3dly <= 1'b0;//3dly
            clk_period_rdy_4dly <= 1'b0;//4dly
            clk_period_rdy_5dly <= 1'b0;//5dly
            clk_period_rdy_6dly <= 1'b0;//6dly
            clk_period_rdy_7dly <= 1'b0;//7dly
        end
        else begin
            clk_period_rdy_1dly <= clk_period_rdy;//1dly
            clk_period_rdy_2dly <= clk_period_rdy_1dly;//2dly
            clk_period_rdy_3dly <= clk_period_rdy_2dly;//3dly
            clk_period_rdy_4dly <= clk_period_rdy_3dly;//4dly
            clk_period_rdy_5dly <= clk_period_rdy_4dly;//5dly
            clk_period_rdy_6dly <= clk_period_rdy_5dly;//6dly
            clk_period_rdy_7dly <= clk_period_rdy_6dly;//7dly
        end
    end
    
    //assign     byte_en_step   = TS_Bytes_Num_Frm*SYS_Baud_Num;//32'd3036096 ;
    //assign    clk_period          = PL_Bytes_Num_Frm*SYS_Freq_Num;//32'd259920000;
    assign     byte_en_cnt        = byte_en_phase+byte_en_step;
    
    always @(posedge sys_clk)begin
    if(~glb_rst_n)
        byte_en_phase    <=0;
    else if((byte_en_cnt>=clk_period)&&(clk_period_rdy_7dly))
        byte_en_phase<=byte_en_cnt-clk_period;
    else
        byte_en_phase<=byte_en_cnt;
    end
    
    reg        ts_clk_pre; 
    
    always @(posedge sys_clk)begin
    if(~glb_rst_n)
        ts_clk_pre<=1'b0;
    else if((byte_en_cnt>=clk_period)&&(clk_period_rdy_7dly))
        ts_clk_pre<=1'b1;
    else
        ts_clk_pre<=1'b0;
    end
    
    
    
    reg                                            ts_clk_pre_1dly;//1dly
    reg                                            ts_clk_pre_2dly;//2dly
    reg                                            ts_clk_pre_3dly;//3dly
    reg                                            ts_clk_pre_4dly;//4dly
    reg                                            ts_clk_pre_5dly;//5dly
    reg                                            ts_clk_pre_6dly;//6dly
    reg                                            ts_clk_pre_7dly;//7dly
    
    
    always @(posedge sys_clk)begin
        if(~glb_rst_n)begin    
            ts_clk_pre_1dly <= 1'b0;//1dly
            ts_clk_pre_2dly <= 1'b0;//2dly
            ts_clk_pre_3dly <= 1'b0;//3dly
            ts_clk_pre_4dly <= 1'b0;//4dly
            ts_clk_pre_5dly <= 1'b0;//5dly
            ts_clk_pre_6dly <= 1'b0;//6dly
            ts_clk_pre_7dly <= 1'b0;//7dly
            fs_en2 <= 1'b0;//8dly
        end
        else begin
            ts_clk_pre_1dly <= ts_clk_pre;//1dly
            ts_clk_pre_2dly <= ts_clk_pre_1dly;//2dly
            ts_clk_pre_3dly <= ts_clk_pre_2dly;//3dly
            ts_clk_pre_4dly <= ts_clk_pre_3dly;//4dly
            ts_clk_pre_5dly <= ts_clk_pre_4dly;//5dly
            ts_clk_pre_6dly <= ts_clk_pre_5dly;//6dly
            ts_clk_pre_7dly <= ts_clk_pre_6dly;//7dly
            fs_en2 <= ts_clk_pre_7dly;//8dly
        end
    end
    
    reg					fs_reg1;
    reg                    fs_reg2;
    always@(posedge sys_clk) begin
        fs_reg1<=fs_en2;
        fs_reg2<=fs_reg1;
    end
    
    always @(posedge sys_clk) begin
        if((fs_reg2==1'b0)&&(fs_reg1==1'b1))
            fs_en<=1'b1;
        else
            fs_en<=1'b0;
    end
    
endmodule
