#!/bin/sh

function get_defined() {
	local l=$1;
	code="echo \$$l";
	eval $code;
}

function env_add_value() {
	local name=$1
	local value=$2

	local values="$(get_defined "$name")"
	local t=($(echo "$values" | sed 's/ /&/g' | sed 's/,/ /g'))

	local item=
	local path_exist=0
	local i
	local ret=0
	local status=

	for((i=0;i<${#t[@]};i+=1));do
		item="${t[i]}"
		item=$(echo $item | sed 's/&/ /g')
		if [ "$item" = "$value" ];then
			path_exist=1;
			break;
		fi
	done

	if [ $path_exist -eq 1 ];then
		status="in"
		ret=1
	else
		status="not in"
		code="echo \${$name:+$values,}$value"
		export $name=$(eval "$code")
	fi

	#echo "$value is $status $name!"
	#echo "$name is $(get_defined $name)"
}

function main() {
	while read e_cs_file;do
		while read line;do
			env_add_value $(echo "$line" | sed 's:^export \([^=]*\)=\(.*\)$:\1 \2:g')
		done < "$e_cs_file"
	done < e_cs
}

main
