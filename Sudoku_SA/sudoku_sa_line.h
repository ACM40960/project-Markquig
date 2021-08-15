//
//  sudoku.cpp
//
//
//  Created by Mark Quigley on 02/06/2021.
//


#include <iostream>
#include <fstream>
#include <cmath>
#include <random>

class Sudoku_SA {
    int order;                                  //order of the puzzle
    int** solution;                             //holds currnt proposed solution
    bool** fixed_points;                        //mark where fixed cells are with 1's
    int** cost;                                 //holds cost at each cell
  public:
    int temperature;        //holds the cost of the current proposed solution
    
    Sudoku_SA (std::ifstream& myfile);          //constructor for input file
    Sudoku_SA (const Sudoku_SA &P);             //copy constructor
    ~Sudoku_SA ();                              //destructor
    
    void print_sol ();                          //prints currint proposed solution
    void print_fixed_points ();                 //prints positions of fixed points
    void print_cost ();                         //prints cost at each cell

    void update_cost ();                        //updates the cost matrix and temperature
    int* sample_full_grid ();                   //used to sample the first point
    int* sample_sub_grid (int* pos_1);          //used to sample the second point
    
    int ** same_squ_elements (int a, int b);    //returns coordinates for the sub grid
    void delete_same_squ_elements (int** a);    //deletes objected treturnd above
    
    int get_cost (int row, int col);    //used to copy cost between two Sudoku_SA
    void copy_cost (Sudoku_SA* P);      //used to copy cost between two Sudoku_SA
    
    void swap (int** points);                   //swap numbers in two cells
};

Sudoku_SA::Sudoku_SA (std::ifstream& myfile) {
    myfile >> order;
    
    int i, j, k;    //variable used in for loops
    int sub_grid;   //the current sub grid number
    int value;      //holds read in values
    
    bool** used_sub_grid = new bool* [order*order];     //array to store the used
    for (i = 0; i < order*order; i++){                  // numbers in each sub grid
        used_sub_grid[i] = new bool [order*order]();
    }
    
    solution     = new int* [order*order];      //create space
    fixed_points = new bool* [order*order];
    cost         = new int* [order*order];

    for (i = 0; i < order*order; i++) {
        solution[i] = new int [order*order];        //create space
        fixed_points[i] = new bool [order*order];
        cost[i] = new int [order*order];
        
        for (j = 0; j < order*order; j++) {
            myfile >> solution[i][j];       //read in value
            
            if (solution[i][j] == 0) {      //if non fixed cell
                fixed_points[i][j] = 0;
            }
            else {                          //if fixed cell
                fixed_points[i][j] = 1;
                sub_grid = order*((int)(i/order)) + ((int)(j/order));   //current sub grid number
                value = solution[i][j] - 1; //read in value
                //std::cout << "\t sub_grid = " << sub_grid << "\t" << value << std::endl;
                used_sub_grid[sub_grid][value] = 1;   //mark number in sub grid as used
            }

        }
    }
    
    //* guess remaining grid s.t. sub grid rule is respecterd
    //   i.e. 1-9 in each sub grid
    bool test = 0;
    for (i = 0; i < order*order; i++){      //loop over rows
        for (j = 0; j < order*order; j++){  //loop over cols
            if (fixed_points[i][j] == 0){   //if cell is non fixed
                
                sub_grid = order*((int)(i/order)) + ((int)(j/order));
                test = 0;
                k = 0;      //holds first number not used in the sub grid
                while (test == 0){
                    if (used_sub_grid[sub_grid][k] == 0){
                        used_sub_grid[sub_grid][k] = 1;
                        test = 1;
                    }
                    k++;
                }
                solution[i][j] = k;     //fill in non fixed cell
            }
        }
    }//*/

    //fill cost matrix
    this->update_cost();
}

Sudoku_SA::Sudoku_SA (const Sudoku_SA &P) {
    int i, j;
    order = P.order;
    
    solution     = new int* [order*order];      //create space
    fixed_points = new bool* [order*order];
    cost         = new int* [order*order];
    temperature = P.temperature;        //defined in class

    for (i = 0; i < order*order; i++) {
        solution[i] = new int [order*order];        //create space
        fixed_points[i] = new bool [order*order];
        cost[i] = new int [order*order];
        
        for (j = 0; j < order*order; j++) {
            solution[i][j] = P.solution[i][j];          //copy values
            fixed_points[i][j] = P.fixed_points[i][j];
            cost[i][j] = P.cost[i][j];
        }
    }
}

