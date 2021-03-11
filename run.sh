#!/usr/bin/env bash

# all configs will be copied to this folder
dotconfig="./dotconfig/"
github="https://github.com/juarez-gonza/dotconfig.git"

function creatdotconfig()
{
	if [ -d $dotconfig ]
	then
		deldotconfig
	fi
		mkdir $dotconfig
}

function deldotconfig()
{
	if [ -d $dotconfig ]
	then
		rm -rf $dotconfig
	fi
}

function callgather()
{
	printf "Resolving links and copying files...\n"

	./gather.sh $dotconfig

	printf "Going inside dotconfig folder"
}

function intodotconfig()
{
	pushd $dotconfig
}

function outofdotconfig()
{
	popd
}

function githubenter()
{
	git clone $github $dotconfig
}

function githubout()
{
	git add --all
	git status
	git commit -m "$(date)"
	git push -u origin main
}

creatdotconfig
githubenter
callgather
intodotconfig

githubout
outofdotconfig
##  deldotconfig
