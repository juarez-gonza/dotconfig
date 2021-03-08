#!/usr/bin/env sh

links=()
files=()
dotconfig="$1"

function populate()
{
	# "$@" -> each element is separate
	# "$*" -> one element with all arguments
	# both should be called with "" in case
	# arguments have special characters
	for file in "$@"
	do
		cp -R $file $dotconfig
	done
}

function resolvepath()
{
	for link in "$@"
	do
		if [ $link != $dotconfig ]
		then
			files+=" $(realpath $link)"
		fi
	done
}

function main()
{
	links=$(ls -A ./)

	resolvepath $links

	if [ -d $dotconfig ]
	then
		populate $files
	else
		mkdir $dotconfig
		populate $files
	fi
}

if [ -z $1 ]
then
	printf "usage: $0 <dotconfig-folder-name>\n"
	exit 1
fi

main
