//
// Created by Laila Nasser ElKoussy 900160812 on 7/4/18.
//

#ifndef PROJECT_1_SIM_H
#define PROJECT_1_SIM_H

#include <iostream>
using namespace std;
int regs[32];
char* memory;
unsigned int pc;
bool running = true;

class Sim {
public:
    Sim(char[], int&);
    void run();

private:
    int size;
    struct Instruction {

        enum Type { R, I, S, SB, U, UJ };

        unsigned int rd, rs1, rs2, funct3, funct7, opcode;
        unsigned int I_imm, S_imm, B_imm, U_imm, J_imm;
        unsigned int bit32;                 //the 32 bit word
        string ins; //instruction in words
        Type type;

        void detType(); //function that determines the type of the instruction and sets the opcode value, then calls relevant extract function
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
        void slli();
        void srl();
        void srli();
        void sra();
        void srai();
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
        void jal();
        void jalr();
        //system instructions
        void ecall();

        void makezero(); //function that makes sure x0 is zerox


    };


};



#endif //PROJECT_1_SIM_H
