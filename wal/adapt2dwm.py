#!/usr/bin/env python

import os
import sys
import tempfile
import shutil
import datetime
import mmap

DEBUG = 0

# files used along the code
home = os.path.expanduser("~")
srcpath = home + "/.config/dwm-6.2/config.def.h"
src_finalpath = srcpath + ".walunmod"
tmpfd, tmppath = tempfile.mkstemp()
scriptdir = os.path.dirname(os.path.realpath(__file__))
scriptpath = scriptdir + __file__
debugpath = scriptdir + "/config.def.h.wal"
logfile = scriptdir + "/waladapt.log"

# according to pywal docs this is what replaces color definition in file at filepath
repl = '#include "' + home  +'/.cache/wal/colors-wal-dwm.h"'

# notification to put in place noticing file change
notif = "/* This file has been modified by pywal adapting script at '" + scriptpath + "'." +\
        "The original (unmodified) file is at " + src_finalpath + " */"

# lines to look for in order to position color definition in file at filepath
start_delim = "/* COLOR BEGIN */"
end_delim = "/* COLOR END */"

# replacements for variables directly mentioned by their name in lines after color definition
var_repl = \
	{
		"col_gray1": "norm_fg",
		"col_gray2": "norm_bg",
		"col_gray3": "norm_border",

		"col_gray4": "sel_fg",
		"col_cyan": "sel_bg",
		"col_cyan": "sel_border",
	}

def log(msg):
    log_buf = ""
    now = datetime.datetime.now()
    now = now.strftime("%d/%m/%Y, %H:%M:%S")
    log_buf += "[ " + now + " ]"
    log_buf += ": " + msg + "\n"
    with open(logfile, "a") as l:
        l.write(log_buf)

def main():
    if len(sys.argv) > 1 and sys.argv[1] == "--debug":
        DEBUG = 1
    try:
        # flags to check if delimiters have been encountered
        st_delim_f = 0
        end_delim_f = 0
        with open(srcpath, "rb") as src:
            with open(tmppath, "wb") as dst:
                srcmap = mmap.mmap(src.fileno(), 0, prot=mmap.PROT_READ)
                tmpmap = bytearray()
                buf = bytearray()
                st_delim_idx = -1
                end_delim_idx = -1

                st_delim_idx = srcmap.find(bytes(start_delim, "utf8"))
                if st_delim_idx == -1:
                    st_delim_f = 0
                else:
                    st_delim_f = 1

                tmpmap += srcmap[:st_delim_idx]
                tmpmap += bytes(repl, "utf8")

                end_delim_idx = srcmap.find(bytes(end_delim, "utf8"))
                if end_delim_idx == -1:
                    end_delim_f = 0
                else:
                    end_delim_f = 1

                if end_delim_f != st_delim_f:
                    raise Exception("Delimiters mismatched in file at " + srcpath)
                if end_delim_f == 0 and st_delim_f == 0:
                    raise Exception("File is already adapted")

                srcmap.seek(end_delim_idx)
                srcmap.readline()
                tmpmap += bytes('\n' + notif, "ascii")

                buf = srcmap.read()
                for key in var_repl.keys():
                    bkey = bytes(key, "utf8")
                    bval = bytes(var_repl[key], "utf8")
                    buf = buf.replace(bkey, bval)
                tmpmap += buf
                dst.write(tmpmap)
        if DEBUG == 1:
            shutil.copymode(srcpath, tmppath)
            shutil.move(tmppath, debugpath)
            log("DEBUG - Successful modified config.def.h")
        else:
            shutil.copymode(srcpath, tmppath)
            shutil.move(srcpath, src_finalpath)
            shutil.move(tmppath, srcpath)
            log("Successfully modified config.def.h")
    except Exception as err:
        log(str(err))

main()
