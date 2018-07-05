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
    temp = temp << 5;
    S_imm = S_imm | temp; //concatenating the immediate value

}

void Sim::Instruction::translate() {

}

void Sim::Instruction::lb() {
    int temp;
    temp = regs[rs1]; //address is in temp
    regs[rd] = memory[temp + I_imm];
}

void Sim::Instruction::lh() {
    int temp1, temp2;
    temp1 = regs[rs1] + I_imm; //address is in temp
    temp2 = regs[temp1+1];//last 4 bits in temp 2
    temp2 = temp2<<4; //temp 2 shifted by one byte
    temp2 = temp2 | regs[temp1]; //temp2 now contains the half word
    regs[rd] = temp2;


}

void Sim::Instruction::lw() {
    int temp1, temp2, temp3;
    temp1 = regs[rs1] + I_imm; //address in temp 1

    for (int i = 4; i<0; i--)
    { temp3 = temp3<<4;
      temp2 = regs[temp1+i-1]; //taking the bytes in from the left
      temp3 = temp3 | temp2;
    }

}

void Sim::Instruction::sb() {
    int temp;
    temp = regs[rs2]; //address is in temp
    memory[temp+ S_imm] = char(regs[rs1]);

}

void Sim::Instruction::sh(){
    int temp1;
    temp1 = regs[rs2] + S_imm; //address in temp
    memory[temp1] = char(regs[rs1] & 0x0000FFFF);
    memory[temp1+1] = char(regs[rs1]>>16);

}

void Sim:: Instruction:: sll()
{
    regs[rd] = (unsigned int)regs[rs1]<< (unsigned int)regs[rs2];
}
void Sim:: Instruction:: slli()
{
    regs[rd] = (unsigned int)regs[rs1] << (unsigned int)I_imm;
}
void Sim:: Instruction:: srl()
{
    regs[rd] = (unsigned int)regs[rs1] >> (unsigned int)regs[rs2];
}
void Sim:: Instruction:: srli()
{
    regs[rd] = (unsigned int)regs[rs1] >> (unsigned int)I_imm;
}
void Sim:: Instruction:: sra()
{
    int temp= 1 & regs[rs1];
    if (temp)
    {
        for (int i=0; i<regs[rs2]; i++)
            regs[rd] = (regs[rs1]>>1 | 0x80000000);

    }
    else
        srl();
}
void Sim:: Instruction:: srai()
{
    int temp= 1 & regs[rs1];
    if (temp)
    {
        for (int i=0; i<I_imm; i++)
            regs[rd] = (regs[rs1]>>1 | 0x80000000);

    }
    else
        srl();
}

void Sim::Instruction::add() {
    regs[rd] = regs[rs1] + regs[rs2];

}

void Sim::Instruction::addi() {
    regs[rd] = regs[rs1] + I_imm;

}

void Sim::Instruction::sub() {

    regs[rd] = regs[rs1] - regs[rs2];
}

void Sim::Instruction::Or()  {

    regs[rd] = unsigned int(regs[rs1]) | regs[rs2];
}

void Sim::Instruction::ori() {

    regs[rd] = unsigned int(regs[rs1]) | I_imm;
}

void Sim::Instruction::andi() {

    regs[rd] =unsigned int(regs[rs1]) & I_imm;
}

void Sim::Instruction::aNd() {

    regs[rd] = unsigned int(regs[rs1]) & regs[rs2];
}

void Sim::Instruction::xOr() {

    regs[rd] = unsigned int(regs[rs1])^ regs[rs2];
}

void Sim::Instruction::xori() {

    regs[rd] = unsigned int(regs[rs1]) ^ I_imm;
}

void Sim::Instruction::slt() {
    if (regs[rs1]<regs[rs2])
        regs[rd] = 1;
    else regs[rd] = 0;

}

void Sim::Instruction::slti() {
    if (regs[rs1]<I_imm)
        regs[rd] = 1;
    else regs[rd] = 0;

}

void Sim::Instruction::beq() {
    if (rs1 == rs2)
        pc+= B_imm;
}

void Sim::Instruction::bne() {
    if (rs1 != rs2)
        pc+= B_imm;
}

void Sim::Instruction::j() {
    pc = J_imm;
}

void Sim::Instruction::jal() {
    regs[1] = pc; //saving current location in $ra
    pc = J_imm;

}

void Sim::Instruction:: ecall()
{
    switch (regs[17])
    {
        case 1:
            cout<< hex<< regs[2];
            break;
        case 4:
            cout<< memory[regs[2]];
            break;
        case 10:
            running = false;
            break;
    }
}