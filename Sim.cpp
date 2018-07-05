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

void Sim::Instruction::detType() {
    switch (this->bit32 & 0x0000007F){ //this switches on the opcode of the instruction
        case 0x33:
            this->opcode = 0x33;
            this->type = R;
            break;

        case 0x23:
            this->opcode = 0x23;
            this->type = S;
            break;

        case 0x63:
            this->opcode = 0x63;
            this->type = SB;
            break;

        case 0x37:
            this->opcode = 0x37;
            this->type = U;
            break;

        case 0x6f:
            this->opcode = 0x6f;
            this->type = UJ;
            break;

        default:
            this->type = I;
            switch (this->bit32 & 0x0000007F) {
                case 0x03:
                    this->opcode = 0x03;
                    break;

                case 0x13:
                    this->opcode = 0x13;
                    break;
                default:
                    this->opcode = 0x73;
            }
    }
}

void Sim::Instruction::extract() {
    switch (this->type){
        case R:
            Rextract();
            break;
        case I:
            Iextract();
            break;
        case S:
            Sextract();
            break;
        case SB:
            SBextract();
            break;
        case U:
            Uextract();
        default:
            UJextract();
    }
}

void Sim::Instruction::Rextract() {

    rd = (bit32>>7) & 0x0000001F;
    funct3 = (bit32>>12) & 0x00000007;
    rs1 = (bit32>> 15) & 0x0000001F;
    rs2 =  (bit32 >> 20) & 0x0000001F;
    funct7 = (bit32 >> 25) & 0x0000007F;


}

void Sim::Instruction::Iextract() {

    rd = (bit32>>7) & 0x0000001F;
    funct3 = (bit32>>12) & 0x00000007;
    rs1 = (bit32>> 15) & 0x0000001F;
    I_imm =  (bit32>> 20) & 0x00000FFF;


}

void Sim::Instruction::Sextract() {
    int temp;

    rd = (bit32>>7) & 0x0000001F;
    S_imm = (bit32>>12) & 0x0000001F;
    funct3 = (bit32>> 17) & 0x00000007;
    rs1 = (bit32>> 20) & 0x0000001F;
    rs2 = (bit32 >> 25) & 0x0000001F;

    temp = (bit32 >> 30) & 0x0000007F;
    temp = temp >> 5;
    S_imm = S_imm | temp; //concatenating the immediate value

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
