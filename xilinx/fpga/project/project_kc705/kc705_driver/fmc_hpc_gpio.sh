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
117	R149.1	FMC_HPC_HA23_N
116	R148.1	FMC_HPC_HA23_P
60	R147.1	FMC_HPC_HA21_N
61	R146.1	FMC_HPC_HA21_P
102	R145.1	FMC_HPC_HA17_CC_N
155	R144.1	FMC_HPC_HA17_CC_P
76	R143.1	FMC_HPC_HA10_N
74	R142.1	FMC_HPC_HA10_P
147	R141.1	FMC_HPC_HA06_N
122	R140.1	FMC_HPC_HA06_P
143	R139.1	FMC_HPC_HA02_N
123	R138.1	FMC_HPC_HA02_P
157	R84.1	FMC_HPC_LA30_N
65	R83.1	FMC_HPC_LA30_P
79	R137.1	FMC_HPC_HA22_N
179	R136.1	FMC_HPC_HA22_P
59	R135.1	FMC_HPC_HA18_N
139	R134.1	FMC_HPC_HA18_P
121	R133.1	FMC_HPC_HA14_N
58	R132.1	FMC_HPC_HA14_P
75	R131.1	FMC_HPC_HA11_N
112	R130.1	FMC_HPC_HA11_P
87	R129.1	FMC_HPC_HA07_N
109	R128.1	FMC_HPC_HA07_P
111	R127.1	FMC_HPC_HA03_N
142	R126.1	FMC_HPC_HA03_P
89	R61.1	FMC_HPC_LA33_N
151	R59.1	FMC_HPC_LA31_N
145	R82.1	FMC_HPC_LA28_N
92	R81.1	FMC_HPC_LA28_P
72	R80.1	FMC_HPC_LA24_N
73	R79.1	FMC_HPC_LA24_P
107	R78.1	FMC_HPC_LA21_N
135	R77.1	FMC_HPC_LA21_P
133	R75.1	FMC_HPC_LA19_P
84	R73.1	FMC_HPC_LA15_P
148	R71.1	FMC_HPC_LA11_P
131	R69.1	FMC_HPC_LA07_P
149	R67.1	FMC_HPC_LA04_P
127	R65.1	FMC_HPC_LA02_P
66	R63.1	FMC_HPC_CLK0_M2C_P
159	R86.1	FMC_HPC_LA32_N
172	R53.1	FMC_HPC_LA22_N
153	R58.1	FMC_HPC_LA31_P
108	R57.1	FMC_HPC_LA29_N
144	R56.1	FMC_HPC_LA29_P
104	R55.1	FMC_HPC_LA25_N
81	R54.1	FMC_HPC_LA25_P
100	R76.1	FMC_HPC_LA19_N
170	R74.1	FMC_HPC_LA15_N
175	R72.1	FMC_HPC_LA11_N
63	R70.1	FMC_HPC_LA07_N
163	R68.1	FMC_HPC_LA04_N
96	R66.1	FMC_HPC_LA02_N
161	R64.1	FMC_HPC_CLK0_M2C_N
128	R62.1	FMC_HPC_PRSNT_M2C_B_LS
158	R52.1	FMC_HPC_LA22_P
126	R51.1	FMC_HPC_LA20_N
138	R49.1	FMC_HPC_LA16_N
68	R50.1	FMC_HPC_LA20_P
166	R47.1	FMC_HPC_LA12_N
168	R48.1	FMC_HPC_LA16_P
88	R46.1	FMC_HPC_LA12_P
80	R45.1	FMC_HPC_LA08_N
132	R44.1	FMC_HPC_LA08_P
106	R43.1	FMC_HPC_LA03_N
97	R42.1	FMC_HPC_LA03_P
169	R41.1	FMC_HPC_LA00_CC_N
85	R40.1	FMC_HPC_LA00_CC_P
125	R39.1	FMC_HPC_CLK1_M2C_N
93	R38.1	FMC_HPC_CLK1_M2C_P
64	R85.1	FMC_HPC_LA32_P
98	R60.1	FMC_HPC_LA33_P
119	R125.1	FMC_HPC_HA19_N
118	R124.1	FMC_HPC_HA19_P
134	R123.1	FMC_HPC_HA15_N
120	R122.1	FMC_HPC_HA15_P
110	R121.1	FMC_HPC_HA12_N
146	R120.1	FMC_HPC_HA12_P
71	R119.1	FMC_HPC_HA08_N
77	R118.1	FMC_HPC_HA08_P
69	R117.1	FMC_HPC_HA04_N
103	R116.1	FMC_HPC_HA04_P
95	R115.1	FMC_HPC_HA00_CC_N
124	R114.1	FMC_HPC_HA00_CC_P
176	R113.1	FMC_HPC_PG_M2C_LS
91	R112.1	FMC_HPC_HA20_N
165	R111.1	FMC_HPC_HA20_P
156	R110.1	FMC_HPC_HA16_N
177	R109.1	FMC_HPC_HA16_P
140	R108.1	FMC_HPC_HA13_N
178	R107.1	FMC_HPC_HA13_P
70	R106.1	FMC_HPC_HA09_N
101	R105.1	FMC_HPC_HA09_P
82	R104.1	FMC_HPC_HA05_N
105	R103.1	FMC_HPC_HA05_P
154	R102.1	FMC_HPC_HA01_CC_N
83	R101.1	FMC_HPC_HA01_CC_P
78	R33.1	FMC_HPC_LA26_N
113	R32.1	FMC_HPC_LA26_P
162	R31.1	FMC_HPC_LA23_N
171	R30.1	FMC_HPC_LA23_P
129	R29.1	FMC_HPC_LA17_CC_N
174	R28.1	FMC_HPC_LA17_CC_P
137	R27.1	FMC_HPC_LA13_N
136	R26.1	FMC_HPC_LA13_P
115	R25.1	FMC_HPC_LA09_N
99	R24.1	FMC_HPC_LA09_P
86	R5.1	FMC_HPC_LA06_P
114	R14.1	FMC_HPC_LA27_N
141	R13.1	FMC_HPC_LA27_P
130	R12.1	FMC_HPC_LA18_CC_N
152	R11.1	FMC_HPC_LA18_CC_P
173	R10.1	FMC_HPC_LA14_N
167	R9.1	FMC_HPC_LA14_P
94	R8.1	FMC_HPC_LA10_N
62	R7.1	FMC_HPC_LA10_P
90	R23.1	FMC_HPC_LA05_N
150	R22.1	FMC_HPC_LA05_P
160	R21.1	FMC_HPC_LA01_CC_N
67	R20.1	FMC_HPC_LA01_CC_P
164	R6.1	FMC_HPC_LA06_N
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
