/* 
 * File:   ProcessTrace.cpp
 * Author: Tristan Gay
 * 
 * Created on January 19, 2018, 9:09 PM
 */

#include "ProcessTrace.h"
#include <sstream> //istringstream
#include <algorithm> //transform (putting our command in lowercase)

using namespace std;

ProcessTrace::ProcessTrace(const std::string &file_name) {
    string line; //line we will send to Execute()
    unsigned int lineNumber = 1;
    
    inputFileStream.open(file_name);
    if (inputFileStream.fail()) {
        cerr << "ERROR: file not found: " << file_name << "\n";
        exit(2);
    }
    
    //While our file has another line, execute that line
    while(getline(inputFileStream, line)){
        cout << lineNumber << ":" << line << "\n";
        lineNumber++;
        Execute(line);
    }
}

ProcessTrace::~ProcessTrace() {
    inputFileStream.close();
}

void ProcessTrace::Execute(string line){
    /* Cannot use istringstream to read uint8_t --
    * uint8_t is treated as unsigned char and will be read
    * as such. Must use unsigned ints, or in our case: uint16_t
    * https://stackoverflow.com/questions/25277218/how-to-read-numeric-data-as-uint8-t/25277376 */
    
    istringstream tokenized (line); //to parse our string
    string command; //stores command name
    uint16_t address; //stores address parameter; we must use something larger than uint8_t, and this will work just fine
    uint16_t token; //for reading tokens from istringstream (see note above)
    unsigned int offset; //for adding multiple values to memory
    unsigned int count; //number of times to add a value
    
    tokenized >> command;//first word of line should be the command
    transform(command.begin(), command.end(), command.begin(), ::tolower);//puts our string in lowercase
    
    if(command.compare("alloc") == 0){
        tokenized >> hex >> memorySize; //Get size of memory as hex
        memory.resize(memorySize, 0); //allocate array of memorySize and fill with 0
    } else if(command.compare("compare") == 0){//compare addr expected_values
        vector<uint8_t> expected_values;
        offset = 0;
        tokenized >> hex >> address;
        
        while(tokenized >> token){
            expected_values.push_back(token);
        }

        for(uint8_t t : expected_values){
            if(t != memory.at(address+offset)){
                cerr << "compare error at address " << hex << address+offset <<
                ", expected " << hex << static_cast<int>(t) << ", actual is " << hex << 
                static_cast<int>(memory.at(address+offset)) << endl;
            }
            offset++;
        }
    } else if(command.compare("put") == 0){
        offset = 0;
        tokenized >> hex >> address;
        
        while(tokenized >> token){
            memory.at(address+offset)=token;
            offset++;
        }
    } else if(command.compare("fill") == 0){
        uint16_t value;
        offset = 0;
        tokenized >> hex >> address;
        tokenized >> hex >> count;
        tokenized >> hex >> value;
        
        for(int i = 0; i < count; i++){
            memory.at(address+offset) = value;
            offset++;
        }
    } else if(command.compare("copy") == 0){
        unsigned int dest_addr;
        unsigned int src_addr;
        offset = 0;
        tokenized >> hex >> dest_addr;
        tokenized >> hex >> src_addr;
        tokenized >> hex >> count;
        
        for(int i = 0; i < count; i++){
            memory.at(dest_addr+offset) = memory.at(src_addr+offset);
            offset++;
        }
    } else if(command.compare("dump") == 0){
        unsigned int addr;
        offset = 0;
        tokenized >> hex >> addr;
        tokenized >> hex >> count;
        
        printf("%x\n", addr);
        printVector(memory, addr, count);
    }
}

void ProcessTrace::printVector(vector<uint8_t> v, unsigned int begin, unsigned int count){
    unsigned int i = 0;
    for(vector<uint8_t>::iterator it = v.begin()+begin; it < (v.begin()+begin+count); ++it){
        if(i%16 == 0 && i != 0){
            cout << "\n";
        }
        printf("%02x ", *it);
        i++;
    }
    cout << "\n";
}
