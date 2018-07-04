//
// Created by Laila Nasser ElKoussy 900160812 on 7/4/18.
//

#include "Sim.h"


Sim::Sim(char a[], int &size) {
    this->size = size;
    memory = a; // passed memory by reference
    pc = 0;
    for (auto element : regs)
        element = 0;            //initializing array with all zeros


}

void Sim::Instruction::translate() {

}

void Sim::Instruction::lb() {
    int temp;
    temp = regs[rs1]; //address is in temp
    regs[rd] = memory[temp + I_imm];
}

void Sim::Instruction::sb() {
    int temp;
    temp = regs[rs2]; //address is in temp
    memory[temp+ S_imm] = regs[rs1];

}

void Sim::Instruction::add() {
    regs[rd] = regs[rs1] + regs[rs2];

}

void Sim::Instruction::addi() {
    regs[rd] = regs[rs1] + I_imm;

}

void::Sim::Instruction::sub() {
    regs[rd] = regs[rs1] - regs[rs2];
}

void Sim::run() {
    while (pc<size){


    }
}
