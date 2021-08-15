[![Work in Repl.it](https://classroom.github.com/assets/work-in-replit-14baed9a392b3a25080506f3b7b6d57f295ec2978f6f33ec97e36a161684cbe9.svg)](https://classroom.github.com/online_ide?assignment_repo_id=4835627&assignment_repo_type=AssignmentRepo)



# Sudoku_SA

## Over View
In this directory you will find code to proform simulated annealing on sudoku puzzles. There are four versions constant (sudoku_sa_const.h), linear (sudoku_sa_line.h), quadratic (sudoku_sa_squ.h) & exponentil (sudoku_sa_exp.h). These are run through the C++ file sudoku_general.cpp. 

The remaining files are used to run sudoku_general.cpp multiple times and put results into a sub directory output/. The file run_100.sh will do this using the paramaters stored in the file variables.txt. The file run_inputs.txt contains examples of line to run run_100.sh for a given input

The sub directory input/ contains example input puzzles. The sub directories of input that begin with numbers (e.g. input/45/) contain puzzles with that many non fixed cells/missing values. The directory input/order_4/ contains sudoku puzzle of order 4.

The remaining sub directories of Sudoku_SA/ contian results of running run_100.sh on several input files for various set of paramaters and header files. For example the directory Sudoku_SA/output_exp_1/ contiains results of my first set of chocen paramaters on the exponentill headder file, sudoku_sa_exp.h. While the directory Sudoku_SA/output_exp_2/ contains the results of my second set of chocen paramaters on the exponentill headder file.


## Run sudoku_general.cpp
I used g++ to compile this. The lines for the teminal when in Sudoku_SA/ are:

    g++ sudoku_general.cpp 
    ./a.out input/45/input_45_1_2,967,429,033.txt 0.9 0.9999995 100000
    
This will run the given file in with an itial tempature of 0.9, cooling rate of 0.9999995, with 100000 step per attmept. If run with the quadratic header file included, results will typicly be less then 10.


## Run run_100.sh
This runs the above lines multiple times, so will need to be able to run them. A directory Sudoku_SA/output/ needs to be created before it will run. 

    mkdir output
Examples of how to run this are griven in the file run_inputs.txt. Copy one, or multiple lines from this into the terminal to run. It shouldn't be needed, but if the file is unable to exicute run: 
    chmod u+x run_100.sh
    

# Rubik_SA
This directory contains my failed attempt at writing code to solve a Rubik cube using simulated annealing. The program is contained in the file rubik_sa.cpp, and an example input in input.txt. To run use the following lines in the terminal:
    g++ rubik_sa.cpp 
    ./a.out input.txt

The sub directory Rubik_SA/generator/ contains code to generate generate new inputs using the files generate.cpp & input_clean.txt. To run from a terminal within the directoru Rubik_SA/generator/ use the following lines:
    g++ generate.cpp   
    ./a.out input_clean.txt > input.txt
    mv input.txt ..    
The last line will move the file up the directory to be used by rubik_sa.cpp 





