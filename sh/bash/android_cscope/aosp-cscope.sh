#!/bin/sh

function red() {
	echo -ne "\033[31m$@\033[0m"
}

project_repo_list="project.repo.list"

function gen_project_repo_list() {
	exec 101>&1-
	exec 1>"$project_repo_list"
	#repo list | sed "s/^\([^ ]*\).*/\1/g" | awk 'BEGIN {FS = "/"} {print $1}' | sort | uniq
	repo list | sed "s/^\([^ ]*\).*/\1/g" | sort | uniq
	exec 1>&101-
}

function gen_cscope_for_project() {
	local current_dir=$(pwd)

	local total_projects=$(wc -l project.repo.list | awk '{print $1}')
	echo $(red "total_projects $total_projects");
	local current_index=0

	while read line;do
		local target_dir="$current_dir/$line"
		local percent=$((current_index * 100 / total_projects))

		cd "$target_dir"

		echo $(red $(printf "[%d%%]enter $target_dir\n" "$percent"));
		((current_index++))

		if [ -d cscope ]; then
			continue;
		fi

		tags.sh prepare
		tags.sh cscope
		tags.sh env
	done < "$project_repo_list"

	cd "$current_dir"

	if [ ! -d cscope ]; then
		echo $(red "create file cscope/cscope.files");
		tags.sh prepare
	fi

	if [ ! -f e_cs ]; then
		echo $(red "create file e_cs");
		exec 101>&1-
		exec 1>e_cs
		while read line;do
			echo ". $line/e_cs"
		done < "$project_repo_list"
		exec 1>&101-
	fi


	echo $(red $(printf "[%d%%]ok!!!\n" "100"));
}

function main() {
	gen_project_repo_list
	gen_cscope_for_project
}

main $@
