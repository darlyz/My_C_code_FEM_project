#!/bin/bash
DestPath=$GidPath/problemtypes/TinyFem.gid

if [ $# -gt 0 ]
then
    SrcPath=./TinyFem_$1.gid
    cp $SrcPath/* $DestPath
fi
