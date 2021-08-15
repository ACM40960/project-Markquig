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

//#include "sudoku_sa_const.h"
//#include "sudoku_sa_line.h"
#include "sudoku_sa_squ.h"
//#include "sudoku_sa_exp.h"



int main(int argc, const char * argv[]) {
        
    //*
    std::ifstream myfile(argv[1], std::ios_base::in);
    Sudoku_SA su_current(myfile);       //sudoku current solution
    myfile.close();
    Sudoku_SA su_proposed(su_current);  //sudoku proposed next step//*/
    
    double temp;                        //holds current temperature of system
    double temp_start = atof (argv[2]); //temperature at start of loop
    double cool = atof (argv[3]);       //cooling rate
    int count;                          //itteration count
    int count_total = 0;                //count over repeat attempts
    int loop_total = atof (argv[4]);    //number of steps per loop
    //*
    int** points = new int* [2];        //holds sample points
    double accept_rate;                 //holds prob of acceting move with higher cost
    std::random_device device;   //random input to sampaler
    std::uniform_real_distribution<double> uniform(0,1);   //U[0,1) sampaler

    //*
    int min = 20;   //when found solutions are rare, this can print loses cost found
    int accept_count = 0;  //cont number of steps accepted
    while (su_current.temperature != 0){    //loops at most 10 times
        temp = temp_start;          //reset inital temparture
        for (count = 1; count < loop_total; count ++){      //loop for 1 full search
            
            points[0] = su_current.sample_full_grid();          //first point sampled
            points[1] = su_current.sample_sub_grid (points[0]); //second point sampled
            
            su_proposed.swap (points);      //swap values in points
            su_proposed.update_cost ();     //find cost after swap

            accept_rate = std::exp((su_current.temperature -        //prob of accepting
                                    su_proposed.temperature)/temp); // new step

            if (accept_rate >= 1) {  //lower cost accept
                su_current.swap (points);               //swap in current solution
                su_current.copy_cost (&su_proposed);    //find new cost
                accept_count++;                         //increment
            }
            else if (accept_rate >= uniform(device) ) { //if accept higher cost
                su_current.swap (points);
                su_current.copy_cost (&su_proposed);
                accept_count++;
            }
            else {
                su_proposed.swap (points);  //reverse swap
                
            }
            
            delete[] points[0];     //data holding sampled points
            delete[] points[1];     // removed
            
            if (min > su_current.temperature){  //update minimum teperture
                min = su_current.temperature;
            }
            
            if (su_current.temperature == 0){   //solution found
                break;
            }
        }
        count_total += count;

        if (count_total >= 10*loop_total){  //escape after 10 attempts
            std::cout << "+" << 10*loop_total/1000;   //+ to indicate right censored
            //std::cout << min;
            //break;
            delete[] points;
            return -1;  //exit program
        }
    }//*/
    //su_current.print_sol ();
    //su_current.print_cost ();
    //su_current.print_fixed_points ();//*/
    
    //std::cout.precision(4);
    std::cout << (double)count_total/1000;// << std::endl;  //number of steps to solution

    delete[] points;

    
    return 0;
}
