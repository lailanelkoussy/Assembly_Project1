/*
	This is just a skeleton. It DOES NOT implement all the requirements.
	It only recognizes the "ADD", "SUB" and "ADDI"instructions and prints
	"Unkown Instruction" for all other instructions!

	References:
	(1) The risc-v ISA Manual ver. 2.1 @ https://riscv.org/specifications/
	(2) https://github.com/michaeljclark/riscv-meta/blob/master/meta/opcodes
*/

#include <iostream>
#include <fstream>
#include "stdlib.h"
#include <iomanip>
#include "Sim.h"


using namespace std;



void emitError(const string &s)
{
    cout << s;
    exit(0);
}



int main(int argc, char *argv[]){

    unsigned int instWord=0;
    ifstream input;
    int size;
    char temp;

    if(argc<1) emitError("use: rvsim <machine_code_file_name>\n");

    input.open(argv[1], ios::in | ios::binary); //file name is in argv[1], it's a binary file

    for (int i = 0; (input.good()) && i < 64*1024; i++) {
    input>>temp;
    memory[i]= temp;
    size = i+1;
    }

    input.close(); //memory is now filled with all instructions
    Sim simu(memory,size); //the simulator is constructed using the memory and memory size


    return 0;
}
