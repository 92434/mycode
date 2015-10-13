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

	if [ ! -f $proc_path/$gpio ]; then
		echo "[$FUNCNAME:$LINENO] $proc_path/$gpio!";
		return -1;
	fi

	for((i=0;i<2;i++));do
		echo 1 > $proc_path/$gpio;
		sleep 0.2;
		echo 0 > $proc_path/$gpio;
		sleep 0.2;
		#echo 2 > $proc_path/$gpio;
		#cat $proc_path/$gpio;
		#sleep 0.2;
	done
}


function test_all() {
	local gpios="
	117
	116
	22
	95
	15
	108
	132
	104
	135
	157
	142
	133
	179
	53
	126
	49
	46
	48
	103
	59
	143
	152
	74
	106
	33
	112
	130
	175
	32
	44
	119
	96
	118
	91
	55
	162
	29
	148
	70
	89
	123
	150
	81
	38
	84
	146
	159
	171
	61
	149
	138
	45
	88
	105
	73
	24
	176
	92
	27
	20
	66
	169
	155
	87
	78
	156
	43
	37
	147
	161
	127
	158
	54
	141
	100
	113
	165
	167
	26
	164
	131
	36
	57
	90
	42
	39
	82
	40
	129
	168
	69
	163
	75
	140
	77
	134
	120
	18
	41
	31
	122
	121
	93
	125
	47
	107
	23
	137
	136
	111
	52
	177
	115
	178
	21
	25
	94
	19
	16
	144
	50
	62
	97
	79
	170
	80
	85
	28
	154
	99
	30
	98
	67
	76
	63
	109
	64
	51
	35
	102
	128
	86
	139
	153
	101
	71
	166
	60
	114
	68
	56
	58
	83
	151
	17
	160
	145
	72
	110
	65
	"

	local gpio_str="
	gpio2_tri_io[1]
	gpio2_tri_io[0]
	gpio2_2_tri_io[5]
	gpio2_2_tri_io[7]
	gpio_2_tri_io[2]
	gpio_1_tri_io[11]
	gpio_3_tri_io[0]
	gpio2_2_tri_io[30]
	gpio_1_tri_io[24]
	gpio2_tri_io[16]
	gpio_1_tri_io[20]
	gpio2_tri_io[19]
	gpio_tri_io[9]
	gpio2_2_tri_io[11]
	gpio2_tri_io[17]
	gpio_tri_io[31]
	gpio2_2_tri_io[4]
	gpio2_1_tri_io[1]
	gpio2_tri_io[10]
	gpio2_2_tri_io[3]
	gpio2_2_tri_io[31]
	gpio_2_tri_io[29]
	gpio2_2_tri_io[27]
	gpio_2_tri_io[26]
	gpio_2_tri_io[28]
	gpio_1_tri_io[19]
	gpio2_1_tri_io[7]
	gpio_tri_io[4]
	gpio_1_tri_io[22]
	gpio_1_tri_io[28]
	gpio2_2_tri_io[28]
	gpio2_2_tri_io[29]
	gpio_2_tri_io[24]
	gpio_1_tri_io[12]
	gpio_1_tri_io[7]
	gpio_tri_io[14]
	gpio_tri_io[0]
	gpio_1_tri_io[5]
	gpio_tri_io[2]
	gpio_2_tri_io[18]
	gpio2_2_tri_io[12]
	gpio_tri_io[11]
	gpio_tri_io[23]
	gpio_tri_io[1]
	gpio_2_tri_io[25]
	gpio_1_tri_io[21]
	gpio_2_tri_io[4]
	gpio_1_tri_io[8]
	gpio_2_tri_io[0]
	gpio_tri_io[21]
	gpio_tri_io[7]
	gpio2_2_tri_io[9]
	gpio_tri_io[8]
	gpio_2_tri_io[17]
	gpio_tri_io[13]
	gpio2_tri_io[8]
	gpio_tri_io[10]
	gpio2_tri_io[25]
	gpio_1_tri_io[16]
	gpio2_2_tri_io[19]
	gpio_tri_io[17]
	gpio_tri_io[19]
	gpio_tri_io[16]
	gpio_2_tri_io[16]
	gpio_1_tri_io[6]
	gpio_2_tri_io[19]
	gpio_2_tri_io[20]
	gpio_tri_io[20]
	gpio_tri_io[15]
	gpio2_tri_io[24]
	gpio2_tri_io[18]
	gpio2_2_tri_io[10]
	gpio_2_tri_io[21]
	gpio2_tri_io[6]
	gpio2_tri_io[5]
	gpio_1_tri_io[9]
	gpio2_tri_io[9]
	gpio_2_tri_io[27]
	gpio_1_tri_io[23]
	gpio2_2_tri_io[22]
	gpio2_2_tri_io[23]
	gpio2_2_tri_io[20]
	gpio_2_tri_io[3]
	gpio2_tri_io[21]
	gpio2_tri_io[20]
	gpio_tri_io[26]
	gpio2_2_tri_io[6]
	gpio_1_tri_io[27]
	gpio2_1_tri_io[0]
	gpio_tri_io[29]
	gpio_1_tri_io[31]
	gpio_tri_io[30]
	gpio2_2_tri_io[21]
	gpio_2_tri_io[1]
	gpio2_2_tri_io[24]
	gpio_2_tri_io[5]
	gpio_1_tri_io[10]
	gpio_3_tri_io[4]
	gpio_3_tri_io[1]
	gpio_2_tri_io[30]
	gpio_1_tri_io[13]
	gpio_tri_io[22]
	gpio_1_tri_io[1]
	gpio_tri_io[6]
	gpio_1_tri_io[15]
	gpio_1_tri_io[14]
	gpio2_2_tri_io[1]
	gpio2_1_tri_io[24]
	gpio_1_tri_io[25]
	gpio_2_tri_io[31]
	gpio_1_tri_io[18]
	gpio_1_tri_io[2]
	gpio_tri_io[5]
	gpio_1_tri_io[17]
	gpio_tri_io[18]
	gpio_1_tri_io[30]
	gpio2_2_tri_io[8]
	gpio2_1_tri_io[6]
	gpio_tri_io[3]
	gpio_tri_io[12]
	gpio2_2_tri_io[13]
	gpio_1_tri_io[26]
	gpio_2_tri_io[14]
	"

	gpio_str=( $gpio_str )

	local i

	#for i in $gpios;do
	#	export_gpio $i
	#	test_gpio $i
	#	unexport_gpio $i
	#done

	for((i=0;i<${#gpio_str[@]};i++));do
		local rtn=1

		while [ ! $rtn -eq 0 ];do
			process_cmd
			rtn=$?
			echo
			echo
			echo

			if [ $rtn -eq 1 ]; then
				return
			elif [ $rtn -eq 2 ];then
				rtn=0
				continue
			elif [ $rtn -eq 3 ];then
				continue
			fi
		done
		repeat_print 100 "*"
	done
}

function process_cmd() {
	echo -e "Press $(red s) to start test $(red ${gpio_str[$i]})!"
	echo -e "Press $(red k) to $(red skip)!"
	echo -e "Press $(red q) to $(red exit)!"
	read -n1 -p ">" c
	echo

	if [ $c = "S" -o $c = "s" ];then
		echo -e "start test ${gpio_str[$i]}..."
		return 0
	else
		echo -en "\rGet command: $(red $c), So "
		if [ $c = "q" ];then
			echo -e "$(red exit)!!!"
			return 1
		elif [ $c = "k" ];then
			echo -e "$(red skip)!!!"
			return 2
		else
			echo -e "$(red retry)!!!"
			return 3
		fi
	fi
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
