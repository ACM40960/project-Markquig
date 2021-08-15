//
//  Rubik_sa.cpp
//
//
//  Created by Mark Quigley on 13/07/2021.
//

#include <iostream>
#include <fstream>
#include <cmath>
#include <random>

class Rubic_SA {
    int*** puzzle;                      //holds current state of puzzle
    //int cost;
public:
    int cost;                           //holds current cost of puzzle
    int* neighbours_cost;               //holds cost of each neighbour
    
    Rubic_SA (std::ifstream& myfile);   //constructor from input file
    Rubic_SA (const Rubic_SA &P);       //copy constructor
    ~Rubic_SA ();                       //destructor

    void print_puzzle ();               //prints puzzle
    void print_face ( int face );       //prints one face of the puzzle
    
    int find_cost ( bool set_cost );    //find the cost of the current state
    
    void rotate (int face, int depth, int no_times);    //proforms a rotation
    
    int find_neighbours_cost (int face, int depth, int no_times);   //finds cost of neigh

    int sample_neighbour ();            //picks a neighbour to try as next step
    
};


Rubic_SA::Rubic_SA (std::ifstream& myfile) {
    //myfile >> order;
    
    puzzle = new int** [6];
    
    int i, j, k;
    
    puzzle[4] = new int* [3];
    for (i = 0; i < 3; i++) {
        puzzle[4][i] = new int [3];
        for (j = 0; j < 3; j++){
            myfile >> puzzle[4][i][j];
        }
    }
    
    puzzle[3] = new int* [3];
    puzzle[0] = new int* [3];
    puzzle[2] = new int* [3];
    puzzle[1] = new int* [3];
    
    int faces[] = {3, 0, 2, 1};
    for (i = 0; i < 3; i++) {
        puzzle[3][i] = new int [3];
        puzzle[0][i] = new int [3];
        puzzle[2][i] = new int [3];
        puzzle[1][i] = new int [3];

        for (j = 0; j < 4; j++) {
            for (k = 0; k < 3; k++){
                myfile >> puzzle [faces[j]] [i] [k];
            }
        }
    }

    puzzle[5] = new int* [3];
    for (i = 0; i < 3; i++) {
        puzzle[5][i] = new int [3];
        for (j = 0; j < 3; j++){
            myfile >> puzzle[5][i][j];
        }
    }

    neighbours_cost = NULL;
}

Rubic_SA::Rubic_SA (const Rubic_SA &P) {
    int i, j, k;
    int value;
    //order = P.order;
    
    puzzle = new int** [6];
    
    for (i = 0; i < 6; i++) {
        puzzle[i] = new int* [3];
        for (j = 0; j < 3; j++){
            puzzle[i][j] = new int[3];
            for (k = 0; k < 3; k++){
                puzzle[i][j][k] = P.puzzle[i][j][k];
            }
        }
    }
    
}

Rubic_SA::~Rubic_SA() {
    //std::cout << "deconstructor\n";
    
    int i, j;
    for (i = 0; i < 6; i++) {
        for (j = 0; j < 3; j++){
            delete[] puzzle[i][j];
        }
        delete[] puzzle[i];
    }
    delete[] puzzle;
    
    if (neighbours_cost != NULL) {
        delete neighbours_cost;
    }

}



