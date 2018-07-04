//
// Created by Laila Nasser ElKoussy 900160812 on 7/4/18.
//

#ifndef PROJECT_1_SIM_H
#define PROJECT_1_SIM_H

#include <iostream>
using namespace std;
int regs[32];
char* memory;

class Sim {
public:
    Sim(char[], int&);
    void run();

private:
    int pc, size;
    struct Instruction {

        unsigned int rd, rs1, rs2, funct3, funct7, opcode;
        unsigned int I_imm, S_imm, B_imm, U_imm, J_imm;
        unsigned int address;
        unsigned int bit32;                 //the 32 bit word
        string ins; //instruction in words

        void translate(); //function that translates the instruction into true RISC-V instruction

        //load instructions
        void lb ();
        //store instructions
        void sb();
        //arithmetic instructions
        void add();
        void addi();
        void sub();

    };


};



#endif //PROJECT_1_SIM_H
