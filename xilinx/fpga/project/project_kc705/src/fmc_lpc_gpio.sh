#!/bin/sh

proc_path=/proc/gpio_test

function export_gpio() {
	local gpio=$1

	if [ ! -f $proc_path/$gpio ]; then
		echo $gpio > $proc_path/export 
	fi

	if [ -f $proc_path/$gpio ]; then
		echo 1 > $proc_path/$gpio;
	fi
}

function unexport_gpio() {
	local gpio=$1

	if [ -f $proc_path/$gpio ]; then
		echo $gpio > $proc_path/unexport 
	fi
}

function test_gpio() {
	local gpio=$1
	local i

	if [ ! -f $proc_path/$gpio ]; then
		echo "[$FUNCNAME:$LINENO] $proc_path/$gpio!";
		return -1;
	fi

	for((i=0;i<1;i++));do
		echo 0 > $proc_path/$gpio;
		#sleep 0.1;
		read -t 0.1
		echo 1 > $proc_path/$gpio;
		#sleep 0.1;
		read -t 0.1
	done
}


function all() {
	local gpios="
156	R84.1	FMC_LPC_LA30_N
157	R83.1	FMC_LPC_LA30_P
158	R78.1	FMC_LPC_LA21_N
159	R75.1	FMC_LPC_LA19_P
160	R63.1	FMC_LPC_CLK0_M2C_P
161	R76.1	FMC_LPC_LA19_N
162	R74.1	FMC_LPC_LA15_N
163	R64.1	FMC_LPC_CLK0_M2C_N
164	R62.1	FMC_LPC_PRSNT_M2C_B_LS
165	R52.1	FMC_LPC_LA22_P
166	R51.1	FMC_LPC_LA20_N
167	R39.1	FMC_LPC_CLK1_M2C_N
168	R38.1	FMC_LPC_CLK1_M2C_P
169	R85.1	FMC_LPC_LA32_P
170	R60.1	FMC_LPC_LA33_P
171	R33.1	FMC_LPC_LA26_N
172	R31.1	FMC_LPC_LA23_N
173	R30.1	FMC_LPC_LA23_P
174	R29.1	FMC_LPC_LA17_CC_N
175	R28.1	FMC_LPC_LA17_CC_P
176	R14.1	FMC_LPC_LA27_N
177	R13.1	FMC_LPC_LA27_P
178	R12.1	FMC_LPC_LA18_CC_N
179	R11.1	FMC_LPC_LA18_CC_P
	"
	gpio_str=( $gpios )

	local i

	local filename
	#read -p"input $(red filename) for test report:" filename 
	if [ "x$filename" = "x" ];then
		filename="report_$(date +%Y%m%d%H%M%S).txt"
	fi
	echo -e "save test report to $(red $filename)"

	for((i=0;i<${#gpio_str[@]};i+=3));do
		local gpio=${gpio_str[i]}
		local resistor=${gpio_str[i+1]}
		local io=${gpio_str[i+2]}

		export_gpio $gpio
	done

	for((i=0;i<${#gpio_str[@]};i+=3));do
		local gpio=${gpio_str[i]}
		local resistor=${gpio_str[i+1]}
		local io=${gpio_str[i+2]}
		local rtn=1

		repeat_print 100 "*"
		local len=${#gpio_str[@]}
		let len=$len/3
		echo -e "Is LED($(red $((i/3 + 1))/$len)) on $(red $resistor) blinking?($(red y)/$(red n))"
		echo -e "Goback to test Prev LED?($(red p))"
		echo -e "Exit Test?($(red Q))"

		while [ ! $rtn -eq 0 ];do
			process_cmd "$gpio" "$resistor" "$io"
			rtn=$?
			echo
			echo
			echo

			if [ $rtn -eq 1 ]; then
				if [ $i -ge 3 ];then
					((i=$i-6))
					rtn=0
				else
					((i=$i-3))
					echo -e "$(red This is first item!!!)"
				       rtn=0
				fi
			elif [ $rtn -eq 2 ];then
				break
			fi
		done
		if [ $rtn -eq 2 ];then
			break
		fi	
	done
	python process_data.py "$filename" "$filename"

	for((i=0;i<${#gpio_str[@]};i+=3));do
		local gpio=${gpio_str[i]}
		local resistor=${gpio_str[i+1]}
		local io=${gpio_str[i+2]}

		unexport_gpio $gpio
	done
}

function process_cmd() {
	local gpio="$1"
	local resistor="$2"
	local io="$3"
	local c=

	while true;do
		if [ "x$c" = "x" ];then
			test_gpio $gpio
			#sleep 0.2
		elif [ $c = "y" -o $c = "Y" ];then
			echo -e "$io\tsuccess" >> "$filename"
			return 0

		elif [ $c = "n" -o $c = "N" ];then
			echo -e "$io\tfailed" >> "$filename"
			return 0

		elif [ $c = "p" -o $c = "P" ];then
			return 1

		elif [ $c = "Q" ];then
			return 2

		else
			echo -en "Get command: $(red $c), So "
			echo -e "$(red retry)!!!"
		fi
		
		read -n1 -t 0.01 c
	done
}

function red() {
	echo -ne "\033[31m$@\033[0m"
}

function repeat_print() {
	local count=$1
	shift
	local content="$@"
	local i

	for((i=0;i<$count;i++));do
		echo -ne "$content"
	done
	echo
}
