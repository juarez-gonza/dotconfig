So this folder contains some scripts that deal with changing dwm color dynamically with help of pywal
There are some problems with dwm and changing colors "on the fly" since dwm colors are statically defined.

pywal: generates colors for differnet applications out of a wallpaper and puts theme somewhere in ${HOME}/.cache/wal

dwm's xrdb patch: this patch allows a non recompilation method for changing colors "on the fly". Otherwise recompilation
	is needed and not only that's a bit trickier but also slower. I should know cause that was my first approach
	to this changing of colors deal.

xdotool: simulates keystrokes. Used in scripts or command line.

pidof: a dependency of pywall used also in a script to check if Xorg is running.
	It's name if pretty self explanatory.

c2xrsrc.py: script that appends to the created by pywal colors.Xresources, the lines needed for dwm's xrdb patch to work
c2xrsrc.c: does the same than c2xrsrc.py but a LOT (10 times in my machine) faster.
	For alternating between these 2 just change the c2xrsrc function in chpalette.sh
	(remember c2xrsrc.c should be compiled and the output of that is what should be loaded)

chpalette.sh: runs the already mentioned c2xrsrc and the keystroke simulation to refresh dwm colors

looppalette.sh: a loop that can be run from .xinitrc appending & to daemonize it.
	It expects chpalette somewhere in the bins like /usr/local/bin or ${HOME}/.local/bin
	so it requires you to place a link to chpalette.sh
