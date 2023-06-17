#!/bin/bash

valid=0

for ((i=1;i<=12;i++))
do
    echo " --------- exam$i calucating --------- "
    ./tinyfem exam$i
    if (($?>1))
    then
        succ=0
    else
        succ=$?
    fi
    ((valid+=$succ))
done

echo " ---- $valid exams success ---- "