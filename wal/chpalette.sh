#!/usr/bin/env bash

imgdir="${HOME}/Wallpapers"
dwmdir="${HOME}/.config/dwm-6.2"
waldir="${HOME}/.config/wal"
walxrsrc="${HOME}/.cache/wal/colors.Xresources"
xrsrc="${HOME}/.Xresources"

walflags=()

function processargs()
{
	if [ ! -z $1 ]
	then
		imgdir=$(realpath $1)
	fi
	[ ! -z $2 ] && [ $2 == "--iterative" ] && walflags+="--iterative "
}

function runwal()
{
	wal ${walflags} -i ${imgdir}
}

# appends to walxrsrc the lines dwm with xrdb patch expects
function c2xrsrc()
{
	${waldir}/a.out
}

# depends on:
#	dwm with xrdb patch
function loadxrdb()
{
	xrdb -load ${xrsrc}
	xrdb -merge ${walxrsrc}
}

# simulates key press to apply xrsrc colors
# depends on:
#	dwm with xrdb patch
#	xdotool package installed,
#	and pidof which wal already needs
function simulatekey()
{
	if [ ! -z $(pidof Xorg) ]
	then
		xdotool key super+F5
	fi
}

processargs $1 $2
runwal
c2xrsrc
loadxrdb
simulatekey
