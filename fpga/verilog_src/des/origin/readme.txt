XAPP 270: README file for DES and Triple-DES 
Export and Usage Restrictions:

The U.S. government has imposed certain restrictions 
on the export of 
DES encryption software and hardware. Xilinx recommends checking with the 
U. S. 
government FIPS-46-3, 1999 publication for export restrictions and 
current government policies 
on the export of these commodities, software, 
and technology. FIPS-46-3 is available online 
at
http://csrc.nist.gov/encryption/tkencryption.html

Cryptographic devices and technical data 
regarding them are subject to 
Federal Government export controls. Exports of cryptographic modules 
implementing this standard and technical data regarding them must
comply with these Federal regulations 
and be licensed by the Bureau of 
Export Administration of the U.S. Department of Commerce.

HDL Design Files Details:

DES and Triple DES reference design VHDL and Verilog files can be found 
in 
Des and Triple-Des folders. For implementing Triple-Des, include all the
files in Des folder.

 
DES Files Hierarchy: Top level file - DES
 - DES_F - DES_KEY
 - DES_SBOX
 - DES_COST


 Triple-DES Files Hierarchy: Top level file - Triple_DES
  - DES
   - DES_F
  - DES_KEY
 - DES_SBOX
 - DES_COST
Test Bench:


The testbench folder has a sample VHDL testbench and this can be used for both
VHDL and Verilog Simulation.


This testbench does not cover all NIST test vectors. 
Xilinx recommends 
verification of all NIST test vectors for DES compliance. 
For a complete 
test suite, refer to NIST test requirements at 
http://csrc.nist.gov/cryptval/des.htm

-


Tools used:

Modelsim 5.5b
Synplify 7:00BETA
Xilinx 4.1i

-----
-

Implementation:

Use a 4 ns period constraint in the UCF file.
PAR effort level is set to 5.

