#!/bin/sh -f
#file: mkcscope
#function: make cscope database and ctags database
#author: wikijone@gmail.com
#date: 20110527
 
mode=0
ignore=
type=
while read line
do
	if [ -z "$line" ]; then
		continue
	fi

	if [ "$line" = ":ignore" ]; then
		mode=1
		continue
	fi

	if [ "$line" = ":type" ]; then
		mode=2
		continue
	fi

	case $mode in
		1)
		if [ -z "$ignore" ]; then
			ignore="$line"
		else
			ignore="$ignore -o $line"
		fi
		;;

		"")
		;;

		2)
		if [ -z "$type" ]; then
			type="$line"
		else
			type="$type -o $line"
		fi
		;;

		"")
		;;
	esac
done < .filetype

if [ -z "$type" ]; then
	type="-name *.java -o -name *.cpp -o -name *.cc -o -name *.[ch] -o -name *.xml -o -name *.aidl -o -name *.py"
fi
type="( $type ) -a ( -type f )"

if [ -z "$ignore" ]; then
	ignore="-name .git -o -name test -o -name tests -o -name out"
fi
ignore="( $ignore )"

echo "type:$type"
echo "ignore:$ignore"

tags_src_dir="${tags_src_dir:=$(pwd)}"
cscopedb="$(pwd)/cscope"

prepare()
{
	if [ ! -d $tags_src_dir ]; then
		echo "no tags_src_dir set"
		echo "please set tags_src_dir at your .bashrc file"
		exit 1
	else
		echo "tags_src_dir:$tags_src_dir"
		if [ -d $cscopedb ]; then
			echo "$cscopedb exist already"
			if [ -e $cscopedb/cscope.files ]; then
				echo "file $cscopedb/cscope.files exist"
				#rm $cscopedb/cscope.files
				
			fi
		else
			mkdir $cscopedb
			echo "make directory $cscopedb"
		fi
	fi
	all_source | sort | uniq > $cscopedb/cscope.files
	cat $cscopedb/cscope.files | sed 's/"//g' > $cscopedb/ctags.files
	lines=$(wc -l $cscopedb/cscope.files | awk '{ printf $1 }')
	echo "find $lines files totaly"
}

all_source()
{
	find -H $tags_src_dir $ignore -prune -o $type -printf "\"%p\"\n"
}
 
docscope()
{
	echo "Now begin build cscope cross-index file"
	start=`date +%s`
	cscope -b -k -q -i $cscopedb/cscope.files -f $cscopedb/cscope.out
	end=`date +%s`
	let "elapse=$end-$start"
	if [ $? -eq 0 ]; then
		echo "make cscope database file with total time ($elapse) seconds"
		size=$(du $cscopedb/cscope.out -h | awk '{ printf $1 }')
		echo "($cscopedb/cscope.out):$size"
	fi  
}

dotags()
{
	echo "Now begin build tags file"
	start=`date +%s`
	ctags --fields=+afiKlmnsSzt --c++-kinds=+p --extra=+q -L $cscopedb/ctags.files -f $cscopedb/tags
	end=`date +%s`
	let "elapse=$end-$start"
	if [ $? -eq 0 ]; then
		echo "make ctags database file with total time ($elapse) seconds"
		size=$(du $cscopedb/tags -h | awk '{ printf $1 }')
		echo "($cscopedb/tags):$size"
	fi  
}
 
usage() 
{
	echo "Usages:"
	echo "$0 [option]"
	echo "[option] can be this:"
	echo "	  prepare : make filelist"
	echo "	  cscope  : make cscope database only"
	echo "	  tags	: make ctags database only"
	echo "	  all	 : make two databases"
	echo "	  env	 : make environment"
}

write_env()
{
	echo export CSCOPE_DB=$cscopedb/cscope.out >e_cs
	echo export CTAGS_DB=$cscopedb/tags >>e_cs
} 
 
case $1 in
	"prepare")
	prepare
	;;

	"cscope")
	docscope
	;;
																		   
	"tags")
	dotags
	;;  
 
	"env")
	write_env
	;;

	"all")
	prepare
	docscope
	dotags
	write_env
	;;

	"")
	usage
	;;
esac	 

