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

        enum Type { R, I, S, SB, U, UJ };

        unsigned int rd, rs1, rs2, funct3, funct7, opcode;
        unsigned int I_imm, S_imm, B_imm, U_imm, J_imm;
        unsigned int address;
        unsigned int bit32;                 //the 32 bit word
        string ins; //instruction in words
        Type type;

        void detType(); //function that determines the type of the instruction and sets the opcode value
        void extract(); //extracts the relevant fields from the intruction and sets them
        void Rextract();
        void Iextract();
        void Sextract();
        void SBextract();
        void Uextract();
        void UJextract();

        void translate(); //function that translates the instruction into true RISC-V instruction

        //load instructions
        void lb ();
        void lh ();
        void lw ();
        void lbu ();
        void lhu ();
        //store instructions
        void sb();
        void sh ();
        void sw ();
        //shift instructions
        void sll();
        //arithmetic instructions
        void add();
        void addi();
        void sub();
        void lui();
        void auipc();
        //logical instructions
        void xOr();
        void xori();
        void Or();
        void ori();
        void aNd();
        void andi();
        //compare instructions
        void slt();
        void slti();
        //branch instructions
        void beq();
        void bne();
        //Jump instructions
        void j();
        void jal();
        //system instructions
        void ecall();



    };


};



#endif //PROJECT_1_SIM_H
