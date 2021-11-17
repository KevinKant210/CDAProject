#include "spimcore.h"


/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
    //Wondering if we need to worry about the overflow during operations

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
    
    //Jsec is address for j types
    //func is function for R-format
    //offset is constant/address for I types

    //R-format  OP (6 bit) - RS 5 bit - rt 5bit - rd 5 bit - shamt 5 bit - funct 6 bit // 0 opccode
    //I-format op 6 bit - rs 5 bit - rt 5 bit - address/immediate 16 bits   //opcode > 3
    //J format op 6bits - target address 26 bits    //opcode of 2 or 3
    /*
        Partition instruction into several parts (op, r1, r2, r3, funct, offset, jsec). 
        2.  Read line 41 to 47 of spimcore.c for more information.
    */

   // mask for op code 11111100000000000000000000000000
   unsigned maskOpcode = fromBinary("11111100000000000000000000000000", 32);
   
   //remeber to shift the bits over!
   unsigned opCode = (maskOpcode & instruction) >> (32-26);

   if(opCode == 0){
       //r-format instruciton
        //bit mask for rs 00000011111000000000000000000000
        unsigned maskRS = fromBinary("00000011111000000000000000000000", 32);
        unsigned maskRT = fromBinary("00000000000111110000000000000000",32);
        unsigned maskRD = fromBinary("0000000000000000111110000000000",32);
        //no shamt pointer so assume this is unneccesary
        //unsigned maskShamt = fromBinary("00000000000000000000011111000000",32);
        unsigned maskFunct = fromBinary("00000000000000000000000000111111",32);

        
        *r1 = (maskRS & instruction) >> (32-21);
        *r2 = (maskRT & instruction) >> (32-16);
        *r3 = (maskRD & instruction) >> (32-11);
        //at the end so no need to shift
        *funct = (maskFunct & instruction);



   }else if(opCode > 3){

       // I format instruction
        unsigned maskRS = fromBinary("00000011111000000000000000000000", 32);
        unsigned maskRT = fromBinary("00000000000111110000000000000000",32);
        unsigned maskAddress = fromBinary("00000000000000001111111111111111",32);

        *r1 = (maskRS & instruction) >> (32-21);
        *r2 = (maskRT & instruction) >> (32-16);
        //at the end so no need to shift
        *offset = (maskAddress & instruction);
   }else{
       //j format instruction
       unsigned maskAddressLong = fromBinary("00000011111111111111111111111111",32);
        //at the end so no need to shift
        *jsec = (maskAddressLong & instruction);
   }
    

}



/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{   
    /*
        Decode the instruction using the opcode (op). 
2.  Assign the values of the control signals to the variables in the structure controls 
(See spimcore.h file). 
The meanings of the values of the control signals: 
For MemRead, MemWrite or RegWrite, the value 1 means that enabled, 0 
means that disabled, 2 means “don’t care”. 
For RegDst, Jump, Branch, MemtoReg or ALUSrc, the value 0 or 1 indicates the 
selected path of the multiplexer; 2 means “don’t care”. 
The following table shows the meaning of the values of ALUOp. 
 
3.  Return 1 if a halt condition occurs; otherwise, return 0. 
    */

}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
    //Read the registers addressed by r1 and r2 from Reg, and write the read values 
    //to data1 and data2 respectively.

    *data1 = Reg[r1];
    *data2 = Reg[r2];
}


/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{
    //Assign the sign-extended value of offset to extended_value. 
    
    // Shift the offset over 15 bits
    unsigned sign = offset >> 15;
    if(sign == 1){
        extended_value = (0xFFFF0000 | offset);
        return;
    }
    
    extended_value = offset;
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
        /*
            Apply ALU operations on data1, and data2 or extended_value (determined by 
    ALUSrc). 
    2.  The operation performed is based on ALUOp and funct. 
    3.  Apply the function ALU(...). 
    4.  Output the result to ALUresult. 
    5.  Return 1 if a halt condition occurs; otherwise, return 0.
        */
}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{
        /*
            1.  Use the value of MemWrite or MemRead to determine if a memory write 
    operation or memory read operation or neither is occurring. 
    2.  If reading from memory, read the content of the memory location addressed by 
    ALUresult to memdata. 

    3.  If writing to memory, write the value of data2 to the memory location 
    addressed by ALUresult. 
    4.  Return 1 if a halt condition occurs; otherwise, return 0.
        */
}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
    /*
        1.  Write the data (ALUresult or memdata) to a register (Reg) addressed by r2 or r3. 
    */

}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{
    //1.  Update the program counter (PC). 
    //to access proper memory index need to divide pc by four 

    

}

//ADDED FUNCTION TO HELP WITH BIT MASKING
unsigned fromBinary(char* bit,int n){
    
    unsigned value = 0;

    for(int i = 0 ; i < n; i++){
        
       if(bit[i] == '1'){
           
           value += (1 << (n-i-1));
       }
    }

    return value;
}
