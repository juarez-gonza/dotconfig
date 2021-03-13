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

# regex used in next function
# not defined inside function because compiling it on every call is not needed
expr = re.compile("(.*)(#[a-fA-F0-9]{6})(.*)")
# extracts the first hex color of format #ffffff in a string
# returns the color if found, empty string otherwise
def extract_hex_color(string):
    match = expr.match(string)
    if match is not None:
        return match.group(2) # extract hex, group 2 in expr
    return None

# note how this expects one assignment per line
# meaning, several assignments separated by comas or statements on the same line
# separated by colens will pass through without being processed
# Luckily file at Cfile passes these requirements
def main():
    with open(Cfile, "r") as cf:
        with open(Xrsrcfile, "a") as xf:

            for line in cf:
                for key in cdefcolor.keys():
                    if key in line and cdefcolor[key] is None:
                        cdefcolor[key] = extract_hex_color(line)
                        if cdefcolor[key] is None:
                            raise Exception("Error extracting hex color")

            for key in cdefcolor.keys():
                buf = c2xrsrc[key] + ": " + cdefcolor[key] + "\n"
                xf.write(buf)

main()
