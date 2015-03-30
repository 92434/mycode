#!/bin/sh

sql_cmd="./out/bin/sqlite3"
sql_option="-batch"
database="xiaofei.db"
table_name="main"
columns="domain username passwd"
cvs_name="dump.cvs"
sql_name="dump.sql"


function get_columns_value() {
	while getopts d:u:p: arguments 2>/dev/null; do
#		echo "arguments is $arguments"
		case $arguments in
			d)
				export domain="$OPTARG"
				;;
			u)
				export username="$OPTARG"
				;;
			p)
				export passwd="$OPTARG"
				;;
			/?)
				echo "Usage:$0-$FUNCNAME [option];"
				echo "\t-d:domain"
				echo "\t-u:username"
				echo "\t-p:passwd;"
				return
				;;
		esac
#		echo "\$OPTIND is $OPTIND \$OPTERR is $OPTERR"
	done
	OPTIND=0
}

#des="domain text not null, username text not null, passwd text not null"
function gen_create_table_columns_des() {
	local des constrain
	constrain="text not null"
	for i do
		des="${des:+"$des, "}$i $constrain"
	done
	echo "$des"
}

#des="domain =='domain' and username =='username' and passwd == 'passwd'" |null is ==''
function gen_select_item_filter_des() {
	local domain username passwd des
	domain=$1
	username=$2
	passwd=$3
	des="${des:+"$des and "}domain=='$domain'"
	des="${des:+"$des and "}username=='$username'"
	des="${des:+"$des and "}passwd=='$passwd'"
	echo "$des"
}
#gen_select_item_filter_des "www.qq.com" "553514546" "xf22001"
#gen_select_item_filter_des "" "553514546" "xf22001"
#gen_select_item_filter_des "www.qq.com" "" "xf22001"
#gen_select_item_filter_des "www.qq.com" "553514546" ""

#des="domain like %domain% and username like %username% and passwd like %passwd%"
function gen_select_item_filter_des_wildcard() {
	local domain username passwd des
	domain=$1
	username=$2
	passwd=$3

	if test -n "$domain";then
		des="${des:+"$des and "}domain like '%$domain%'"
	fi
	if test -n "$username";then
		des="${des:+"$des and "}username like '%$username%'"
	fi
	if test -n "$passwd";then
		des="${des:+"$des and "}passwd like '%$passwd%'"
	fi
	echo "$des"
}

#des="'domain', 'username', 'passwd'" |null is ''
function gen_insert_item_value_des() {
	local domain username passwd des
	domain="'$1'"
	username="'$2'"
	passwd="'$3'"
	des="${des:+"$des, "}${domain:-"''"}"
	des="${des:+"$des, "}${username:-"''"}"
	des="${des:+"$des, "}${passwd:-"''"}"
	echo "$des"
}
#gen_insert_item_value_des "www.qq.com" "553514546" "xf22001"
#gen_insert_item_value_des "" "553514546" "xf22001"
#gen_insert_item_value_des "www.qq.com" "" "xf22001"
#gen_insert_item_value_des "www.qq.com" "553514546" ""

#des="domain, username, passwd"
function gen_order_columns_des() {
	local des
	for i do
		des="${des:+"$des, "}$i"
	done
	echo "$des"
}

#des="domain=='domain', username=='username', passwd=='passwd'"
function gen_set_item_des() {
	local domain username passwd des
	domain=$1
	username=$2
	passwd=$3
	if test -n "$domain";then
		des="${des:+"$des, "}domain=='$domain'"
	fi
	if test -n "$domain";then
		des="${des:+"$des, "}username=='$username'"
	fi
	if test -n "$domain";then
		des="${des:+"$des, "}passwd=='$passwd'"
	fi
	echo "$des"
}

function create_table() {
	local table_columns_des

	set $columns
	table_columns_des=$(gen_create_table_columns_des $*)
	$sql_cmd $sql_option $database "create table if not exists $table_name( $table_columns_des )"
}

function insert_item() {
	local item_filter_des item_value_des
	local domain username passwd
	
	get_columns_value $*
	if test -z "$domain$username$passwd";then
		return
	fi

	item_filter_des=$(gen_select_item_filter_des  "$domain" "$username" "$passwd")
	item_value_des=$(gen_insert_item_value_des "$domain" "$username" "$passwd")
	$sql_cmd $sql_option $database "delete from $table_name where $item_filter_des"
	$sql_cmd $sql_option $database "insert into $table_name values($item_value_des)"
}

function update_index() {
	local index_column="domain"
	local index_name="domain_index"
	$sql_cmd $sql_option $database "create index if not exists $index_name on $table_name ($index_column)"
	$sql_cmd $sql_option $database "reindex $index_name"
}

