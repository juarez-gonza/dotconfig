#! /bin/sh

#userresources=$HOME/.Xresources
#usermodmap=$HOME/.Xmodmap
#sysresources=/etc/X11/xinit/.Xresources
#sysmodmap=/etc/X11/xinit/.Xmodmap
## merge in defaults and keymaps
#
#if [ -f $sysresources ]; then
#    xrdb -merge $sysresources
#fi
#
#if [ -f $sysmodmap ]; then
#    xmodmap $sysmodmap
#fi
#
#if [ -f "$userresources" ]; then
#    xrdb -merge "$userresources"
#fi
#
#if [ -f "$usermodmap" ]; then
#    xmodmap "$usermodmap"
#fi
#
## start some nice programs
#
#if [ -d /etc/X11/xinit/xinitrc.d ] ; then
# for f in /etc/X11/xinit/xinitrc.d/?*.sh ; do
#  [ -x "$f" ] && . "$f"
# done
# unset f
#fi

# not using nitrogen nor feh since dwm/xrdb/pywal looppalette is down there
# nitrogen --restore &
picom &

setxkbmap latam

looppalette &
while true
do
	# dwmstatus (dwm status bar in C) and dwm
	dwmstatus 2>&1 >/dev/null &
	dwm 2>&1 >/dev/null
	# prevent spamming of dwm status (there is a patch for dwm for dealing with this but meh)
	killall dwmstatus
done

exec dwm
