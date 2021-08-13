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

#include "sudoku_sa_const.h"
//#include "sudoku_sa_line.h"
//#include "sudoku_sa_squ.h"
//#include "sudoku_sa_exp.h"



int main(int argc, const char * argv[]) {
        
    //std::ifstream myfile("input_easy1.txt", std::ios_base::in);
    //*
    std::ifstream myfile(argv[1], std::ios_base::in);
    Sudoku_SA su_current(myfile);       //sudoku current solution
    myfile.close();
    Sudoku_SA su_proposed(su_current);  //sudoku proposed solution//*/
    
    //su_current.print_sol();
    //su_proposed.print_sol();
    
    //su_current.print_cost();
    //su_proposed.print_cost();
    
//    su_current.print_fixed_points();
//    su_proposed.print_fixed_points();

//    su_current.print_CDF();
//    su_proposed.print_CDF();
//    su_current.print_CDF_squ (5);
//    su_proposed.print_CDF_squ (6);
    
    //std::cout << "temperature = " << su_current.temperature << std::endl;
    //std::cout << "temperature = " << su_proposed.temperature << std::endl;
    
    double temp;   //holds current temperature of system
    double temp_start = atof (argv[2]);   //temperature at start of loop
    double cool = atof (argv[3]);
    int count;       //itteration count
    int count_total = 0;
    int loop_total = atof (argv[4]);
    //*
    int** points = new int* [2];
    double accept_rate;
    std::random_device device;   //random input to sampaler
    std::uniform_real_distribution<double> uniform(0,1);   //U[0,1) sampaler

    
//    points[0] = su_proposed.sample_full_grid ();
//    points[1] = su_proposed.sample_sub_grid (points[0]);
//
//    std::cout << "points[0] = " << points[0][0] << ", " << points[0][1] << "\n";
//    std::cout << "points[1] = " << points[1][0] << ", " << points[1][1] << "\n";
//
//    su_proposed.swap (points);
//    su_proposed.update_cost ();
//
//    std::cout << "temperature = " << su_current.temperature << std::endl;
//    std::cout << "temperature = " << su_proposed.temperature << std::endl;
//
//    delete[] points[0];
//    delete[] points[1];

    //*
    int min = 20;
    int accept_count = 0;
    while (su_current.temperature != 0){
        temp = temp_start;
        for (count = 1; count < loop_total; count ++){
            
            points[0] = su_current.sample_full_grid();
            points[1] = su_current.sample_sub_grid (points[0]);
            
            su_proposed.swap (points);
            su_proposed.update_cost ();
            //su_proposed.print_sol();
            //su_proposed.print_cost();
            //std::cout << "temperature = " << su_proposed.temperature << std::endl;

            temp = temp*cool;
            accept_rate = std::exp((su_current.temperature - su_proposed.temperature)/temp);
            //std::cout << "rate = " << accept_rate << std::endl;
            //std::cout << su_current.temperature << "\t" << su_proposed.temperature << std::endl;

            if (accept_rate >= 1) {  //accept
                su_current.swap (points);
                su_current.copy_cost (&su_proposed);
                accept_count++;
            }
            else if (accept_rate >= uniform(device) ) {
                su_current.swap (points);
                su_current.copy_cost (&su_proposed);
                accept_count++;
            }
            else {
                su_proposed.swap (points);  //reverse swap
                //su_proposed.copy_cost (su_current);
            }

            
            delete[] points[0];
            delete[] points[1];
            
            if (min > su_current.temperature){
                min = su_current.temperature;
            }
            
            if (su_current.temperature == 0){
                break;
            }
            //std::cout << "current temp = " << su_current.temperature << "\t prop temp = " << su_proposed.temperature << std::endl;
            //std::cout << "\trate = " << accept_rate << std::endl;
            //std::cout << "\ttemperature = " << su_current.temperature << std::endl;
            //std::cout << std::endl;
        }
        count_total += count;
        //std::cout << "\tcount = " << count << std::endl;
        //std::cout << (double)count_total/1000 << std::endl;
        if (count_total >= 1*loop_total){
            //std::cout << "+" << 10*loop_total/1000;   //+ to indicate right censored
            std::cout << min;
            //break;
            return -1;
        }
//        std::cout << "temperature = " << su_current.temperature << std::endl;
//        std::cout << "accept_count = " << accept_count << std::endl;
//        su_current.print_sol ();
//        su_current.print_cost ();
//        std::cout << "\n\n";
        
    }//*/
    //*
    su_current.print_sol ();
    //su_current.print_cost ();
    //su_current.print_fixed_points ();//*/
    /*
    std::cout << "temperature = " << su_current.temperature << std::endl;
    std::cout << "count = " << count_total << std::endl;
    std::cout << std::endl;//*/
    
    //std::cout.precision(4);
    std::cout << (double)count_total/1000;// << std::endl; //for bash script to asses multiple runtimes

    
    delete[] points;

    
    return 0;
}