function query_item() {
	local item_filter_des item_value_des
	local domain username passwd

	get_columns_value $*
	if test -n "$domain$username$passwd";then
		item_filter_des=$(gen_select_item_filter_des_wildcard "$domain" "$username" "$passwd")
	fi
	item_filter_des=${item_filter_des:+"where $item_filter_des"}
	$sql_cmd $sql_option $database ".mode column" ".header on" ".width 50 30 30" "select * from $table_name $item_filter_des order by $(gen_order_columns_des $columns)"
	$sql_cmd $sql_option $database ".mode column" ".header on" ".width 50 30 30" "select count(*) from $table_name $item_filter_des"
}

function delete_item() {
	local item_filter_des item_value_des
	local domain username passwd

	get_columns_value $*
	item_filter_des=$(gen_select_item_filter_des "$domain" "$username" "$passwd")
	item_filter_des=${item_filter_des:+"where $item_filter_des"}
	$sql_cmd $sql_option $database "delete from $table_name $item_filter_des"
}

function delete_items() {
	local item_filter_des item_value_des
	local domain username passwd

	get_columns_value $*
	if test -z "$domain$username$passwd";then
		echo "error($LINENO)no item filter for deleting!!!"
		return
	fi
	item_filter_des=$(gen_select_item_filter_des_wildcard "$domain" "$username" "$passwd")
	item_filter_des=${item_filter_des:+"where $item_filter_des"}
	$sql_cmd $sql_option $database "delete from $table_name $item_filter_des"
}

function update_item() {
	local para1 para2 domain username passwd item_filter_des item_content_des result_count
	
	echo -ne "which item to change:"
	read para1

	get_columns_value $para1
	item_filter_des=$(gen_select_item_filter_des "$domain" "$username" "$passwd")
	item_filter_des=${item_filter_des:+"where $item_filter_des"}
	$sql_cmd $sql_option $database ".mode column" ".header on" ".width 50 30 30" "select * from $table_name $item_filter_des order by $(gen_order_columns_des $columns)"
	$sql_cmd $sql_option $database ".mode column" ".header on" ".width 50 30 30" "select count(*) from $table_name $item_filter_des"
	result_count=$({
		$sql_cmd $sql_option $database ".width 50 30 30" "select count(*) from $table_name $item_filter_des"
		})
	if test "$result_count" == "0"; then
		echo "error($LINENO)no item to change!!!"
		return;
	fi

	domain=
	username=
	passwd=
	echo -ne "change item to:"
	read para2
	get_columns_value $para2
	if test -z "$domain$username$passwd";then
		echo "error($LINENO):no value to set!!!"
		return
	fi

	item_content_des=$(gen_set_item_des "$domain" "$username" "$passwd")
	$sql_cmd $sql_option $database "select count(*) from $table_name $item_filter_des" "update $table_name set $item_content_des $item_filter_des"
}

function drop_table() {
	$sql_cmd $sql_option $database "drop table if exists $table_name"
}

function check() {
	$sql_cmd $sql_option $database ".schema" ".tables" ".dump" ".show"
	$sql_cmd $sql_option -separator "*" $database "select * from $table_name order by $(gen_order_columns_des $columns)"
}

function import_cvs() {
	create_table
	$sql_cmd $sql_option -separator "*" $database ".import $cvs_name $table_name"
	query_item
}

function export_cvs() {
	$sql_cmd $sql_option -separator "*" $database "select * from $table_name order by $(gen_order_columns_des $columns)" | uniq
	$sql_cmd $sql_option -separator "*" $database "select * from $table_name order by $(gen_order_columns_des $columns)" | uniq >"$cvs_name"

	$sql_cmd $sql_option $database ".dump" >$sql_name
#	$sql_cmd $sql_option $database ".read $sql_name"
#	$sql_cmd $sql_option $database  <$sql_name
}

function merge_cvs() {
	import_cvs
	export_cvs
	rm $database
	import_cvs
}

function mytest() {
	create_table
	insert_item -d "www.qq.com" -u "553514546" -p "xf22001"
	insert_item -u "553514546" -p "xf22001"
	insert_item -d "www.qq.com" -p "xf22001"
	insert_item -d "www.qq.com" -u "553514546"
	query_item -u "553514546"
	query_item -d "www.qq.com"
	query_item -p "xf22001"
	query_item -u "553514546" -d "www.qq.com" -p "xf22001"
	query_item
	check
	export_cvs
	delete_item -d "www.qq.com" -u "553514546"
	query_item
	rm "$database"
	import_cvs
	query_item
}
