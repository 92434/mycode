*******************************************************************************
** © Copyright 2009 Xilinx, Inc. All rights reserved.
** This file contains confidential and proprietary information of Xilinx, Inc. and 
** is protected under U.S. and international copyright and other intellectual property laws.
*******************************************************************************
**   ____  ____ 
**  /   /\/   / 
** /___/  \  /   Vendor: Xilinx 
** \   \   \/    
**  \   \        readme.txt Version: 1.0  
**  /   /        Date Last Modified: August 17, 2009 
** /___/   /\    Date Created:       August 17, 2009
** \   \  /  \   Associated Filename: xapp1015_sec1_ASI_LVDS.zip
**  \___\/\___\ 
** 
**  Device: Spartan-3E
**  Purpose: DVB-ASI LVDS Reference Design
**  Reference: XAPP1015 Section 1
**  Revision History: 
**      August 17, 2009: Initial release
**   
*******************************************************************************
**
**  Disclaimer: 
**
**		This disclaimer is not a license and does not grant any rights to the materials 
**              distributed herewith. Except as otherwise provided in a valid license issued to you 
**              by Xilinx, and to the maximum extent permitted by applicable law: 
**              (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND WITH ALL FAULTS, 
**              AND XILINX HEREBY DISCLAIMS ALL WARRANTIES AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, 
**              INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-INFRINGEMENT, OR 
**              FITNESS FOR ANY PARTICULAR PURPOSE; and (2) Xilinx shall not be liable (whether in contract 
**              or tort, including negligence, or under any other theory of liability) for any loss or damage 
**              of any kind or nature related to, arising under or in connection with these materials, 
**              including for any direct, or any indirect, special, incidental, or consequential loss 
**              or damage (including loss of data, profits, goodwill, or any type of loss or damage suffered 
**              as a result of any action brought by a third party) even if such damage or loss was 
**              reasonably foreseeable or Xilinx had been advised of the possibility of the same.


**  Critical Applications:
**
**		Xilinx products are not designed or intended to be fail-safe, or for use in any application 
**		requiring fail-safe performance, such as life-support or safety devices or systems, 
**		Class III medical devices, nuclear facilities, applications related to the deployment of airbags,
**		or any other applications that could lead to death, personal injury, or severe property or 
**		environmental damage (individually and collectively, "Critical Applications"). Customer assumes 
**		the sole risk and liability of any use of Xilinx products in Critical Applications, subject only 
**		to applicable laws and regulations governing limitations on product liability.

**  THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS PART OF THIS FILE AT ALL TIMES.

*******************************************************************************

This is the DVB-ASI Receiver and Transmitter reference design for Spartan-3E devices.

*******************************************************************************

** IMPORTANT NOTES **

1) The files in this reference design are intended to be used with XST.
Some of them contain XST-specific constraints that would need to be translated
if a different synthesizer is used.
*******************************************************************************

Included are designs for a DVB-ASI Receiver and Transmitter that can be used 
seperately as noted in the application note section.

In addition, there is a demo included in the reference design that provides
an example of how to integrate the receiver and transmitter together to build
a complete DVB-ASI pass-through design.  This is useful for demonstrating the
receiver and transmitter by inserting this demo between a DVB-ASI source and
recevier.  A bit stream file for the CTXIL206 board is included.


To implement this demo, add all verilog or VHDL the files to an ISE project.  ALso include the UCF
file.  There are also two files required for a coregen core used in the design.  These files are 
named dcfifo_*.*.  These files can be placed anywhere, but the simplest is to place them in the project
directory.  If these files are placed elsewhere, you must set the "Macro Search Path" in the translate
properties.
