/*
 * Scheduler class to implement Round-Robin and Shortest Process Next
 * scheduling algorithms 
 */

/*
 * -All numeric values in the input file are decimal integers
 * (Time unit doesn't matter)
 * -Input file contains 1 line per process
 * -Lines are sorted in increasing order of arrival time in the system
 * 
 * -Line format --> name arrival_time total_time block_interval
 *  name: a sequence of non-blank characters representing the name of the process
 *  arrival_time: the time at which the process arrives in the system
 *  total_time: the total amount of CPU time which will be used by the process
 *  block_interval: interval at which will block for I/O. When a process blocks,
 *                  it is unavailable to run for the time specified by block_duration
 *                  in the scheduler parameter file
 * 
 * OUTPUT: --> all output should be written to standard output
 * For each scheduling algorithm:
 * - A single line with the name of the scheduling algorithm (RR or SPN),
 *   followed by the block_duration (for both algorithms) and time_slice (for RR)
 *   as specified on the command line. Values should be separated by spaces.
 * - One line for each interval during which a process is running or the system is 
 *   idle. The line should consist of a single space, followed by the current simulation
 *   time (starting at 0), followed by the process name (or "<idle>" if no process is
 *   running), the length of the interval and a status code:
 *      -"B" for blocked
 *      -"S" for time slice ended
 *      -"T" if the process terminated
 *      -"I" for an idle interval
 *   The fields should be separated by the tab character, '\t'.
 * - After all jobs have terminated, write a line consisting of a single space, the 
 *   simulation time at which the last job terminated, a tab character, the string
 *   "<done>", another tab character, and the average turnaround time of all
 *   processes (floating point value)
 * 
 * Turnaround Time: termination time - arrival time
 */

/* 
 * File:   Scheduler.h
 * Author: Peter Gish
 *
 * Created on February 10, 2018, 5:36 PM
 */


#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <queue>
#include <vector>
#include <string>
#include <fstream>

class Scheduler {
public:
    /**
     * Constructor - initialize processing
     */
    Scheduler(std::string file_name_, int block_duration, int time_slice);

    /**
     * Destructor - clean up processing
     */
    virtual ~Scheduler();

    /**
     * Rule of 5:
     * All other constructors/assignments are not needed
     */
    Scheduler(const Scheduler &other) = delete;
    Scheduler(Scheduler &&other) = delete;
    Scheduler operator=(const Scheduler &other) = delete;
    Scheduler operator=(Scheduler &&other) = delete;



private:

    /**
     * struct to hold all necessary information about a process
     */
    struct Process {
        std::string name; //name of process
        int arrival_time; //arrival time of process in system
        int total_time; //total time needed for process to run
        int block_interval; //interval of time process blocks for I/O
        int remaining_time; //might be useful for SPN
        int termination_time; //might be useful for computing average turnaround time
        int time_blocked; //keeps track of how long the process has been blocked for OR how long until the next block
        int time_until_blocked;
        bool is_blocked; //holds the block status of the process
        
        /**
         * Implements less than operator for the priority queue
         * ****Sorts Process(es) based on remaining_time or block interval time, whichever is shortest****
         * @param x
         * @return 
         */
        bool operator<(const Process& x) const {
            if(remaining_time < block_interval){
                if(x.remaining_time < x.block_interval){
                    //Sort both by remaining_time
                    return remaining_time < x.remaining_time;
                } else {
                    //Sort this process by remaining time and x process by block interval
                    return remaining_time < x.block_interval;
                } 
            } else {
                if(x.remaining_time < x.block_interval){
                    //Sort this process by block interval and x process by remaining time
                    return block_interval < x.remaining_time;
                } else {
                    //Sort both processes by block_interval
                    return block_interval < x.block_interval;
                }
            }
        }
    };


    int BLOCK_DURATION; //decimal integer time length a process is unavailable to run after it blocks
    int TIME_SLICE; //decimal integer length of time slice for RoundRobin algorithm 

    /**
     * Extracts information from input file (allocates data into Process structs)
     * -Name: sequence of non-blank characters for name of process
     * -Arrival Time: time at which the process arrives
     * -Total Time: total amount of CPU time the process needs
     * -Block Interval: interval at which a process blocks for I/O
     * Format:
     *  name arrival_time total_time block_interval
     * 
     * -All numeric values are decimal integers
     * -1 line per process (formatted as shown above)
     * @param file_name_
     */
    std::vector<Process> ParseFile(std::string file_name_);
    
    
    /**
     * Function to call both scheduling algorithms
     * Passes the vector of processes read from the ParseFile method to both
     * algorithms 
     * @param processes
     */
    void Execute(std::vector<Scheduler::Process> processes);

    /*****
     * For both algorithms below, when a process re-enters the ready queue
     * after being blocked or entering the system should be placed on the 
     * end of the queue
     ******/
    /**
     * Round Robin scheduling algorithm implementation:
     * 
     * -Scheduler keeps a circular list of processes 
     * -Scheduler runs periodically or when a process blocks (period = time_slice)
     * -Each time scheduler runs it gives the CPU to the next process in the
     *  circular list
     * (Smaller time slice = better response time but reduces CPU efficiency)
     * (Larger time slice decreases the total amount of process switch overhead)
     */
    void RoundRobin(std::vector<Process> processes);

    /**
     * Shortest Process Next scheduling algorithm implementation:
     * 
     * -Uses a "prediction value" to determine which process to run next
     * -Instead of making predictions, use the block_interval (or the total time
     *  left, whichever is shortest) of the processes in the ready list to determine 
     *  which process to run next
     */
    void ShortestProcessNext(std::vector<Process> processes);

    /**
     * Computes the average turnaround time for a given scheduling algorithm
     * Turnaround time = termination time - arrival time
     * @return 
     */
    float AverageTurnaroundTime(std::vector<Process> processes);
    
    /**
     * Sets the currentIndex to the next valid index of the process list
     * (wraps the list)
     * @param currentIndex
     * @param numProcesses
     */
    void getNextIndex(int& currentIndex, int& numProcesses);
};

#endif /* SCHEDULER_H */

