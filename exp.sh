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
# regular experiment with no sensitivity
for tv in {1..20}; do
    for at in "per_apartment" "per_floor" "per_line" "per_door"; do
        for lm in "uniform" "two_quadrants" "few_hot" "gaussian" "sequential"; do
            for fs in "single" "triple" "all"; do
                filename=$fs"__tv"_$tv"__"$at"__"$lm".log"
                rawname="./out/raw/"$filename
                parsedname="./out/parsed/"$filename
                for it in {1..20}; do
                    python3 main.py $at $lm $fs $tv >> $rawname 2> /dev/null
                done
                python3 output_parser.py $rawname > $parsedname
            done
        done
    done
done

