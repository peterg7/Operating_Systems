/* 
 * File:   main.cpp
 * Author: Tristan Gay, Peter Gish, Chenchen Mao
 *
 * Created on January 27, 2018, 3:06 PM
 */

#include <sstream> //istringstream
#include <fstream> //filestream
#include <iostream> //cout, cerr
#include "PageFrameAllocator.h"

using std::cout;
using std::cerr;
using std::getline;
using std::istringstream;
using std::string;
using std::hex;
using std::vector;

/**
 * parseCommand         parses a line and executes commands on pageframeallocator
 * @param line          contains commands and arguments to pass to pageframeallocator
 * @param pf            reference to our PageFrameAllocator
 * @param page_frames   reference to our vector of page frames
 */
void parseCommand(string &line, PageFrameAllocator &pf, vector<uint32_t> &page_frames);

/**
 * readFile            opens our file, prints the command, and passes commands
 * @param file_name    file to read from
 * @param page_frames  reference to vector containing our allocated page frames
 */
void readFile(string &file_name, vector<uint32_t> &page_frames);

int main(int argc, char** argv) {
    vector<uint32_t> allocated_pages; //our vector of allocated page frames
    
    //Use command line argument as file name
    if (argc != 2) {
        cerr << "usage: Lab3 file\n";
        exit(1);
    }
    string file = argv[1];
  
    readFile(file, allocated_pages);
    return 0;
}

void parseCommand(string &line, PageFrameAllocator &pf, vector<uint32_t> &page_frames){
    istringstream tokenized (line); //tokenizes a string
    uint32_t command; //first value on line is a command
    uint32_t arg; //second value will be an argument
    bool retVal; //gets the return value of our de/allocate methods
    
    //entire file is to be read as hex values
    //store this token as a hex value, otherwise it is stored as a decimal value
    tokenized >> hex >> command; //read first value from line
    
    if(command == 0){ //0 = deallocate
        tokenized >> hex >> arg;
        printf(">%x %x\n", command, arg);
        retVal = pf.Deallocate(arg, page_frames);
        if(retVal){ //successful deallocation
            printf(" T %x\n", pf.get_page_frames_free());
        } else {
            printf(" F %x\n", pf.get_page_frames_free());
        }
    } else if(command == 1){ //1 = allocate
        tokenized >> hex >> arg;
        printf(">%x %x\n", command, arg);
        retVal = pf.Allocate(arg, page_frames);
        if(retVal){ //successful allocation
            printf(" T %x\n", pf.get_page_frames_free());
        } else {
            printf(" F %x\n", pf.get_page_frames_free());
        }
    } else if (command == 2){ //2 = print free list
        printf(">2\n");        
        for(int i = 0; i < pf.get_page_frames_free(); i++){
            printf(" %x", pf.get_free_list_head()+i);
        }
        cout << "\n";
    }
}

void readFile(string &file_name, vector<uint32_t> &page_frames){
    std::ifstream inputFileStream; //file stream for our file
    inputFileStream.open(file_name); //open file
    bool isFirstLine = true;
    uint32_t size;
    string line; 
    
    if (inputFileStream.fail()) { //Verify file read was successful
        cerr << "ERROR: failed to open input file: " << file_name << "\n";
        exit(2);
    }
    
    //First line of file should be number of page frames to allocate
    if(getline(inputFileStream, line) && isFirstLine){
        istringstream tokenized (line);
        tokenized >> hex >> size;
        printf(">%x\n", size);
    }
    isFirstLine = false;
    PageFrameAllocator pf(size);
    
    //Pass the rest of the file to parseCommand
    while(getline(inputFileStream, line)){
        parseCommand(line, pf, page_frames);
    }
    
}