Sudoku_SA::~Sudoku_SA() {
    //std::cout << "deconstructor\n";
    int i;
    for (i = 0; i < order*order; i++) {
        delete[] solution[i];
        delete[] fixed_points[i];
        delete[] cost[i];
        //delete[] singles[i];
    }
    delete[] solution;
    delete[] fixed_points;
    delete[] cost;
}

/********/

void Sudoku_SA::print_sol(){
    std::cout << "Solution:\n";
    int i, j;
    for (i = 0; i < order*order; i++) {
        for (j = 0; j < order*order; j++) {
            if (solution[i][j] == 0) {
                std::cout << "- ";
            }
            else {
                std::cout << solution[i][j] << " ";
            }
            if (j % order == (order - 1)) {
                std::cout << "\t";
            }
        }
        std::cout << "\n";
        if (i % order == (order - 1)) {
            std::cout << "\n";
        }
    }
}

void Sudoku_SA::print_fixed_points(){
    std::cout << "Fixed Points:\n";
    int i, j;
    for (i = 0; i < order*order; i++) {
        for (j = 0; j < order*order; j++) {
            if (fixed_points[i][j] == 0) {
                std::cout << "- ";
            }
            else {
                std::cout << "1 ";
            }
            if (j % order == (order - 1)) {
                std::cout << "\t";
            }
        }
        std::cout << "\n";
        if (i % order == (order - 1)) {
            std::cout << "\n";
        }
    }
}

void Sudoku_SA::print_cost(){
    std::cout << "Cost:\n";
    int i, j;
    for (i = 0; i < order*order; i++) {
        for (j = 0; j < order*order; j++) {
            if (fixed_points[i][j] == 1) {
                std::cout << "- ";
            }
            else {
                std::cout << cost[i][j] << " ";
            }
            if (j % order == (order - 1)) {
                std::cout << "\t";
            }
        }
        std::cout << "\n";
        if (i % order == (order - 1)) {
            std::cout << "\n";
        }
    }
}

/********/

void Sudoku_SA::update_cost(){
    int i, j, k;
    temperature = 0;        //defined in class
    for (i = 0; i < order*order; i++) {         //loop over rows
        for (j = 0; j < order*order; j++) {     //loop over cols
            cost[i][j] = 0;                     //cost of currnet cell
            if (fixed_points[i][j] != 1){       //if cell is non fixed
                int current_cell = solution[i][j];
                
                for (k = 0; k < order*order; k++){
                    // check row
                    if ((current_cell == solution[i][k]) && (j != k)) {
                        cost[i][j]++;
                        temperature++;
                        //std::cout << "\tNo. " << current_cell << "\t" << solution[i][k] << "\trow \ti = " << i << "\tj = " << j << "\tk = " << k << std::endl;
                    }
                    // check column
                    if ((current_cell == solution[k][j]) && (k != i)) {
                        cost[i][j]++;
                        temperature++;
                        //std::cout << "\tNo. " << current_cell << "\t" << solution[k][j] << "\tcol \ti = " << i << "\tj = " << j << "\tk = " << k << std::endl;
                    }
                }
            }
        }
    }
}

int* Sudoku_SA::sample_full_grid(){
    double CDF[order*order*order*order];
    //double* CDF = new double [order*order*order*order];

    int position = 0;
    int i, j;
    for (i = 0; i < order*order; i++){
        for (j = 0; j < order*order; j++){
            if (fixed_points[i][j] == 1){   //if fixed cell
                if (i == 0 && j == 0){          //if first cell
                    CDF[position] = 0;
                }
                else {
                    CDF[position] = CDF[position-1];
                }
            }
            else {                          //if non fixed cell
                if (i == 0 && j == 0){          //if first cell
                    CDF[position] = cost[i][j] + 1;
                }
                else {
                    CDF[position] = CDF[position-1] + cost[i][j] + 1;
                }
            }
            position++;
        }
    }
    
    //normalization
    double Z = CDF[order*order*order*order-1]; //normalization constant
    for (i = 0; i < order*order*order*order; i++) {
        CDF[i]/=Z;
        //std::cout << CDF[i] << std::endl;
    }
    
    /*  prints the CDF
    std::cout << "CDF:\n";
    for (i = 0; i < order*order*order*order; i++) {
        std::cout << CDF[i] << " ";
        
        if ((i % order) == (order - 1)) {
            std::cout << "\t";
        }
        
        if ((i % (order*order)) == (order*order - 1)) {
            std::cout << "\n";
        }
        if ((i % (order*order*order)) == (order*order*order - 1)) {
            std::cout << "\n";
        }
    }//*/

    std::random_device device;   //random input to sampaler
    std::uniform_real_distribution<double> uniform(0,1);   //U[0,1) sampaler
    float u = uniform(device);
    i = 0;
    while (CDF[i] < u){
        i++;
    }
    
    int* ret = new int [2];   //to pass out of function //need delete outside
    ret[0] = ((int)(i/(order*order)));   //row
    ret[1] = ((int)(i%(order*order)));   //col
    
    return ret;   //needs delete[]
}

