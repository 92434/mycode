-------------------------------------------------------------------------------- 
-- Copyright (c) 2006, 2007 Xilinx, Inc. 
-- All Rights Reserved 
-------------------------------------------------------------------------------- 
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /   Vendor: Xilinx 
-- \   \   \/    Author: Jeff Hutchings, Advanced Product Division, Xilinx, Inc.
--  \   \        Filename: $RCSfile: dvb_fifo.vhd,v $
--  /   /        Date Last Modified:  $Date: 2008-04-10 08:12:43-06 $
-- /___/   /\    Date Created: Mar 26, 2007
-- \   \  /  \ 
--  \___\/\___\ 
-- 
--
-- Revision History: 
-- $Log: dvb_fifo.vhd,v $
-- Revision 1.2  2008-04-10 08:12:43-06  hutching
-- New release for 10.1i
--
-- Revision 1.1  2008-02-06 14:23:23-07  hutching
-- Added reset to coregen FIFO.
--
-- Revision 1.0  2007-06-07 11:44:13-06  hutching
-- Initial revision
--
-------------------------------------------------------------------------------- 
--   
--   XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" 
--   AS A COURTESY TO YOU, SOLELY FOR USE IN DEVELOPING PROGRAMS AND 
--   SOLUTIONS FOR XILINX DEVICES.  BY PROVIDING THIS DESIGN, CODE, 
--   OR INFORMATION AS ONE POSSIBLE IMPLEMENTATION OF THIS FEATURE, 
--   APPLICATION OR STANDARD, XILINX IS MAKING NO REPRESENTATION 
--   THAT THIS IMPLEMENTATION IS FREE FROM ANY CLAIMS OF INFRINGEMENT, 
--   AND YOU ARE RESPONSIBLE FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE 
--   FOR YOUR IMPLEMENTATION.  XILINX EXPRESSLY DISCLAIMS ANY 
--   WARRANTY WHATSOEVER WITH RESPECT TO THE ADEQUACY OF THE 
--   IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO ANY WARRANTIES OR 
--   REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE FROM CLAIMS OF 
--   INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
--   FOR A PARTICULAR PURPOSE. 
--
-------------------------------------------------------------------------------- 

-------------------------------------------------------------------------------
-- Module Description 
-- This module instantiates the CoreGen FIFO and has the FIFO control state machine.
-- This -- module is the last piece of the rx data path for the DVB ASI receiver.
-- The user interfaces with this module to extract the MPEG-2 packet data from the
-- receiver and forward to the next section.  The design provides comma correction
-- by inserting or deleting commas as needed to keep the FIFO level between two
-- predefined water marks.


library IEEE;
use IEEE.STD_LOGIC_1164.all;


entity dvb_fifo is
  
  generic (
    FIFO_LEVEL_HIGH : std_logic_vector (10 downto 0) := "11000000000";  
    FIFO_LEVEL_LOW  : std_logic_vector (10 downto 0) := "00100010001"); 
                                                                   

  port (
    clk            : in  std_logic;     -- 2X word-rate clock
    user_clk       : in  std_logic;     -- FIFO read domain clock <= clk
    data8b_in      : in  std_logic_vector (7 downto 0);  -- 8-bit decoded input data
    data8b_valid   : in  std_logic;     -- New data flag from decoder;
    framed         : in  std_logic;     -- ASI framing sequence detect
    reset          : in  std_logic;     -- Synchronous reset
    is_kchar       : in  std_logic;     -- Indicates if 8-bit data is comma
    fifo_rden      : in  std_logic;     -- FIFO read enable (unused)
    fifo_dout_8b   : out std_logic_vector (7 downto 0);  -- 8-bit output data
    fifo_kcode_out : out std_logic;     -- Current output is comma
    rd_count       : out std_logic_vector (10 downto 0);  -- FIFO read count 
    fifo_empty     : out std_logic;     -- FIFO empty flag - synchronous to user_clk
    fifo_full      : out std_logic);    -- FIFO full flag - synchronous to clk

                                        
end dvb_fifo;


architecture rtl of dvb_fifo is

  component dcfifo_2kx9
    port (
      din           : in  std_logic_vector (8 downto 0);
      wr_en         : in  std_logic;
      wr_clk        : in  std_logic;    
      rd_en         : in  std_logic;
      rd_clk        : in  std_logic;
      rst           : in  std_logic;
      dout          : out std_logic_vector (8 downto 0);
      full          : out std_logic;
      empty         : out std_logic;    
      wr_data_count : out std_logic_vector (10 downto 0);
      rd_data_count : out std_logic_vector (10 downto 0));
  end component;

  --attribute max_delay : string;

  -- State machine enumerated type
  type STATE_TYPE is (FIFO_RESET,
                      FIFO_MONITOR,
                      FIFO_INS_COMMA,
                      FIFO_DEL_COMMA);

  attribute STATE_ENUM : string;
  attribute STATE_ENUM of STATE_TYPE : type is "00 10 11 01";

  -- Internal Registers
  signal data8b_in_R : std_logic_vector (7 downto 0);
  signal is_kchar_R  : std_logic;
  signal data8b_valid_R : std_logic;
  signal framed_R : std_logic;

  -- Local fifo reset signal.  Coregen FIFO uses asynchronous reset.  This
  -- local net only sources the FIFO and has a MAXDELAY constraint to insure
  -- no race conditions with the clock signal 'clk'
  
 -- signal fifo_rst : std_logic := '0';  -- Local reset for coregen FIFO.
