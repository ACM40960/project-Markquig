[![Work in Repl.it](https://classroom.github.com/assets/work-in-replit-14baed9a392b3a25080506f3b7b6d57f295ec2978f6f33ec97e36a161684cbe9.svg)](https://classroom.github.com/online_ide?assignment_repo_id=4835627&assignment_repo_type=AssignmentRepo)



# Sudoku_SA

## Over View
In this directory, you will find code to perform simulated annealing on sudoku puzzles. There are four versions constant (sudoku_sa_const.h), linear (sudoku_sa_line.h), quadratic (sudoku_sa_squ.h) & exponential (sudoku_sa_exp.h). These are run through the C++ file sudoku_general.cpp. 

The remaining files are used to run sudoku_general.cpp multiple times and put results into the directory Sudoku_SA/output/. The file run_100.sh will do this using the parameters stored in the file variables.txt. The file run_inputs.txt contains examples of lines to run run_100.sh for a given input

The directory Sudoku_SA/input/ contains example input puzzles. The subdirectories of this that begin with numbers (e.g. Sudoku_SA/input/45/) contain puzzles with that many non fixed cells/missing values. The directory input/order_4/ contains sudoku puzzle of order 4.

The directory Sudoku_SA/generate/ contains a script to get new puzzles.

The remaining subdirectories of Sudoku_SA/contain results of running run_100.sh on several input files for various sets of parameters and header files. For example, the directory Sudoku_SA/output_exp_1/ contains results of my first set of chosen parameters on the exponential header file, sudoku_sa_exp.h. While the directory Sudoku_SA/output_exp_2/ contains the results of my second set of chosen parameters on the exponential headder file.


## Run sudoku_general.cpp
I used g++ to compile this. The lines for the terminal when in Sudoku_SA/ are:

    g++ sudoku_general.cpp 
    ./a.out input/45/input_45_1_2,967,429,033.txt 0.9 0.9999995 100000
    
This will run the given file in with an initial temperature of 0.9, cooling rate of 0.9999995, with 100000 steps per attempt. If run with the quadratic header file included, results will typically be less than 10.


## Run run_100.sh
This runs the above lines multiple times, so you will need to be able to run them. A directory Sudoku_SA/output/ needs to be created before it will run. 

    mkdir output
Examples of how to run this are griven in the file run_inputs.txt. Copy one, or multiple lines from this into the terminal to run. It shouldn't be needed, but if the file is unable to exicute run: 

chmod u+x run_100.sh
    
## Generate
in the directory Sudoku_SA/generate/ is another bash script find.sh which will get new puzzles from the website https://www.websudoku.com/ .To run from the terminal in the directory Sudoku_SA/generate/ the line is:

    ./find.sh
Again chmod u+x may be required here. 
    
    

# Rubik_SA
This directory contains my failed attempt at writing code to solve a Rubik cube using simulated annealing. The program is contained in the file rubik_sa.cpp, which has two versions of the function, Rubic_SA::sample_neighbour, one to pick the proposed next step total randomly, another to pick dependent on the cost of the neighbouring steps. The preference can be made by commenting out one over the other. The file input.txt contains a sample input. To run, use the following lines in the terminal:

    g++ rubik_sa.cpp 
    ./a.out input.txt

The subdirectory Rubik_SA/generator/ contains code to generate new inputs using the files generate.cpp & input_clean.txt. To run from a terminal within the directory Rubik_SA/generator/ use the following lines:

    g++ generate.cpp   
    ./a.out input_clean.txt > input.txt
    mv input.txt ..    
The last line will move the file up the directory to be used by rubik_sa.cpp 





