#!/bin/sh

username="xiaofei"
passwd="xiaofei"
database="test.db"

function get_parameter() {
	while getopts d:u:p: arguments 2>/dev/null; do
#		echo "arguments is $arguments"
		case $arguments in
			u)
				export username="$OPTARG"
				;;
			p)
				export passwd="$OPTARG"
				;;
			d)
				export database="$OPTARG"
				;;
			/?)
				echo "Usage:$0-$FUNCNAME [option];"
				echo "\t-u:username"
				echo "\t-p:passwd;"
				echo "\t-d:database"
				return
				;;
		esac
#		echo "\$OPTIND is $OPTIND \$OPTERR is $OPTERR"
	done
	OPTIND=0

}

function add_user() {
	get_parameter $@
	#insert into mysql.user(Host,User,Password) values("localhost","test",password("1234"));
}

function create_db() {
	get_parameter $@

	mysqladmin -u $username -p $passwd create $database
}
create_db
