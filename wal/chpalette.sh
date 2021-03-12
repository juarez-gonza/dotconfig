#!/usr/bin/env bash

imgdir="${HOME}/Wallpapers"
dwmdir="${HOME}/.config/dwm-6.2"
waldir="${HOME}/.config/wal"
xrsrc="${HOME}/.cache/wal/colors.Xresources"
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

# appends to xrsrc the lines dwm with xrdb patch expects
function c2xrsrc()
{
	${waldir}/c2xrsrc.py
}

# depends on:
#	dwm with xrdb patch
function loadxrdb()
{
	xrdb -load ${xrsrc}
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
