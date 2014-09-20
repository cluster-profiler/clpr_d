#!/bin/env python

import glob, shutil, os

indir = "/tmp/Clpr/output_tmp"
outdir = "/tmp/Clpr/output"

files = glob.glob("%s/db*"%indir)
files.sort()

snapshot = {}

if(len(files)>=2):
    for f in files[:-1]:
        ff = open(f)
        lines = map(lambda x: x.strip(), ff.readlines());
        for line in lines:
            tokens = line.split("?")
            if((not snapshot.has_key(tokens[0])) or int(tokens[7]) > int(snapshot[tokens[0]][7])):
                snapshot[tokens[0]] = tokens
        os.remove(f)

    f = files[-2] + ".s"
    ff = open(f, "w")
    ks = snapshot.keys()
    for k in ks:
        print >>ff,"?".join(snapshot[k])
    ff.close()
    shutil.move(f, outdir)
elif(len(files)==1):
    shutil.move(files[0], outdir)
else:
    pass



