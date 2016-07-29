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
164	R84.1	FMC_LPC_LA30_N
111	R83.1	FMC_LPC_LA30_P
165	R61.1	FMC_LPC_LA33_N
122	R59.1	FMC_LPC_LA31_N
136	R82.1	FMC_LPC_LA28_N
158	R81.1	FMC_LPC_LA28_P
149	R80.1	FMC_LPC_LA24_N
140	R79.1	FMC_LPC_LA24_P
151	R78.1	FMC_LPC_LA21_N
153	R77.1	FMC_LPC_LA21_P
175	R75.1	FMC_LPC_LA19_P
133	R73.1	FMC_LPC_LA15_P
118	R71.1	FMC_LPC_LA11_P
154	R69.1	FMC_LPC_LA07_P
127	R67.1	FMC_LPC_LA04_P
168	R65.1	FMC_LPC_LA02_P
112	R63.1	FMC_LPC_CLK0_M2C_P
146	R86.1	FMC_LPC_LA32_N
176	R53.1	FMC_LPC_LA22_N
162	R58.1	FMC_LPC_LA31_P
170	R57.1	FMC_LPC_LA29_N
121	R56.1	FMC_LPC_LA29_P
161	R55.1	FMC_LPC_LA25_N
131	R54.1	FMC_LPC_LA25_P
178	R76.1	FMC_LPC_LA19_N
173	R74.1	FMC_LPC_LA15_N
169	R72.1	FMC_LPC_LA11_N
124	R70.1	FMC_LPC_LA07_N
144	R68.1	FMC_LPC_LA04_N
113	R66.1	FMC_LPC_LA02_N
156	R64.1	FMC_LPC_CLK0_M2C_N
148	R62.1	FMC_LPC_PRSNT_M2C_B_LS
139	R52.1	FMC_LPC_LA22_P
114	R51.1	FMC_LPC_LA20_N
160	R49.1	FMC_LPC_LA16_N
115	R50.1	FMC_LPC_LA20_P
110	R47.1	FMC_LPC_LA12_N
130	R48.1	FMC_LPC_LA16_P
171	R46.1	FMC_LPC_LA12_P
142	R45.1	FMC_LPC_LA08_N
143	R44.1	FMC_LPC_LA08_P
128	R43.1	FMC_LPC_LA03_N
157	R42.1	FMC_LPC_LA03_P
117	R41.1	FMC_LPC_LA00_CC_N
159	R40.1	FMC_LPC_LA00_CC_P
123	R39.1	FMC_LPC_CLK1_M2C_N
152	R38.1	FMC_LPC_CLK1_M2C_P
145	R85.1	FMC_LPC_LA32_P
135	R60.1	FMC_LPC_LA33_P
166	R33.1	FMC_LPC_LA26_N
150	R32.1	FMC_LPC_LA26_P
125	R31.1	FMC_LPC_LA23_N
126	R30.1	FMC_LPC_LA23_P
132	R29.1	FMC_LPC_LA17_CC_N
108	R28.1	FMC_LPC_LA17_CC_P
134	R27.1	FMC_LPC_LA13_N
109	R26.1	FMC_LPC_LA13_P
179	R25.1	FMC_LPC_LA09_N
177	R24.1	FMC_LPC_LA09_P
147	R5.1	FMC_LPC_LA06_P
137	R14.1	FMC_LPC_LA27_N
138	R13.1	FMC_LPC_LA27_P
163	R12.1	FMC_LPC_LA18_CC_N
174	R11.1	FMC_LPC_LA18_CC_P
119	R10.1	FMC_LPC_LA14_N
172	R9.1	FMC_LPC_LA14_P
116	R8.1	FMC_LPC_LA10_N
141	R7.1	FMC_LPC_LA10_P
129	R23.1	FMC_LPC_LA05_N
155	R22.1	FMC_LPC_LA05_P
167	R21.1	FMC_LPC_LA01_CC_N
120	R20.1	FMC_LPC_LA01_CC_P
107	R6.1	FMC_LPC_LA06_N
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
