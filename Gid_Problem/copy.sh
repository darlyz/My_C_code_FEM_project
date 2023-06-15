#!/bin/bash
DestPath=/mnt/d/ECTEC/PFELAC2.2/GiD/problemtypes/TinyFem.gid

if [ $# -gt 0 ]
then
    SrcPath=./TinyFem_$1.gid
    cp $SrcPath/* $DestPath
fi
