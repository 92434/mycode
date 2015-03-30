#!/bin/sh

#../tools/m_config fileencrypt 000012 ./../bin/rom_encrypt.bin  ./rom.bin de
cp G8/EN25Q64/1.bin xiaofei.bin
./rom_header xiaofei.bin 0 0 unfix
mkdir -p xi
./insert_file xiaofei.bin 0x0005c000 0x0008f9b1 xi/xi.0 0 0
#./insert_file xiaofei.bin 0x000ec000 0x00233d83 xi/xi.1 0 0
#./insert_file xiaofei.bin 0x00320000 0x00008084 xi/xi.2 0 0
#./insert_file xiaofei.bin 0x00000000 0x00000000 xi/xi.3 0 0
#./insert_file xiaofei.bin 0x00329000 0x00004f07 xi/xi.4 0 0
./insert_file xiaofei.bin 0x0032e000 0x0003f379 xi/xi.5 0 0
#./insert_file xiaofei.bin 0x0036e000 0x0009740c xi/xi.6 0 0
#./insert_file xiaofei.bin 0x00406000 0x00016ed3 xi/xi.7 0 0
#./insert_file xiaofei.bin 0x0041d000 0x0000467d xi/xi.8 0 0
#./insert_file xiaofei.bin 0x00422000 0x0027be2e xi/xi.9 0 0
#./insert_file xiaofei.bin 0x00000000 0x00000000 xi/xi.10 0 0
#./insert_file xiaofei.bin 0x0069e000 0x00021978 xi/xi.11 0 0
#./insert_file xiaofei.bin 0x006c0000 0x00000400 xi/xi.12 0 0
#./insert_file xiaofei.bin 0x006c1000 0x00002000 xi/xi.13 0 0
for i in xi/*;do ./lzma_op.py $i de;done
mkdir -p de
mv xi/*.de de/
#for i in de/*;do echo $i; ./search $i "...27...8f...3c...af...8c...8c...3c";done
#./insert_code de/xi.0.de 000b14f8 0
#./lzma_op.py de/xi.0.de en
#./insert_file de/xi.0.de.en 0 0 rom.bin 0x00054000 0
#./rom_header rom.bin 0 0x000cabe7 fix
##########find api for set key
#for i in de/*;do echo $i; ./search $i "...27...af...94...af...af...2e...af...af...af...af";done
for i in de/*;do echo $i; ./search $i "...27...af...af...af...af...af...af...af...00...8f...10...83...94...2e...14...27...24...8f...02...8f...8f...8f...8f...8f...8f";done
echo -----------------------------------------------------------------------------------------------

./insert_code de/xi.5.de 0009e994 0
./lzma_op.py de/xi.5.de en
./insert_file de/xi.5.de.en 0 0 xiaofei.bin 0x0032e000 0
./rom_header xiaofei.bin 5 $(printf "0x%08x" $(wc -c de/xi.5.de.en | awk '{ printf $1 }')) fix


#./insert_code de/xi.0.de 00152f84 0
#./lzma_op.py de/xi.0.de en
#./insert_file de/xi.0.de.en 0 0 xiaofei.bin 0x0005c000 0
#./rom_header xiaofei.bin 0 0x00101eff fix



#../tools/m_config fileencrypt 000012 rom.bin rom_encrypt.bin en
##########find printf
#for i in de/*;do echo $i; ./search $i "...27...00...8f...27...af...af...af...00";done
for i in de/*;do echo $i; ./search $i "...27...00...8f...27...af...af...af...00...00...00...af...0c...af...8f...03...27";done
rm -rf de
rm -rf xi