void Rubic_SA::print_puzzle (){
    int i, j, k;
    int face;   //the current sub grid number
    int value;
    
    for (i = 0; i < 3; i++) {
        std::cout << "\t";
        for (j = 0; j < 3; j++){
            std::cout << puzzle[4][i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    
    int faces[] = {3, 0, 2, 1};
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++) {
            for (k = 0; k < 3; k++){
                std::cout << puzzle [faces[j]] [i] [k] << " ";
            }
            std::cout << "\t";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    
    for (i = 0; i < 3; i++) {
        std::cout << "\t";
        for (j = 0; j < 3; j++){
            std::cout << puzzle[5][i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void Rubic_SA::print_face ( int face ) {
    std::cout << "print face " << face << std::endl;
    
    int i, j;
    for (i = 0; i < 3; i++) {
        std::cout << "\t";
        for (j = 0; j < 3; j++){
            std::cout << puzzle[face][i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

}


int Rubic_SA::find_cost ( bool set_cost ){
    int cost_calculate = 0;   //variable from class
    
    int i, j, k;

    int colour_count[6];
    int colour_count_total[6] = {};
    int max, test;
    
    for (i = 0; i < 6; i++) {
        for (j = 0; j < 3; j++) {
            for (k = 0; k < 3; k++) {
                if ( puzzle[i][j][k] != puzzle[i][1][1] ){
                    cost_calculate++;
                }
            }
        }
    }
    
    if (set_cost == 1) {
        this->cost = cost_calculate;   //saves to cost in class definition
    }
    //std::cout << "\t\tcost_calculate = " << cost_calculate << std::endl;

    return cost_calculate;
}


void Rubic_SA::rotate (int face, int depth, int no_times) {
    int i, j, k;
    
    // see https://www.geeksforgeeks.org/rotate-a-matrix-by-90-degree-in-clockwise-direction-without-using-any-extra-space/
    //1 2 3   //1 3 9 7 and 2 6 8 4 are the two dimonds of the outer cycle of
    //4 5 6   //this matrix as these pemute with each other
    //7 8 9   //in a 4x4 matrix there are 2 cycles, 4 dimonds in outer, 1 in inner
    if (depth == 0 ) {
        //std::cout << "rotate face " << face << std::endl;
        int fc = face;
        int N = 3;
        
        for (int i = 0; i < N / 2; i++) {   //over each cycle
            for (int j = i; j < N - i - 1; j++) {   //over dimonds in cycle
                for (k = 0; k < no_times; k++){
                    // Swap elements of each cycle
                    // in clockwise direction
                    int temp = puzzle[fc][i][j];
                    puzzle[fc] [i] [j]         = puzzle[fc] [N-1-j] [i];
                    puzzle[fc] [N-1-j] [i]     = puzzle[fc] [N-1-i] [N-1-j];
                    puzzle[fc] [N-1-i] [N-1-j] = puzzle[fc] [j] [N-1-i];
                    puzzle[fc] [j] [N-1-i]     = temp;
                }
            }
        }
    }
    else if (depth == 2) {
        //std::cout << "rotate face " << face+1 << std::endl;
        int fc = face + 1;
        int N = 3;

        for (int i = 0; i < N / 2; i++) {   //over each cycle
            for (int j = i; j < N - i - 1; j++) {   //over dimonds in cycle
                for (k = 0; k < no_times; k++){
                    // Swap elements of each cycle
                    // in counter clockwise direction
                    int temp = puzzle[fc][i][j];
                    puzzle[fc][i][j]           = puzzle[fc] [j] [N-1-i];
                    puzzle[fc] [j] [N-1-i]     = puzzle[fc] [N-1-i] [N-1-j];
                    puzzle[fc] [N-1-i] [N-1-j] = puzzle[fc] [N-1-j] [i];
                    puzzle[fc] [N-1-j] [i]     = temp;
                }
            }
        }
    }
    
    //this chould be replace with case statment
    //I would like to try an array for which holds which cols and rows to switch
    // in each case, may not be possable
    if (face == 0) {
        //std::cout << "rotate row & col\n";
        //std::cout << "\tfaces = " <<
        int hold_val;
        for (i = 0; i < no_times; i++){
            for (j = 0; j < 3; j++) {
                hold_val = puzzle[2][j][depth];
                puzzle[2][j][depth]     = puzzle[4][2-depth][j];
                puzzle[4][2-depth][j]   = puzzle[3][2-j][2-depth];
                puzzle[3][2-j][2-depth] = puzzle[5][depth][2-j];
                puzzle[5][depth][2-j]   = hold_val;
            }
        }

    }
    //*
    else if (face == 2) {
        //std::cout << "rotate cols\n";
        int hold_val;
        for (i = 0; i < no_times; i++){
            for (j = 0; j < 3; j++) {
                hold_val = puzzle[0][j][depth];
                puzzle[0][j][depth]   = puzzle[5][j][depth];
                puzzle[5][j][depth]   = puzzle[1][2-j][2-depth];
                puzzle[1][2-j][2-depth] = puzzle[4][j][depth];
                puzzle[4][j][depth]   = hold_val;
            }
        }
    }
    
    else if (face == 4) {
        //std::cout << "rotate rows\n";
        int *hold_row;
        for (i = 0; i < no_times; i++){
            hold_row = puzzle[0][depth];   //check if the sharing of pointers is ok
            puzzle[0][depth] = puzzle[2][depth];
            puzzle[2][depth] = puzzle[1][depth];
            puzzle[1][depth] = puzzle[3][depth];
            puzzle[3][depth] = hold_row;
        }
    }//*/
    
}

int Rubic_SA::find_neighbours_cost (int face, int depth, int no_times){
    this->rotate ( face, depth, no_times);
    int new_cost = this->find_cost (0);
    return new_cost;
}

//*
int Rubic_SA::sample_neighbour (){
    
    //sample from CDF
    std::random_device device;   //random input to sampaler
    std::uniform_real_distribution<double> uniform(0,1);   //U[0,1) sampaler
    float u = uniform(device);
    int i = (int)(u*27);
    
    return i;
}//*/

/*
int Rubic_SA::sample_neighbour (){
    int i, j, k;
    
    int costs[3*3*3];       //holds max_cost - cost of each neighbour

    int position = 0;       //position in cost array
    int total_costs = 0;    //total cost of all neighbours (CDF nomalizing const)
    int max_cost = 42;      //maximum possable cost
    for (i = 0; i < 3; i++){            //face
        for (j = 0; j < 3; j++){        //depth
            for (k = 0; k < 3; k++){    //no_times
                position = i*9 + j*3 + k;
                int hold = this -> find_neighbours_cost (i*2, j, 1);
                costs[position] = max_cost - hold;  //high prob for lower cost
                total_costs += costs [position];    //nomalizing constant
                
                //costs[position] = max_cost - hold;
                //total_costs += costs [position];
                
                if (hold == 0) {    //when this move solves it
                    return -1;
                }
                
                //std::cout << "\t" << i*2 << ", " << j << ", " << k+1 << ", cost = " << hold << std::endl;
                //this->print_puzzle();

                //std::cout << costs[position] << ", ";
                //position++;
            }
            this->rotate ( i*2, j, 1);   //to bring back to orriginal state
        }
        //std::cout << std::endl;
    }
    //std::cout << std::endl;
    
    //std::cout << "total_costs = " << total_costs << "\n";

    
    //calculate normalized CDF
    double CDF[3*3*3];
    CDF[0] = (double)costs[0]/total_costs;
    //std::cout << "CDF = " << CDF[0] << ", ";
    for (i = 1; i < 3*3*3; i++) {
        CDF [i] = CDF[i-1] + (double)costs[i]/total_costs;
        
    }
    //std::cout << std::endl;
    
    //sample from CDF
    std::random_device device;   //random input to sampaler
    std::uniform_real_distribution<double> uniform(0,1);   //U[0,1) sampaler
    float u = uniform(device);
    i = 0;
    while (CDF[i] < u){
        i++;
    }
    
    return i;
}//*/


int main (int argc, const char * argv[]) {
    std::cout << "hello\n";
    
    std::ifstream myfile(argv[1], std::ios_base::in);
    Rubic_SA rubic_current(myfile);       //sudoku current solution
    Rubic_SA rubic_new (rubic_current);
    
    rubic_current.find_cost (1);
    std::cout << "cost = " << rubic_current.cost << std::endl;
    
    
    std::random_device device;   //random input to sampaler
    std::uniform_real_distribution<double> uniform(0,1);   //U[0,1) sampaler
    
    
    double temp = 1;   //holds current temperature of system
    double cool = 1;
    int loop_total = 100000;

    int accept_rate;
    int count;
    


    //*
    int move_count = 0;
    for (count = 1; count < loop_total; count ++){
        
        //double u = uniform(device);
        //int n = (int) (u*3*3*3);
        
        int n = rubic_current.sample_neighbour ();
        
        int face = ( n/9 );
        int depth = ( (n-9*face) / 3 );
        int no_times = ( n - 9*face - 3*depth );
        face = face*2;
        
        rubic_current.rotate ( face, depth, no_times);
        int new_cost = rubic_current.find_cost (0);
        
        //std::cout << "new_cost = " << new_cost << std::endl;
        if (new_cost == -1){
            break;
        }

        temp = temp*cool;
        accept_rate = std::exp((rubic_current.cost - new_cost)/temp);

        if (accept_rate >= 1) {  //accept
            rubic_current.cost = new_cost;
            move_count++;
        }
        else if (accept_rate >= uniform(device) ) {
            rubic_current.cost = new_cost;
            move_count++;
        }
        else {
            rubic_current.rotate ( face, depth, 4-no_times );
        }

                    
        if (rubic_current.cost == 0){
            std::cout << "end reached in " << count << "steps\n";
            break;
        }
    }
    std::cout << "move_count = " << move_count << ", out of " << count << std::endl;
    //*/
    
    std::cout << "end cost = " << rubic_current.cost << std::endl;
    
    rubic_current.print_puzzle();
    
}