int* Sudoku_SA::sample_sub_grid(int* pos_1){
    int sub_grid_1 = order*((int)(pos_1[0]/order)) + ((int)(pos_1[1]/order));
    
    //returns rows and cols of same sub grid
    int** sub_grid_elements = this->same_squ_elements (pos_1[0],pos_1[1]);
    
    int i, j;
    int row, col;
    double CDF[order*order];
    int position = 0;
        
    for (i = 0; i < order; i++){        //loop over rows
        row = sub_grid_elements[0][i];
        for (j = 0; j < order; j++){        //loop over cols
            col = sub_grid_elements[1][j];
            if (fixed_points[row][col] == 1 ||          //don't sample fided point
                (row == pos_1[0] && col == pos_1[1])){  //sample different element
                if (i == 0 && j == 0){
                    CDF[position] = 0;
                }
                else {
                    CDF[position] = CDF[position-1];
                }
            }
            else {
                if (i == 0 && j == 0){
                    CDF[position] = cost[row][col] + 1;
                }
                else {
                    CDF[position] = CDF[position-1] + cost[row][col] + 1;
                }
            }
            position++;
        }
    }
    
    
    //normalization
    double Z = CDF[order*order-1]; //normalization constant
    for (i = 0; i < (order*order); i++) {
        CDF[i]/=Z;
    }
    
    
    /*  print CDF
    std::cout << "CDF_squ " << sub_grid_1 << ":\n";
    for (i = 0; i < order*order; i++) {
        std::cout << CDF[i] << "   ";
        
        if ((i % order) == (order - 1)) {
            std::cout << "\n";
        }
    }
    std::cout << "\n";//*/
    std::random_device device;   //random input to sampaler
    std::uniform_real_distribution<double> uniform(0,1);   //U[0,1) sampaler
    float u = uniform(device);
    i = 0;
    while (CDF[i] < u){
        i++;
    }
    
    int* ret = new int [2];   //to pass out of function //need delete outside
    ret[0] = sub_grid_elements[0][(int)(i/order)];   //row
    ret[1] = sub_grid_elements[1][(int)(i%order)];   //col
    this->delete_same_squ_elements(sub_grid_elements);
    
    return ret;   //needs delete[]
}

/********/ //miscellaneous

int** Sudoku_SA::same_squ_elements(int a, int b) {
    int n;
    if (b == -1) {
        n = a;
    }
    else {
        n = (int)(b / order) + order * (int)(a / order);
    }

    int** ret = new int* [2];        //value to be returned
    ret[0] = new int[order];            //holds the row no's
    ret[1] = new int[order];            //holds the col no's
    
    int i;
    for (i = 0; i < order; i++){
        ret[0][i] = ((int)(n/order))*order + i;
        ret[1][i] = ((int)(n%order))*order + i;
    }

    return ret;
}

void Sudoku_SA::delete_same_squ_elements(int** a) {
    delete[] a[1];
    delete[] a[0];
    delete[] a;
}


int Sudoku_SA::get_cost (int row, int col) {
    return cost[row][col];
}

/*
void Sudoku_SA::copy_cost (Sudoku_SA* P){
    int i, j;
    for (i = 0; i < order; i++){
        for (j = 0; j < order; j++){
            cost[i][j] = P->get_cost(i,j);
        }
    }
    temperature = P->temperature;
}//*/

//*
void Sudoku_SA::copy_cost (Sudoku_SA* P){
    int i, j;
    for (i = 0; i < order*order; i++){
        for (j = 0; j < order*order; j++){
            cost[i][j] = P->get_cost(i,j);
        }
    }
    temperature = P->temperature;
}//*/


void Sudoku_SA::swap (int** points){
    int hold = solution[points[0][0]][points[0][1]];
    solution[points[0][0]][points[0][1]] = solution[points[1][0]][points[1][1]];
    solution[points[1][0]][points[1][1]] = hold;
    
    //this->update_cost ();
}
