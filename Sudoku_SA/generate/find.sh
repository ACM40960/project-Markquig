#!/bin/bash

echo "print to screen"

n=4
site=https://nine.websudoku.com/?level=${n}
curl -o out.html "${site}"
#curl -o out.html "https://nine.websudoku.com/?level=2"

sed -n -e 's/^.*ID="cheat" TYPE=hidden VALUE="\(.*\)">/\1/p' out.html > solution.txt

#sed -e "s/.\{9\}/&\n/g" solution.txt > solution_2.txt
#sed -e "s/.\{3\}/&\t/g" solution_2.txt > solution_3.txt
#sed -e 's/\(.\)/\1 /g' < solution_3.txt > solution_4.txt
sed -e 's/\(.\)/\1 /g' -e "s/.\{6\}/&\t/g" -e "s/.\{21\}/&\n/g" solution.txt > solution_2.txt

sed -n -e 's/^.*ID="editmask" TYPE=hidden VALUE="\(.*\)">/\1/p' out.html > hints.txt
#sed -e "s/.\{9\}/&\n/g" hints.txt > hints_2.txt
#sed -e "s/.\{3\}/&\t/g" hints_2.txt > hints_3.txt
#sed -e 's/\(.\)/\1 /g' < hints_3.txt > hints_4.txt
sed -e 's/\(.\)/\1 /g' -e "s/.\{6\}/&\t/g" -e "s/.\{21\}/&\n/g" hints.txt > hints_2.txt


#count no. of missing values i.e. 0's
no_zeros="$(grep -o '1' hints_2.txt | wc -l)"
#remove blank space at start of end of variable
no_zeros="$(echo -e "${no_zeros}" | sed -e 's/^[[:space:]]*//' -e 's/[[:space:]]*$//')"
echo $no_zeros
echo $no_zeros >> puzzle_2.txt

if [ "$no_zeros" == "55" ] ||  [ "$no_zeros" == "50" ];
then
    exit 0
fi

#number assigned to puzzle by website
#puzzle_num="$(perl -nle 'print $1 if /Medium Puzzle (.+?)\</' out.txt)"
puzzle_num="$(perl -nle 'print $1 if /(?:Easy|Medium|Hard|Evil) Puzzle (.+?)\</' out.html)"
echo $puzzle_num
echo $puzzle_num >> puzzle_2.txt


#i_n=1
#j_n=3
#echo $i_n $j_n
#perl -lanse 'print $F[$j-1] if $. == $i' -- -i=5 -j=3 file
#hint="$(perl -lanse 'print $F[$j-1] if $. == $i' -- -i=$i_n -j=$j_n hints_4.txt )"
#hold="$(perl -lane 'print $F[${i}] if $. == ${j}' hints_4.txt )"
#echo $hint

echo 3 > puzzle.txt

for i_n in {1..9}
do
for j_n in {1..9}
do
    #echo $j_n
    hint="$(perl -lanse 'print $F[$j-1] if $. == $i' -- -i=$i_n -j=$j_n hints_2.txt )"

    if [ "$hint" == "0" ];
    then
        #echo $j_n
        value="$(perl -lanse 'print $F[$j-1] if $. == $i' -- -i=$i_n -j=$j_n solution_2.txt )"
        printf "$value " >> puzzle.txt
    else
        printf "0 " >> puzzle.txt
    fi
done
done

sed -e "s/.\{6\}/&\t/g" -e "s/.\{21\}/&\n/g" puzzle.txt > puzzle_2.txt


#n=2
file_name=input_${no_zeros}_${n}_${puzzle_num}.txt

awk 'FNR==1{print ""}1' puzzle_2.txt solution_2.txt > $file_name

mkdir -p ./hold_results/${no_zeros}/; mv $file_name $_

rm solution*
rm puzzle*
rm hint*


 
exit 0




