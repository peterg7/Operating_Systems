/* 
 * File:   main.cpp
 * Author: Peter Gish
 *
 * Created on February 10, 2018, 5:23 PM
 */

/*
 * The program should accept three arguments:
 * 1) input file name
 * 2) block_duration: the decimal integer time length that a process
 *                    is unavailable to fun after it blocks
 * 3) time_slice: the decimal integer of the time slice for the 
 *                Round-Robin scheduler
 * -Arguments are passed in the order shown above
 * 
 */

#include "Scheduler.h"

#include <cstdlib>
#include <iostream>
#include <sstream>

#include "Scheduler.h"


int main(int argc, char** argv) {
    if (argc != 4) {
        std::cerr << "usage: Assignment1 input_file\n";
        exit(1);
    }
    std::istringstream ss1(argv[2]);
    int block_duration;
    if (!(ss1 >> block_duration))
        std::cerr << "Invalid argument1 " << argv[2] << '\n';
    std::istringstream ss2(argv[3]);
    int time_slice;
    if (!(ss2 >> time_slice))
        std::cerr << "Invalid argument " << argv[3] << '\n';

    Scheduler s(argv[1], block_duration, time_slice); //create scheduler object and pass in command line arguments

    return 0;
}

