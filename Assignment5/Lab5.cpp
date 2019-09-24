// Lab 5 - COMP3361 Operating Systems Winter 2018
//
// Implementation of deadlock detection algorithm.
// Code is provided to read a system configuration
// from the input file (specified by the program
// parameter). You should fill in code as noted by
// the "TODO" blocks to detect deadlocks. See the
// assignment for details on the output format.
//
// Template author: Mike Goss (mikegoss@cs.du.edu)
//
// Student names: Peter Gish, Tristan Gay, Chenchen Mao

#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>

using std::cin;
using std::cout;
using std::cerr;
using std::ifstream;
using std::setw;

namespace {

    // Define maximum supported data sizes.
    const int kMaxProcesses = 50; // maximum number of processes
    const int kMaxResources = 50; // maximum number of resources

    // Define arrays for algorithm
    int Available[kMaxResources];
    int Allocation[kMaxProcesses][kMaxResources];
    int Request[kMaxProcesses][kMaxResources];
    std::vector<int> DeadLocked; //Keeps track of the deadlocked processes

    int numProcesses; // actual number of processes
    int numResources; // actual number of resources

    

    // AddToAvailable - add each element in Allocation[i] to corresponding
    //   element in Available
    //
    // @param i - index of row in Allocation array
    void addAvailable(int i) {
        for (int j = 0; j < kMaxResources; j++) {
            Available[j] += Allocation[i][j];
        }
    }

    // IsRequestLessEqual - compare row of Request array to Available
    //
    // @param i - index of row in Request array
    // @returns true if every element of Request[i] is <= corresponding element
    //          of Available, false otherwise.
    bool lessEqual(int i) {
        bool result = true;
        int check = 0;
        for (int j = 0; j < kMaxResources; j++) {
            if (Request[i][j] <= Available[j]) {
                ++check;
            }
        }
        if (check != kMaxResources) {
            result = false;
        }
        return result;
    }

    void SystemUpdate() {
        /* Unmark all processes*/
        for (int i = 0; i < numProcesses; ++i) {
            DeadLocked.push_back(i);
        }
        
        /* Go through every unmarked process and check
         * for deadlock
         */
        for(int i = 0; i < numProcesses; ++i){ 
            if (lessEqual(i) && DeadLocked.at(i) != -1) {
                addAvailable(i);
                DeadLocked.at(i) = -1;
                i = 0; //Need to go back over every unmarked process 
                       //since Available has been updated
            }
        }
    }

    // PrintDeadlocks - print indices of deadlocked processes
    //
    void PrintDeadlocks(void) {
        cout << "Deadlocked processes:      ";
        SystemUpdate();
        for (int process : DeadLocked) {
            if(process != -1){ //Only print out unmarked processes
                cout << process << "   ";
            }
        }
    }

    // ReadSystemConfig - read the system configuration from the
    //   input file, specified in argv[1].

    void ReadSystemConfig(const char *fileName) {
        // Clear the configuration arrays
        for (int j = 0; j < kMaxResources; ++j) {
            Available[j] = 0;
            for (int i = 0; i < kMaxProcesses; ++i) {
                Allocation[i][j] = 0;
                Request[i][j] = 0;
            }
        }

        ifstream in;
        in.open(fileName);
        if (!in.is_open()) {
            cerr << "ERROR: input file not found: " << fileName << "\n";
            exit(2);
        }

        // Read the actual number of processes and resources
        in >> numProcesses >> numResources;
        if (in.fail()) {
            cerr << "ERROR: failed to read numProcesses and numResources\n";
            exit(2);
        }
        if (numProcesses <= 0 || numProcesses > kMaxProcesses
                || numResources <= 0 || numResources > kMaxResources) {
            cerr << "ERROR: out of range value, numProcesses = " << numProcesses
                    << "numResources = " << numResources << "\n",
                    exit(2);
        }
        cout << numProcesses << " processes, " << numResources << " resources\n";
        // Read the Available array
        for (int i = 0; i < numResources; ++i) {
            in >> Available[i];
            if (in.fail()) {
                cerr << "ERROR: failed to read Available[" << i << "]\n";
                exit(2);
            }
        }
        cout << "Available:\n\t";
        for (int i = 0; i < numResources; ++i) {
            cout << setw(3) << Available[i];
        }
        cout << "\n";

        // Read the Allocation array
        for (int i = 0; i < numProcesses; ++i) {
            for (int j = 0; j < numResources; ++j) {
                in >> Allocation[i][j];
                if (in.fail()) {
                    cerr << "ERROR: failed to read Alocation[" << i << "][" << j << "]\n";
                    exit(2);
                }
            }
        }
        cout << "Allocation:\n";
        for (int i = 0; i < numProcesses; ++i) {
            cout << "\t";
            for (int j = 0; j < numResources; ++j) {
                cout << setw(3) << Allocation[i][j];
            }
            cout << "\n";
        }

        // Read the Request array
        for (int i = 0; i < numProcesses; ++i) {
            for (int j = 0; j < numResources; ++j) {
                in >> Request[i][j];
                if (in.fail()) {
                    cerr << "ERROR: failed to read Request[" << i << "][" << j << "]\n";
                    exit(2);
                }
            }
        }
        cout << "Request:\n";
        for (int i = 0; i < numProcesses; ++i) {
            cout << "\t";
            for (int j = 0; j < numResources; ++j) {
                cout << setw(3) << Request[i][j];
            }
            cout << "\n";
        }
    }

} // namespace

int main(int argc, char *argv[]) {
    // Read system configuration
    if (argc != 2) {
        cerr << "usage: lab5 filename\n";
        exit(1);
    }
    ReadSystemConfig(argv[1]);
    PrintDeadlocks();

    return 0;
}