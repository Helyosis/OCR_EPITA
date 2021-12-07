#!/bin/bash
mkdir bests
mkdir results
mkdir weightsres
# Itteration variable :
i=0
while [ $i -lt 200000 ]
do
    echo "Iteration $i"
    ./main -m train -n 40000 --nb-images 2000 ./results/res$1.txt -vvv >> ./weightsres/bias$1.txt
    i=$[$i+1]
done



# function get best weights
# this functions goes through the folder results and gets the file with the best weights
# it then copies the file to the folder bests
function getbestweights()
{
    # reads the file res$1.txt and counts the numbers of time that a line contains a number over 0.98 and stores it in a variable
    count = 0

    #reads the file res$1.txt line by line 
    while read line
    do
        # if the line contains a number over 0.98, the variable count is incremented
        if [[ line -gt 0.98 ]]
        then
            count=$[$count+1]
        fi
    done < ./results/res$1.txt
}