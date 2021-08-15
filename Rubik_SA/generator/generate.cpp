#include <iostream>
#include <fstream>
#include <cmath>
#include <random>

class Rubic_SA {
    int*** puzzle;
    //int cost;
public:
    int cost;
    int* neighbours_cost;
    
    Rubic_SA (std::ifstream& myfile);
    Rubic_SA (const Rubic_SA &P);   //copy constructor
    ~Rubic_SA ();

    void print_puzzle ();
    void print_face ( int face );
    
    int find_cost ( bool set_cost );
    
    void rotate (int face, int depth, int no_times);
    
    int find_neighbours_cost (int face, int depth, int no_times);

    int sample_neighbour ();
    
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
        //colour_count[] = {0,0,0,0,0,0};
        memset(colour_count, 0, sizeof(colour_count));
        max = 0;
        
        //find most common colour
        for (j = 0; j < 3; j++) {
            for (k = 0; k < 3; k++) {
                colour_count [ puzzle[i][j][k] - 1 ] ++;
                test = colour_count [ puzzle[i][j][k] - 1 ];
                if ( test > max){
                    max = test;
                }
            }
        }
        //std::cout << "\tface " << i << ", max = " << max << std::endl;
        cost_calculate += 3*3 - max;
        //std::cout << "\tcost_calculate = " << cost_calculate << std::endl;
        //std::cout << "cost for face " << i << " = " << 3*3 - max << std::endl;
        
        for (j = 0; j < 6; j++){
            colour_count_total[j] += colour_count[j];
        }
    }
    
    for (i = 0; i < 6; i++){
        if (colour_count_total[i] != 9){
            std::cout << "\tcolour_count_total = ";
            for (j = 0; j < 6; j++) {
                std::cout << colour_count_total[j] << " ";
            }
            std::cout << std::endl;
            return -1;   //exit function with impossable value of cost
            std::cout << "test\n";
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

int Rubic_SA::sample_neighbour (){
    //max cost is 6*3*3 - 6 = 48
    int i, j, k;
    
    int costs[3*3*3];

    int position = 0;
    int total_costs = 0;
    int max_cost = 6*3*3 - 6;
    double reduce = 1;   //factor to reduce the cost by
    //std::cout << "costs = ";
    for (i = 0; i < 3; i++){   // face
        for (j = 0; j < 3; j++){   //depth
            for (k = 0; k < 3; k++){   //no_times
                position = i*9 + j*3 + k;
                //CDF[position] = this -> find_neighbours_cost (i, j, k);
                int hold = this -> find_neighbours_cost (i*2, j, 1);
                costs[position] = max_cost - hold;
                total_costs += costs [position];
                
                //std::cout << "\t" << i*2 << ", " << j << ", " << k+1 << ", cost = " << hold << std::endl;
                //this->print_puzzle();

                //std::cout << costs[position] << ", ";
                //position++;
                /*
                if (i == 2 && j == 1 && k == 2){
                    this->print_puzzle();
                }//*/
            }
            this->rotate ( i*2, j, 1);   //to bring back to orriginal state
        }
        //std::cout << std::endl;
    }
    //std::cout << std::endl;
    
    //std::cout << "total_costs = " << total_costs << "\n";

    
    double CDF[3*3*3];
    CDF[0] = (double)costs[0]/total_costs;
    //std::cout << "CDF = " << CDF[0] << ", ";
    for (i = 1; i < 3*3*3; i++) {
        CDF [i] = CDF[i-1] + (double)costs[i]/total_costs;
        /*
        std::cout << CDF[i] << ", ";
        if (i%9==8) {
            std::cout << std::endl;
        }//*/
    }
    //std::cout << std::endl;
    
    std::random_device device;   //random input to sampaler
    std::uniform_real_distribution<double> uniform(0,1);   //U[0,1) sampaler
    float u = uniform(device);
    i = 0;
    while (CDF[i] < u){
        i++;
    }
    

    return i;
}
    


int main () {
    
    std::ifstream myfile ("input_clean.txt");
    Rubic_SA rubic_current (myfile);

    std::random_device device;   //random input to sampaler
    std::uniform_real_distribution<double> uniform(0,1);   //U[0,1) sampaler
    //*
    int no_turns = 100;
    int i;
//    int max = 10;
//    int hold;
    for (i = 0; i < no_turns; i++) {
        double u = uniform(device);
        int n = (int) (u*3*3*3);
                
        int face = ( n/9 );
        int depth = ( (n-9*face) / 3 );
        int no_times = ( n - 9*face - 3*depth );
        face = 2*face;
        
        //std::cout << "\tn = " << n << "\tface = " << face << "\tdepth = " << depth << "\tno_times = " << no_times << std::endl;

        
        rubic_current.rotate ( face, depth, no_times);
        
        /*hold = rubic_current.find_cost (0);
        if (max < hold ){
            max = hold;
            if (hold == 42){
                rubic_current.print_puzzle();
            }
        }//*/
    }//*/
    //rubic_current.rotate ( 4, 1, 1);
    
//    std::cout << "max = " << max << std::endl;
    
    rubic_current.print_puzzle ();

}
