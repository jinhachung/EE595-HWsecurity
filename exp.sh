#!/bin/bash

filename="temp_results.txt"

rm $filename
for i in {1..10}; do
    python3 main.py >> $filename 2> /dev/null
    #python3 main.py >> $filename
done
python3 output_parser.py $filename
