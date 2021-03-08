#!/usr/bin/env sh

# all configs will be copied to this folder
dotconfig="./dotconfig/"
github="https://github.com/juarez-gonza/dotcondig.git"

function callgather()
{
	printf "Resolving links and copying files...\n"

	. ./gather.sh $dotconfig

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

function githubstuff()
{
	git init
	git add .
	git commit -m "$(date)"
	git branch -M main
	git remote add origin "$github"
	git push -u origin main
}

callgather
intodotconfig
githubstuff
outofdotconfig
