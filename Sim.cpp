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

Sim::Instruction::Instruction() {
    detType();
    translate();
}

void Sim::Instruction::detType() {
    opcode = this->bit32 & 0x0000007F;
    switch (opcode){ //this switches on the opcode of the instruction
        case 0x33:
            this->type = R;
            Rextract();
            break;

        case 0x23:
            this->type = S;
            Sextract();
            break;

        case 0x63:
            this->type = SB;
            SBextract();
            break;

        case 0x37:
        case 0x17:
            this->type = U;
            Uextract();
            break;

        case 0x6f:
        case 0x67:
            this->type = UJ;
            UJextract();
            break;

        default:
            this->type = I;
            Iextract();
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

    if (opcode == 0x3)
        I_imm =  (bit32>> 20) & 0x00000FFF;
    else {
        I_imm = (bit32>>20) & 0x1F;
        funct7 = (bit32>>25) & 0x7F;

    }




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

void Sim::Instruction::UJextract() {
    unsigned int temp;
    if (opcode == 0x6f) // if it's jal
    {   rd = (bit32>>7) & 0x0000001F;
        J_imm = (bit32>>12) & 0x000000FF; // extracting [19:12] immediate
        J_imm = (J_imm<<1); //making place for bit 11
        temp = (bit32>>20) & 0x01; //extracting bit 11
        J_imm = J_imm | temp;
        J_imm= J_imm<<10; //making place for [10:1]
        temp = (bit32>>21) & 0x00003FF;//extracting [10:1]
        J_imm = J_imm | temp;
        temp = (bit32>>31) & 0x0001; //extracting bit 20
        if (temp)
            J_imm = J_imm | 0x80000000; // if temp = 1, adding 1 at beginning of immediate
        J_imm = J_imm<<1;
    }
    else {
        rd = (bit32>>7) & 0x0000001F;
        funct3 = 0;
        rs1 = (bit32>>15) & 0x1F;
        J_imm = (bit32>>20) & 0xFFF;

    }

}

void Sim::Instruction::SBextract() {
    unsigned int temp1, temp2, temp3;
    temp1 = (bit32>>7) & 0x1; //temp 1 immediate bit 11
    temp2 = (bit32>>8) & 0xF; //temp 2 has [4:1]
    funct3 = (bit32>>12) & 0x7;
    rs1 = (bit32>>15) & 0x1F;
    rs2 = (bit32>>20) & 0x1F;
    temp1 = temp1<<6; //making space for [10:5]
    temp3 = (bit32>>25) & 0x3F; //temp has [10:5]
    temp1 = temp1 | temp3; //temp 1 has [11:5]
    temp1 = temp1<<4; //making space for [4:1]
    temp1 = temp2 | temp1; //temp1 has [11:1]
    temp3 = (bit32>>31) & 0x1; //temp3 has bit 12

    if (temp3)
        S_imm = temp1 | 0x80000000; // if temp3 = 1, adding 1 at beginning of immediate





}

void Sim::Instruction::Uextract() {
    rd = (bit32>>7) & 0x1F;
    U_imm = bit32>>12;
    U_imm = U_imm<<12;

}

void Sim::Instruction::translate() {

    switch (type) {

        case R:
            switch (funct3) {
                case 0x0:
                    if (!funct7)
                        ins = "sub x" + rd + ','+ '' + 'x' + rs1 + ',' + ' ' + 'x' + rs2;
                    else ins = "add x" + rd + ',' + '' + 'x' + rs1 + ',' + ' ' + 'x' + rs2;
                    break;
                case 0x1:
                    ins = "sll x" + rd + ',' + '' + 'x' + rs1 + ',' + ' ' + 'x' + rs2;
                    break;
                case 0x2:
                    ins = "slt x" + rd + ',' +  '' + 'x' + rs1 + ',' + ' ' + 'x' + rs2;
                    break;
                case 0x3:
                    ins = "sltu x" + rd + ',' + '' + 'x' + rs1 + ',' + ' ' + 'x' + rs2;
                    break;
                case 0x4:
                    ins = "xor x" + rd + ',' + '' + 'x' + rs1 + ',' + ' ' + 'x' + rs2;
                    break;
                case 0x5:
                    if (!funct7)
                        ins = "srl x" + rd +',' +  '' + 'x' + rs1 +',' +  ' ' + 'x' + rs2;
                    else ins = "sra x" + rd + ',' +  '' + 'x' + rs1 + ',' + ' ' + 'x' + rs2;
                    break;
                case 0x6:
                    ins = "or x" + rd + ',' + '' + 'x' + rs1 + ',' + ' ' + 'x' + rs2;
                    break;
                default:
                    ins = "and x" + rd + ',' + '' + 'x' + rs1 + ',' + ' ' + 'x' + rs2;
                    break;

            }
            break;
        case I:
           if (opcode == 0x3){
               switch (funct3){
                   case 0x0:
                       ins = "lb x" + rd + '' + ',' +rs1+ '(' + 'x' + I_imm + ')';
                       break;

                   case 0x1:
                       ins = "lh x" + rd + '' + ',' +rs1+ '(' + 'x' + I_imm + ')';
                       break;

                   case 0x2:
                       ins = "lw x" + rd + '' + ',' +rs1+ '(' + 'x' + I_imm + ')';
                       break;

                   case 0x4:
                       ins = "lbu x" + rd + '' + ',' +rs1+ '(' + 'x' + I_imm + ')';
                       break;

                   default:
                       ins = "lhu x" + rd + '' + ',' +rs1+ '(' + 'x' + I_imm + ')';
                       break;
               }
           }
               else {

               switch (funct3){
                   case 0x0:
                       ins = "addi x" + rd + ',' + '' + 'x' + rs1 + ',' + ' ' + I_imm;
                       break;

                   case 0x2:
                       ins = "slti x" + rd + ',' + '' + 'x' + rs1 + ',' + ' ' + I_imm;
                       break;

                   case 0x3:
                       ins = "sltiu x" + rd + ',' + '' + 'x' + rs1 + ',' + ' ' + I_imm;
                       break;

                   case 0x4:
                       ins = "xori x" + rd + ',' + '' + 'x' + rs1 + ',' + ' ' + I_imm;
                       break;

                   case 0x6:
                       ins = "ori x" + rd + ',' + '' + 'x' + rs1 + ',' + ' ' + I_imm;
                       break;

                   case 0x7:
                       ins = "andi x" + rd + ',' + '' + 'x' + rs1 + ',' + ' ' + I_imm;
                       break;

                   case 0x1:
                       ins = "slli x" + rd + ',' + '' + 'x' + rs1 + ',' + ' ' + I_imm;
                       break;

                   default:
                       if (!funct7)
                           ins = "srli x" + rd + ',' + '' + 'x' + rs1 + ',' + ' ' + I_imm;
                       else ins = "srai x" + rd + ',' + '' + 'x' + rs1 + ',' + ' ' + I_imm;
                       break;

               }

           }

            break;
        case S:
            switch (funct3){
                case 0x0:
                    ins = "sb x" + rd + '' + ',' +rs1+ '(' + 'x' + S_imm+ ')';
                    break;
                case 0x1:
                    ins = "sh x" + rd + '' + ',' +rs1+ '(' + 'x' + S_imm+ ')';
                    break;
                case 0x2:
                    ins = "sw x" + rd + '' + ',' +rs1+ '(' + 'x' + S_imm+ ')';
                    break;
            }
            break;
        case SB:
            switch(funct3){
                case 0x0:
                    ins = "beq x" + rd + '' + ',' +rs1+ '(' + 'x' + B_imm+ ')';
                    break;

                case 0x1:
                    ins = "bne x" + rd + '' + ',' +rs1+ '(' + 'x' + B_imm+ ')';
                    break;

                case 0x4:
                    ins = "blt x" + rd + '' + ',' +rs1+ '(' + 'x' + B_imm+ ')';
                    break;

                case 0x5:
                    ins = "bge x" + rd + '' + ',' +rs1+ '(' + 'x' + B_imm+ ')';
                    break;

                case 0x6:
                    ins = "bltu x" + rd + '' + ',' +rs1+ '(' + 'x' + B_imm+ ')';
                    break;

                case 0x7:
                    ins = "bgeu x" + rd + '' + ',' +rs1+ '(' + 'x' + B_imm+ ')';
                    break;


            }

            break;
        case U:
            if (opcode == 0x37)
                ins = "lui x" + rd + '' + ',' + U_imm;
            else
                ins = "auipc x" + rd + '' + ',' + U_imm;

            break;
        case UJ:
            if (opcode == 0x6F)
                ins = "jal x" + rd;
            else ins = "jalr x" + rd;
            break;

    }

}

void Sim::Instruction::lb() {
    unsigned int temp1, temp2;
    temp1 = regs[rs1]; //address is in temp1
    temp2 = memory[temp1 + I_imm];//temp2 has byte
    temp1 = temp2 & 0x80; //sign bit is in temp;
    if (temp1)
    { temp2 = temp2 | 0xFFFFFF00; //sign extension
      regs[rd] = temp2; }

    else lbu();
}

void Sim::Instruction::lbu() {
    unsigned int temp1, temp2;
    temp1 = regs[rs1]; //address is in temp1
    temp2 = memory[temp1 + I_imm];//temp2 has byte
    regs[rd] = temp2;

}

void Sim::Instruction::lh() {
    unsigned int temp1, temp2, temp3;
    temp1 = regs[rs1] + I_imm; //address is in temp
    temp2 = regs[temp1+1];//last 4 bits in temp 2
    temp2 = temp2<<4; //temp 2 shifted by one byte
    temp2 = temp2 | regs[temp1]; //temp2 now contains the half word

    temp3 = temp2 & 0x8000; //temp3 contains signed bit

    if (temp3){
        temp2 = temp2 | 0xFFFF0000;
        regs[rd] = temp2;
        makezero();
    }
    else lhu();

}

void Sim::Instruction::lhu() {
    unsigned int temp1, temp2;
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
    unsigned int temp1;
    temp1 = regs[rs1] + S_imm; //address in temp
    memory[temp1] = char(regs[rs2] & 0x0000FFFF);
    memory[temp1+1] = char(regs[rs2]>>16);

}

void Sim::Instruction::sw() {
    unsigned int temp1;

    temp1 = regs[rs1] + S_imm; //address is in temp1
    for (int i=0; i<4; i++){
        memory [temp1 + i] = char(regs[temp1]>>(8*i) & 0xFF);
    }
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
    int temp = 0x80000000 & regs[rs1];
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
    int temp = 0x80000000 & regs[rs1];

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

void Sim::Instruction::lui(){
    regs[rd] = U_imm;
}

void Sim::Instruction::auipc() {
    regs[rd] = memory[pc+U_imm];
    pc+=U_imm;
}

void Sim::Instruction::Or()  {

    regs[rd] = (unsigned int)(regs[rs1]) | regs[rs2];
}

void Sim::Instruction::ori() {

    regs[rd] = (unsigned int)(regs[rs1]) | I_imm;
}

void Sim::Instruction::andi() {

    regs[rd] = (unsigned int)(regs[rs1]) & I_imm;
}

void Sim::Instruction::aNd() {

    regs[rd] = (unsigned int)(regs[rs1]) & regs[rs2];
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
    if (regs[rs1] == regs[rs2])
        pc+= B_imm;
}

void Sim::Instruction::bne() {
    if (regs[rs1]!= regs[rs2])
        pc+= B_imm;
}

void Sim::Instruction::blt() {
    if (regs[rs1]< regs[rs2])
        pc+= B_imm;
}

void Sim::Instruction::bge() {
    if (regs[rs1]>= regs[rs2])
        pc+= B_imm;
}

void Sim::Instruction::bgeu() {
    bge();
}

void Sim::Instruction::bltu() {
    blt();
}





void Sim::Instruction::jal() {
    regs[rd] = pc+4;

    pc += J_imm;
}

void Sim::Instruction::jalr() {
    unsigned int temp;
    regs[rd] = pc + 4; //saving current location in $ra
    temp = J_imm + regs[rs1]; //adding values of J_imm and content of register rs1
    temp = temp & 0xFFFFFFFE; //setting least significant bit to zero
    pc = temp;

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

void Sim::Instruction::makezero() {
    regs[0] = 0;
}