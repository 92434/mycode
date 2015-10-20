#!/bin/sh

proc_path=/proc/gpio_test

function export_gpio() {
	local gpio=$1

	if [ ! -f $proc_path/$gpio ]; then
		echo $gpio > $proc_path/export 
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
		sleep 0.2;
		echo 1 > $proc_path/$gpio;
		sleep 0.2;
	done
}


function all() {
	local gpios="
148	R149.1	FMC_HPC_HA23_N
149	R148.1	FMC_HPC_HA23_P
150	R147.1	FMC_HPC_HA21_N
151	R146.1	FMC_HPC_HA21_P
152	R145.1	FMC_HPC_HA17_CC_N
153	R144.1	FMC_HPC_HA17_CC_P
154	R143.1	FMC_HPC_HA10_N
155	R142.1	FMC_HPC_HA10_P
156	R141.1	FMC_HPC_HA06_N
157	R140.1	FMC_HPC_HA06_P
158	R139.1	FMC_HPC_HA02_N
159	R138.1	FMC_HPC_HA02_P
160	R84.1	FMC_HPC_LA30_N
161	R83.1	FMC_HPC_LA30_P
162	R137.1	FMC_HPC_HA22_N
163	R136.1	FMC_HPC_HA22_P
164	R135.1	FMC_HPC_HA18_N
165	R134.1	FMC_HPC_HA18_P
166	R133.1	FMC_HPC_HA14_N
167	R132.1	FMC_HPC_HA14_P
168	R131.1	FMC_HPC_HA11_N
169	R130.1	FMC_HPC_HA11_P
170	R129.1	FMC_HPC_HA07_N
171	R128.1	FMC_HPC_HA07_P
172	R127.1	FMC_HPC_HA03_N
173	R126.1	FMC_HPC_HA03_P
174	R61.1	FMC_HPC_LA33_N
175	R59.1	FMC_HPC_LA31_N
176	R82.1	FMC_HPC_LA28_N
177	R81.1	FMC_HPC_LA28_P
178	R80.1	FMC_HPC_LA24_N
179	R79.1	FMC_HPC_LA24_P
116	R78.1	FMC_HPC_LA21_N
117	R77.1	FMC_HPC_LA21_P
118	R75.1	FMC_HPC_LA19_P
119	R73.1	FMC_HPC_LA15_P
120	R71.1	FMC_HPC_LA11_P
121	R69.1	FMC_HPC_LA07_P
122	R67.1	FMC_HPC_LA04_P
123	R65.1	FMC_HPC_LA02_P
124	R63.1	FMC_HPC_CLK0_M2C_P
125	R86.1	FMC_HPC_LA32_N
126	R53.1	FMC_HPC_LA22_N
127	R58.1	FMC_HPC_LA31_P
128	R57.1	FMC_HPC_LA29_N
129	R56.1	FMC_HPC_LA29_P
130	R55.1	FMC_HPC_LA25_N
131	R54.1	FMC_HPC_LA25_P
132	R76.1	FMC_HPC_LA19_N
133	R74.1	FMC_HPC_LA15_N
134	R72.1	FMC_HPC_LA11_N
135	R70.1	FMC_HPC_LA07_N
136	R68.1	FMC_HPC_LA04_N
137	R66.1	FMC_HPC_LA02_N
138	R64.1	FMC_HPC_CLK0_M2C_N
139	R62.1	FMC_HPC_PRSNT_M2C_B_LS
140	R52.1	FMC_HPC_LA22_P
141	R51.1	FMC_HPC_LA20_N
142	R49.1	FMC_HPC_LA16_N
143	R50.1	FMC_HPC_LA20_P
144	R47.1	FMC_HPC_LA12_N
145	R48.1	FMC_HPC_LA16_P
146	R46.1	FMC_HPC_LA12_P
147	R45.1	FMC_HPC_LA08_N
84	R44.1	FMC_HPC_LA08_P
85	R43.1	FMC_HPC_LA03_N
86	R42.1	FMC_HPC_LA03_P
87	R41.1	FMC_HPC_LA00_CC_N
88	R40.1	FMC_HPC_LA00_CC_P
89	R39.1	FMC_HPC_CLK1_M2C_N
90	R38.1	FMC_HPC_CLK1_M2C_P
91	R85.1	FMC_HPC_LA32_P
92	R60.1	FMC_HPC_LA33_P
93	R125.1	FMC_HPC_HA19_N
94	R124.1	FMC_HPC_HA19_P
95	R123.1	FMC_HPC_HA15_N
96	R122.1	FMC_HPC_HA15_P
97	R121.1	FMC_HPC_HA12_N
98	R120.1	FMC_HPC_HA12_P
99	R119.1	FMC_HPC_HA08_N
100	R118.1	FMC_HPC_HA08_P
101	R117.1	FMC_HPC_HA04_N
102	R116.1	FMC_HPC_HA04_P
103	R115.1	FMC_HPC_HA00_CC_N
104	R114.1	FMC_HPC_HA00_CC_P
105	R113.1	FMC_HPC_PG_M2C_LS
106	R112.1	FMC_HPC_HA20_N
107	R111.1	FMC_HPC_HA20_P
108	R110.1	FMC_HPC_HA16_N
109	R109.1	FMC_HPC_HA16_P
110	R108.1	FMC_HPC_HA13_N
111	R107.1	FMC_HPC_HA13_P
112	R106.1	FMC_HPC_HA09_N
113	R105.1	FMC_HPC_HA09_P
114	R104.1	FMC_HPC_HA05_N
115	R103.1	FMC_HPC_HA05_P
57	R102.1	FMC_HPC_HA01_CC_N
58	R101.1	FMC_HPC_HA01_CC_P
59	R33.1	FMC_HPC_LA26_N
60	R32.1	FMC_HPC_LA26_P
61	R31.1	FMC_HPC_LA23_N
62	R30.1	FMC_HPC_LA23_P
63	R29.1	FMC_HPC_LA17_CC_N
64	R28.1	FMC_HPC_LA17_CC_P
65	R27.1	FMC_HPC_LA13_N
66	R26.1	FMC_HPC_LA13_P
67	R25.1	FMC_HPC_LA09_N
68	R24.1	FMC_HPC_LA09_P
69	R5.1	FMC_HPC_LA06_P
70	R14.1	FMC_HPC_LA27_N
71	R13.1	FMC_HPC_LA27_P
72	R12.1	FMC_HPC_LA18_CC_N
73	R11.1	FMC_HPC_LA18_CC_P
74	R10.1	FMC_HPC_LA14_N
75	R9.1	FMC_HPC_LA14_P
76	R8.1	FMC_HPC_LA10_N
77	R7.1	FMC_HPC_LA10_P
78	R23.1	FMC_HPC_LA05_N
79	R22.1	FMC_HPC_LA05_P
80	R21.1	FMC_HPC_LA01_CC_N
81	R20.1	FMC_HPC_LA01_CC_P
82	R6.1	FMC_HPC_LA06_N
83	R17.1	FMC_C2M_PG_LS
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
		local rtn=1

		repeat_print 100 "*"
		echo -e "Is LED($(red $((i/3 + 1)))) on $(red $resistor) blinking?($(red y)/$(red n))"
		echo -e "Goback to test Prev LED?($(red p))"
		echo -e "Exit Test?($(red Q))"

		export_gpio $gpio
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
		unexport_gpio $gpio
		if [ $rtn -eq 2 ];then
			break
		fi	
	done
	python process_data.py "$filename"
	rm $filename
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
