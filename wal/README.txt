So this folder contains some scripts that deal with changing dwm color dynamically with help of pywal
There are some problems with dwm and changing colors "on the fly" since dwm colors are statically defined.

pywal: generates colors for differnet applications out of a wallpaper and puts theme somewhere in ${HOME}/.cache/wal

xdotool: simulates keystrokes (used in ./change_theme.sh to simulate dwm "close" keybind)

pidof: a dependency of pywall used also in a script to check if Xorg is running.
	It's name if pretty self explanatory.

adapt2dwm.py: script that adapts config.def.h to whatever pywall expects.
	The original config.def.h is kept under config.def.h.walunmod.

change_theme.sh: runs the already mentioned adapt2dwm and recompiles dwm
	DWM NEEDS TO BE INSTALLED LOCALLY (${HOME}/.local/bin/) OTHERWISE THIS
	WONT BE DONE BECAUSE OF LACK OF PERMISSIONS.
		This can be done by modifying config.mk prefix to be equal
		to ${HOME}/.local/bin

theme_loop.sh: a loop that can be run from .xinitrc appending & to daemonize it.
	It expects change_theme as a binary
	so it requires you to place a soft link somewhere in the bin/
