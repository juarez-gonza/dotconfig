#!/usr/bin/env python
import os
import re

userhome = os.path.expanduser("~")
walcache = os.path.join(userhome, ".cache/wal")
Cfile = os.path.join(walcache, "colors-wal-dwm.h")
Xrsrcfile = os.path.join(walcache, "colors.Xresources")

# translation of C variable names in Cfile to expected Xresource resource
c2xrsrc = \
        {
                "norm_fg": "dwm.normfgcolor",
                "norm_bg": "dwm.normbgcolor",
                "norm_border": "dwm.normbordercolor",

                "sel_fg": "dwm.selfgcolor",
                "sel_bg": "dwm.selbgcolor",
                "sel_border": "dwm.selbordercolor",
        }

# color assigned to C variable in its definition at Cfile
cdefcolor = \
        {
                "norm_fg": None,
                "norm_bg": None,
                "norm_border": None,

                "sel_fg": None,
                "sel_bg": None,
                "sel_border": None,
        }

def xfile_append():
    with open(Xrsrcfile, "a") as xf:
        for key in cdefcolor.keys():
            buf = c2xrsrc[key] + ": " + cdefcolor[key] + "\n"
            xf.write(buf)

# regex used in next function
# not defined inside function because compiling it on every call is not needed
hexpattern = re.compile("(.*)(#[a-fA-F0-9]{6})(.*)")

# extracts the first hex color of format #ffffff in a string
# returns the color if found, empty string otherwise
# @string:  string to be matched with the hexcolor pattern
def getnexthex(string):
    match = hexpattern.match(string)
    if match is not None:
        return match.group(2) # extract hex, group 2 in hexpattern
    return None


# note how this expects one assignment per line
# meaning several assignments separated by comas or statements on the same line
# separated by colens will pass through without being processed
# Luckily file at Cfile passes these requirements
def cfile_extract():
    with open(Cfile, "r") as cf:
        for line in cf:
            for key in cdefcolor.keys():
                if key in line and cdefcolor[key] is None:
                    cdefcolor[key] = getnexthex(line)
                    if cdefcolor[key] is None:
                        raise Exception("Error extracting hex color")

def main():
    try:
        cfile_extract()
        xfile_append()
        exit(0)
    except Exception as err:
        print(err)
        exit(1)
main()
