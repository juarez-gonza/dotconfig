#!/usr/bin/env bash

imgdir="${HOME}/Wallpapers"
dwmdir="${HOME}/.config/dwm-6.2"
waldir="${HOME}/.config/wal" 

function runwal()
{
	wal -i ${imgdir}
}

# script to adapt config.def.h to pywal
# the original config.def.h is kept so don't worry
function adapt2dwm()
{
	./${waldir}/adapt2dwm.py
}

# depends on: dwm installed locally on ${HOME}/.local/bin
# otherwise sudo or root permissions are needed
# and automating a script with suid root or guid wheel
# is a BAD idea.
function compiledwm()
{
	pushd ${dwmdir}
	make clean install
	popd
}

# depends on: xdotool package installed, and pidof which wal already needs
# simulate dwm quit
# loop dwm in .xinitrc to make it seem like a reset
# and keep applications open
function simulatekey()
{
	if [ ! -z $(pidof Xorg) ]
	then
		xdotool key super+shift+q
	fi
}

runwal
adapt2dwm
compiledwm
simulatekey