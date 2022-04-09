#!/bin/bash

#filename="temp_results.txt"
#rm $filename
#for i in {1..10}; do
#    python3 main.py >> $filename 2> /dev/null
#    #python3 main.py >> $filename
#done
#python3 output_parser.py $filename


rm -rf out/raw
rm -rf out/parsed
mkdir -p out
mkdir -p out/raw
mkdir -p out/parsed
for at in "per_apartment" "per_floor" "per_line" "per_door"; do
    for lm in "uniform" "two_quadrants" "few_hot" "gaussian"; do
        filename=$at"__"$lm".log"
        rawname="./out/raw/"$filename
        parsedname="./out/parsed/"$filename
        for it in {1..10}; do
            python3 main.py $at $lm >> $rawname 2> /dev/null
            #python3 main.py $at $lm >> $rawname
            #echo "" > /dev/null
        done
        #echo python3 main.py $at $lm >> $rawname 2> /dev/null"
        python3 output_parser.py $rawname > $parsedname
        #echo "python3 output_parser.py $filename > $parsedname"
    done
done
