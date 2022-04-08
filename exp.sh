#!/bin/bash

#filename="temp_results.txt"
#rm $filename
#for i in {1..10}; do
#    python3 main.py >> $filename 2> /dev/null
#    #python3 main.py >> $filename
#done
#python3 output_parser.py $filename


mkdir -p out
for at in "per_apartment" "per_floor" "per_line" "per_door"; do
    for lm in "uniform" "two_quadrants" "few_hot" "gaussian"; do
        filename="./out/"$at"__"$lm".log"
        for it in {1..100}; do
            python3 main.py $at $lm >> $filename 2> /dev/null
            #python3 main.py $at $lm >> $filename
            #echo "" > /dev/null
        done
        #echo "python3 main.py $at $lm >> $filename 2> /dev/null"
        python3 output_parser.py $filename
        #echo "python3 output_parser.py $filename"
    done
done
