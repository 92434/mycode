#!/bin/sh

ip_set=(${1//\// })
if ((${#ip_set[@]}!=2));then
	echo "[$LINENO]:error!"
	exit;
fi

ip_fields_count=4

server_ip=${ip_set[0]}
netmask_bits=${ip_set[1]}

mask_fields=$(($netmask_bits/8))

mask_bound_field_var_bits=$((8-$netmask_bits%8))
mask_bound_field_var_mask=$((2**$mask_bound_field_var_bits-1))
field_var_mask=$((2**8-1))

server_ip_fields=(${server_ip//\./ })
var_ip_fields[0]=0
var_ip_fields[1]=0
var_ip_fields[2]=0
var_ip_fields[3]=0

i=0
for((i=0;i<$mask_fields;i++));do
	var_ip_fields[$i]=${server_ip_fields[$i]}
done

function get_ip_field() {
	local field_index=$1
	local ip_filed_mask
	local j

	if (($field_index==$mask_fields));then
		ip_filed_mask=$mask_bound_field_var_mask
	else
		ip_filed_mask=$field_var_mask
	fi

	for((j=0;j<=$ip_filed_mask;j++));do
		local high_mask=$((${server_ip_fields[$field_index]}&(255^$ip_filed_mask)))
		local low_mask="$(($ip_filed_mask&$j))"
		local value="$(($high_mask|$low_mask))"
		var_ip_fields[$field_index]=$value
		if (($field_index==$ip_fields_count-1));then
			local var_ip=
			local k
			for ((k=0;k<$ip_fields_count;k++));do
				var_ip="${var_ip:+$var_ip.}${var_ip_fields[k]}"
			done
			echo "$var_ip"
		fi

		if (($field_index < $ip_fields_count-1));then
			get_ip_field $(($field_index+1))
		fi

	done
}

get_ip_field $i
