#include "spimcore.h"


/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
    //create switch statements for alu operations

    switch (ALUControl)
    {
    case 0:
        //perform addition
        *ALUresult = A + B;

        break;
    
    case 1:

        //subtraction

        *ALUresult = A-B;
        break;
    case 2:

        //if A < B, Z = 1; otherwise, Z = 0 
        if(A < B){
             *ALUresult = 1;
        }else{
            *ALUresult = 0;
        }
        break;
    
    case 3:
        //if A < B, Z = 1; otherwise, Z = 0 (A and B are unsigned integers)
        if(A < B){
             *ALUresult = 1;
        }else{
            *ALUresult = 0;
        }
        break;
    case 4:
        //Z = A AND B
        *ALUresult = A & B; 
        break;
    case 5:
        //Z = A OR B 
        *ALUresult = A | B;
        break;
    case 6:
        //Z = Shift B left by 16 bits
        *ALUresult = B<<16;
        break;
    case 7:
        //Z = NOT A 
        *ALUresult = ~A;
        break;
    default:
        break;
    }

    if(ALUresult == 0){
        Zero = 1;
    }
}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
    //need to divide pc by four to get the index for mem
    if(PC>>2 > 65536>>2){
        return 1;
    }
    
    *instruction = Mem[PC/4];
    return 0;
}


/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
    //implement some form of bit masking to extract the bits from istruction

    
}



/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{

}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{

}


/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{

}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{

}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{

}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{

}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{
    //to access proper memory index need to divide pc by four 

    

}