--  attribute max_delay of fifo_rst : signal is "3ns";

  -- Registers to equalize data path delay with control path
  signal data8b_in_R2 : std_logic_vector (7 downto 0);
  signal data8b_valid_R2 : std_logic;
  signal is_kchar_R2 : std_logic;

  -- Internal Signals
  signal din_fifo : std_logic_vector (8 downto 0);   -- FIFO input data.
  signal fifo_dout : std_logic_vector (8 downto 0);  -- Internal FIFO data out
  signal wr_count : std_logic_vector (10 downto 0);  -- FIFO write count

  -- Internal control path signals from state machine.
  signal fifo_din_sel : std_logic;   -- Selects between real data or comma constant
  signal fifo_wr_comma : std_logic;
  signal fifo_wren_gate : std_logic;
  signal fifo_wren : std_logic;

  -- State variables
  signal fifoctlr_current_state : STATE_TYPE := FIFO_RESET;
  signal fifoctlr_next_state    : STATE_TYPE;
  
  
begin  -- rtl

  CLK_PROC: process (clk)
  begin  -- process CLK_PROC
    if clk'event and clk = '1' then  -- rising clock edge
      if reset = '1' then            -- synchronous reset (active high)
        data8b_valid_R <= '0';
   	framed_R <= '0';
      else
        data8b_in_R <= data8b_in;
	data8b_in_R2 <= data8b_in_R;
	is_kchar_R <= is_kchar;
        is_kchar_R2 <= is_kchar_R;
        data8b_valid_R <= data8b_valid;
        data8b_valid_R2 <= data8b_valid_R;
        framed_R <= framed;
      end if;
    end if;
  end process CLK_PROC;

  -----------------------------------------------------------------------------
  -- FIFO Data Path:
  --
  -- The FIFO data path needs some muxing to handle the comma insertion case.
  -- When we need to insert a comma, the muxes switch the input data of the FIFO
  -- to a constant 0xBC value which is the 8-bit comma value.  Since 0xBC is also
  -- a valid data value, we also mux in a value of 0x1 for the kchar input bit to
  -- tag the FIFO data.  Data path mux muxes 8-bit input data and kchar flag between
  -- actual values from decoder, and constant values for comma insertion.
  -- "fifo_din_sel" is asserted from the state machine to select the correct data
  -- path.  FIFO tagged output data is separated.

  din_fifo <= ('1' & X"BC") when fifo_din_sel = '1' else (is_kchar_R2 & data8b_in_R2);
  fifo_dout_8b <= fifo_dout(7 downto 0);
  fifo_kcode_out <= fifo_dout(8);

  -----------------------------------------------------------------------------
  -- FIFO write enable control logic.  In the comma insertion case, we assert the
  -- write enable only when the incoming data is not valid - which indicates a dead
  -- cycle.  This module runs at 2X the word rate, so on average, the
  -- "data_in_valid" input will only be asserted every other cycle.  The state
  -- machine looks for those cycles, and asserts the "fifo_wr_comma" signal, and
  -- the "fifo_wren_gate" signal.  In the comma deletion   -- case, the state
  -- machine deasserts the "fifo_wren_gate" signal which disables all writes to the
  -- FIFO.  Otherwise, the "fifo_wr_comma" signal  is deasserted, and the
  -- "fifo_wren_gate" signal is asserted,  allowing writes to occurr normally as
  -- valid data comes through.

  fifo_wren <= ((data8b_valid_R2 or fifo_wr_comma) and fifo_wren_gate);

  -----------------------------------------------------------------------------
  -- FIFO Controller.
  --
  -- This state machine manages the "write" side of the FIFO and does the comma
  -- insertion and deletion; thereby providing basic comma correction capability
  -- to match small frequency differences between the receiver clock domain and the
  -- user clock domain.  A block RAM based CoreGen FIFO is used as the asynchronous
  -- FIFO storage block.  Basic operation is as follows:

  -- Current/next state logic
  FIFO_NS_PROC: process (clk)
  begin  -- process FIFO_NS_PROC
    if clk'event and clk = '1' then  -- rising clock edge
      if reset = '1' then            -- synchronous reset (active high)
        fifoctlr_current_state <= FIFO_RESET;
      else
       fifoctlr_current_state <= fifoctlr_next_state;
      end if;
    end if;
  end process FIFO_NS_PROC;

  ------------------------------------------------------------------------------
  -- Output decoding logic

  OUTPUT_DEC_PROC: process (fifoctlr_current_state)
  begin  -- process OUTPUT_DEC_PROC
    case fifoctlr_current_state is
      when FIFO_RESET =>
        fifo_din_sel   <= '1';       -- Select constant comma data path
        fifo_wren_gate <= '1';       -- Enable writes to the FIFO - comma stuffing
        fifo_wr_comma  <= '0';       -- Don't force writes.  They occurr normally.
	
      when FIFO_MONITOR =>
        fifo_wren_gate <= '1';       -- Enable writes to FIFO.
        fifo_din_sel   <= '0';       -- Select input data to FIFO
        fifo_wr_comma  <= '0';
	
      when FIFO_INS_COMMA =>
        fifo_din_sel   <= '1';      -- Select constant comma data to FIFO
        fifo_wren_gate <= '1';      -- Enable writes to FIFO
        fifo_wr_comma  <= '1';      -- Force write to FIFO
	
      when FIFO_DEL_COMMA =>
        fifo_din_sel   <= '0';     -- Select input data path to FIFO
        fifo_wren_gate <= '0';     -- Disable writes to FIFO
        fifo_wr_comma  <= '0';
    end case; -- case(fifoctlr_current_state)
  end process OUTPUT_DEC_PROC;


  -----------------------------------------------------------------------------
  -- Next state decode logic

  NS_PROC: process (fifoctlr_current_state, framed_R,wr_count, data8b_valid_R, is_kchar_R)
  begin  -- process NS_PROC
    case fifoctlr_current_state is
      	
	when FIFO_RESET =>
	  if (framed_R = '1') then                    -- Link is framed?
	    fifoctlr_next_state <= FIFO_MONITOR;
	  elsif (wr_count >= FIFO_LEVEL_HIGH) then    -- Not framed and high water mark.
	    fifoctlr_next_state <= FIFO_DEL_COMMA;  -- Delete a comma
	  elsif (wr_count <= FIFO_LEVEL_LOW) then   -- Not framed and low water mark?
	    fifoctlr_next_state <= FIFO_INS_COMMA;  -- Go insert a comma
	  else
	    fifoctlr_next_state <= FIFO_RESET;      -- Not framed, just keep stuffing the FIFO
          end if;
	
	when FIFO_MONITOR =>
	  if (framed_R /= '1') then   -- Error occurred?
	    fifoctlr_next_state <= FIFO_RESET;
	  elsif ((wr_count >= FIFO_LEVEL_HIGH) and data8b_valid_R = '1' and is_kchar_R = '1') then  
	    fifoctlr_next_state <= FIFO_DEL_COMMA;     -- go delete a comma
	  elsif ((wr_count <= FIFO_LEVEL_LOW) and data8b_valid_R /= '1') then  -- Gap in data and low water mark?
	    fifoctlr_next_state <= FIFO_INS_COMMA;     -- Go insert a comma
	  else
	    fifoctlr_next_state <= FIFO_MONITOR;       -- Otherwise, just monitor and let the data flow
          end if;
	
	when FIFO_DEL_COMMA =>
	  if (framed_R /= '1') then   -- Error occurred?
	    fifoctlr_next_state <= FIFO_RESET;
	  else 
	    fifoctlr_next_state <= FIFO_MONITOR;
          end if;
					 
        when FIFO_INS_COMMA =>
	  if (framed_R /= '1') then   -- Error occurred?
	    fifoctlr_next_state <= FIFO_RESET;
	  else 
            fifoctlr_next_state <= FIFO_MONITOR;
          end if;
	
    end case; -- case(fifoctrl_current_state)
  end process NS_PROC;
   

  -----------------------------------------------------------------------------
  -- Packet FIFO Instantiation
  --
  -- Asynchronous FIFO that transfers the Rx data packets 
  -- into the system clock domain.  The "write" side of this FIFO
  -- is completely under the control the comma insert/delete state machine
  -- in this module.
  --
  -- The FIFO is 9-bits wide to support data tagging.  The lower 8-bits are the
  -- decoded data from the 8b10b decoder.  The 9th bit, is the kchar indicator.
  -- These bits are separated in the output ports section of the module.

  -- Local process to re-register and duplicate incoming synchronous reset.
  -- This is to enable a local MAXDDELAY constraint on the asynchronous reset
  -- input to the FIFO for proper timing since the FIFO does not have a
  -- synchronous reset option.

  rx_fifo : dcfifo_2kx9 port map (din           => din_fifo,
                                  wr_en         => fifo_wren,
                                  wr_clk        => clk,
                                  rd_en         => fifo_rden,
                                  rd_clk        => user_clk,
                                  rst           => reset,
                                  dout          => fifo_dout,
                                  full          => fifo_full,
                                  empty         => fifo_empty,
                                  wr_data_count => wr_count,
                                  rd_data_count => rd_count);
  
end rtl;
