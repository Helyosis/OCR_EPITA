#!/bin/bash
mkdir bests
mkdir results
mkdir weightsres
# Itteration variable :
i=0
while [ $i -lt 200000 ]
do
    echo "Iteration $i"
    ./main -m train -n 40000 --nb-images 2000 ./results/res$1.txt -vv | awk '{print $2}' >> ./weightsres/bias$1.txt
    get_best_weights
    i=$[$i+1]
done


function get_best_weights() {
    # Reads each file from the directory results
    nbfiles = $(ls weightsres/*.txt | wc -l)
    i=0
    max = 0
    while [ $i -lt $nbfiles ]
    do
        counter = 0
        while read line
        do
            if [$line -gt 0.98]
            then
                counter=$[$counter+1]
            fi
        done < results/res$i.txt
        if [ $counter -gt $max ]
        then
            max=$counter
        fi
        i=$[$i+1]
    done
    cp ./results/res$max.txt ./bests/best.txt
}