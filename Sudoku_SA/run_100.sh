#!/bin/bash

file="${1##*/}"   #remove any directions to the folder
file=${file%.*}   #remove file postfix (e.g. .txt)
echo $file          #print current file
start_temps=( $(sed '1q;d' variables.txt) ) #import variables
cool=( $(sed '2q;d' variables.txt) )
loop_total=( $(sed '3q;d' variables.txt) )

g++ -o run.out sudoku_general.cpp   #compile

#three loops to print variables
for i in "${start_temps[@]}"
do
    printf "$i\t" >> output/${file}_out.txt
done
printf "\n" >> output/${file}_out.txt
for j in "${cool[@]}"
do
    printf "$j\t" >> output/${file}_out.txt
done
printf "\n" >> output/${file}_out.txt
for k in "${loop_total[@]}"
do
    printf "$k\t" >> output/${file}_out.txt
done
printf "\n" >> output/${file}_out.txt


#loops to add start temp to fist row of each column
for i in "${start_temps[@]}"
do
    for j in "${cool[@]}"
    do
        for k in "${loop_total[@]}"
        do
            #printf "$i _ $j _ $k,\t" >> output/${file}_out.txt
            printf "$i,\t" >> output/${file}_out.txt
        done
    done
done
printf "\n" >> output/${file}_out.txt

#loops to add cool to socond row of each column
for i in "${start_temps[@]}"
do
    for j in "${cool[@]}"
    do
        for k in "${loop_total[@]}"
        do
            printf "$j,\t" >> output/${file}_out.txt
        done
    done
done
printf "\n" >> output/${file}_out.txt

#loops to add loop total to fist row of each column
for i in "${start_temps[@]}"
do
    for j in "${cool[@]}"
    do
        for k in "${loop_total[@]}"
        do
            printf "$k,\t" >> output/${file}_out.txt
        done
    done
done
printf "\n" >> output/${file}_out.txt


for VARIABLE in {1..30}
do
    for i in "${start_temps[@]}"
    do
        for j in "${cool[@]}"
        do
            for k in "${loop_total[@]}"
            do
                ./run.out $1 $i $j $k >> output/${file}_out.txt #run program
                printf ",\t" >> output/${file}_out.txt
            done
        done
    done
    
    printf "\n" >> output/${file}_out.txt
done

exit 0

#example of how to run
#./run_100.sh input/46/input_46_1_473,120,317.txt

#changed to take in file direction as input
#print variables for each column in seporate rows

