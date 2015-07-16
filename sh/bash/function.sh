#!/bin/sh

function test_redirection {
	local i

	for i in `ls`
	do
		echo "$i --from console"
	done

	exec 100>&1
	exec 1>>hello.txt
	exec 101>&2
	exec 2>&1
	ls nofile
	for i in `ls`
	do
		echo "$i --from file"
	done
	ls nofile1
	exec 1>&100
	exec 100>&-
	exec 2>&101
	exec 101>&-

	exec 100<&0
	exec 0<hello.txt
	read line
	echo "$line"
	exec 0<&100
	exec 100<&-

	echo ----------------------------------
	cat hello.txt
	echo ----------------------------------
	rm hello.txt
	echo "i is $i"
}
#test_redirection

function test_fd() {
	local CONFIG_STATUS=log0
	cat >$CONFIG_STATUS <<_ACEOF
	xiaofei
	xiaofei
	xiaofei
_ACEOF

	cat log0
	rm log0

	exec 5>log1
	{
		echo xi
		echo fe
	} >&5

	5>&1
	>&5-
	cat log1
	rm log1
}
#test_fd

function test_awk_NF() {
	local db=/asdf/asdf\;/sajkj/sadflkjlkjlkjlj/\;/asdf/asdf
	local i
	echo $db |awk -F \; '{for(i = 1; i <= NF; i++) {print $i}}'
}
#test_awk_NF

function test_find_word_in_dir() {
	if [ -z "$1" -o -z "$2" ]; then
		echo "parameter error!"
		return 0
	fi

	local DIR="$1"
	local WORD="$2"

	if [ ${DIR:0:1} != "/" ]; then
		DIR="$(pwd)/$DIR"
	fi

	find "$DIR" -exec grep "$WORD" {} \; -a -ok less {} \;
}
#test_find_word_in_dir $(pwd) xiaofei

function git_cmd_with_work_dir() {
	local CMD="$1"
	local GIT_DIR=${2:+'--git-dir='$2}
	local WORK_DIR=${3:+'--work-tree='$3}

	local OPT="$GIT_DIR $WORK_DIR"

	git $OPT $CMD
}
#git_cmd_with_work_dir $*

function git_delete_file_from_repository() {
	local filename=$1
	if test -z $filename;then
		echo "[$FUNCNAME:$LINENO]no file to delete"
		return;
	fi
	git filter-branch --tree-filter "rm -f $filename" HEAD
	git update-ref -
	git ls-remote .
	rm -rf .git/logs  
	git reflog --all  
	git prune  
	git gc  
}

function git_get_all_commits(){
	local commits=$(git log | grep commit | awk '{print $2}')
	echo "$commits"
}

function show_all_commits() {
	for commit in $(git_get_all_commits);do
		echo "$(git show $commit)"
	done
}

function test_array() {
	local a1 i line size
	a1=()
	i=0

	while read line;
		do a1[((i++))]="$line";
	done<$1

	size=${#a1[*]}
	echo "array size is ${size}"

	for((i=0;i<$size;i++));do
		echo "${a1[$i]}";
	done
}

#test_array function.sh

function test_para() {
	local VAR="w1 w2 w3 w4 w5"

	echo $VAR
	echo ${VAR%% *}
	echo ${VAR#* }
}
#test_para

function test_read() {
	while read line
	do
		if [ -z "$line" ]; then 
			continue
		fi
		
		echo "$line"
	done << EOF
./lib/ld-2.17.so
./lib/ld-linux-armhf.so.3
./lib/libanl-2.17.so
./lib/libanl.so
EOF

	cat > log.txt << EOF
./lib/ld-2.17.so \
./lib/ld-linux-armhf.so.3 \
./lib/libanl-2.17.so \
./lib/libanl.so
EOF
}
#test_read


function test_export(){
	export -n "xiaofei1=0"
	export "xiaofei2=0"
	export "$1=$xiaofei1"
	echo "$xiaofei1"
	[ true ] & export "xiaofei3=1"
}
#test_export "xiaofei"
#echo "hi $xiaofei1"
#echo "hi $xiaofei2"
#echo "hi $xiaofei"
#export -p | grep "xiaofei" >&1
#set |grep "xiaofei" >&1


function test_quota() {
	"$@"	
}
#test_quota echo "xiaofei" "is"

function test_option {
	for xi do
		echo $xi
	done

	for xi do
		echo `expr $xi : "xiao\(.*\)"`
	done

	echo --------------------------

	while getopts xyz: arguments 2>/dev/null; do
		case $arguments in
			x) echo "option x";;
			y) echo "option y";;
			z) echo "option z with arg. $OPTARG";;
			/?) echo "Usage: optdemo [-xy] [-z argment]"; exit 1;;
		esac
		echo "\$OPTIND is $OPTIND \$OPTERR is $OPTERR"
	done
	OPTIND=0
}
#test_option -x -y -z xiaofei -k

function set_var() {
	local x
#	export $1="xiaofei"
	export x="xiaofei"
}

function test_set_var() {
	local x
	set_var x
	echo $x
}
#test_set_var

#echo $x
#$ git filter-branch --tree-filter 'rm -f testme.txt' HEAD  
#Rewrite bb383961a2d13e12d92be5f5e5d37491a90dee66 (2/2)  
#Ref 'refs/heads/master'  
# was rewritten  
#$ git ls-remote .  
#230b8d53e2a6d5669165eed55579b64dccd78d11        HEAD  
#230b8d53e2a6d5669165eed55579b64dccd78d11        refs/heads/master  
#bb383961a2d13e12d92be5f5e5d37491a90dee66        refs/original/refs/heads/master  
#$ git update-ref -d refs/original/refs/heads/master [bb383961a2d13e12d92be5f5e5d37491a90dee66]  
#$ git ls-remote .  
#230b8d53e2a6d5669165eed55579b64dccd78d11        HEAD  
#230b8d53e2a6d5669165eed55579b64dccd78d11        refs/heads/master  
#$ rm -rf .git/logs  
#$ git reflog --all  
#$ git prune  
#$ git gc  
#$ du -hs  
# 84K  


function get_system_bitwidth() {
	expr "$(getconf -a | grep LONG_BIT)" : "LONG_BIT\s*\(.*\)"
}

function get_system_info() {
	echo $(get_system_bitwidth)
}
#get_system_info

function mount_iso() {
	local ISO=$1
	local MOUNT_POINT=$2
	sudo mount -o loop,ro -t iso9660 -r "$ISO" "$MOUNT_POINT"
}

function restart_wifi_ap() {
	sudo ap-hotspot restart
}


function dump_data() {
	local file="$1"
	hexdump -e '"%08.08_ax " 8/1 "%02x " "\n"' "$file"
}

function get_net_data() {
	local interface="$1"
	sudo tcpdump -i "$interface" -An
}
