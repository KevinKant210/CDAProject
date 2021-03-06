#include "spimcore.h"


unsigned fromBinary(char* bit,int n);

unsigned findFunct(unsigned funct);

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
        if((int) A < (int) B){
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
        *ALUresult = (A & B); 
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
    
    }

    if(*ALUresult == 0){
        *Zero = 1;
    }
}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
    
    //make sure the address is word aligned ( aka a mulitple of 4)
    //need to divide pc by four to get the index for mem
    if(PC > 0xFFFF || PC < 0x0000){
        return 1;
    }
    if(PC % 4 != 0){
        return 1;
    }
    
    *instruction = Mem[PC>>2];
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

   unsigned maskOpcode = fromBinary("11111100000000000000000000000000",32);
   
   
   //remeber to shift the bits over!
   unsigned opCode = (instruction&maskOpcode) >> (26);
   
   if(opCode == 0){
       //r-format instruciton
        //bit mask for rs 00000011111000000000000000000000
        unsigned maskRS = fromBinary("00000011111000000000000000000000", 32);
        unsigned maskRT = fromBinary("00000000000111110000000000000000",32);
        unsigned maskRD = fromBinary("00000000000000001111100000000000",32);
        //no shamt pointer so assume this is unneccesary
        //unsigned maskShamt = fromBinary("00000000000000000000011111000000",32);
        unsigned maskFunct = fromBinary("00000000000000000000000000111111",32);
        
        *r1 = (maskRS & instruction) >> (21);
        *r2 = (maskRT & instruction) >> (16);
        *r3 = (maskRD & instruction) >> (11);
        //at the end so no need to shift
        *funct = (maskFunct & instruction);
        //since R-type instruction the op code must become the function code
        *op = opCode;
   }else if(opCode > 3){
       // I format instruction
        unsigned maskRS = fromBinary("00000011111000000000000000000000", 32);
        unsigned maskRT = fromBinary("00000000000111110000000000000000",32);
        unsigned maskAddress = fromBinary("00000000000000001111111111111111",32);
        *r1 = (maskRS & instruction) >> (21);
        *r2 = (maskRT & instruction) >> (16);
        //at the end so no need to shift
        *offset = (maskAddress & instruction);
        *op = opCode;
        
   }else{
       //j format instruction
       unsigned maskAddressLong = fromBinary("00000011111111111111111111111111",32);
        //at the end so no need to shift
        *jsec = (maskAddressLong & instruction);
        
        *op = opCode;
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
means that disabled, 2 means ???don???t care???. 
For RegDst, Jump, Branch, MemtoReg or ALUSrc, the value 0 or 1 indicates the 
selected path of the multiplexer; 2 means ???don???t care???. 
The following table shows the meaning of the values of ALUOp. 
 
3.  Return 1 if a halt condition occurs; otherwise, return 0. 
    */
   
   switch ((int)op)
   {
   case 0:
        //r-type so we make alu op 7
       
       controls->RegDst = 1;
       controls->Jump = 0;
       controls->Branch = 0;
       controls->MemRead = 0;
       controls->MemtoReg = 0;
       controls->ALUOp = 7;
       controls->MemWrite = 0;
       controls->ALUSrc = 0;
       controls->RegWrite = 1;
    
       break;
   case 8:
        //add immediate is occuring
        controls->RegDst = 0;
        controls->Jump = 0;
        controls->Branch = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls-> ALUOp = 0;
        controls->MemWrite = 0;
        controls->ALUSrc = 1;
        controls->RegWrite = 1;
        break;
    case 35:
        //a load word is occuring 100011
        controls->RegDst = 0;
        controls->Jump = 0;
        controls->Branch = 0;
        controls->MemRead = 1;
        controls->MemWrite = 0;
        controls->MemtoReg = 1;
        controls->ALUOp = 0;
        controls->ALUSrc = 1;
        controls->RegWrite = 1;
        break;
    case 43:
        //store word is occuring 101011
        controls->RegDst = 0;
        controls->Jump = 0;
        controls->Branch = 0;
        controls->MemRead = 0;
        controls->MemWrite = 1;
        controls->MemtoReg = 0;
        controls->ALUOp = 0;
        controls->ALUSrc = 1;
        controls->RegWrite = 0;
        break;
    case 15:
        //load upper immediate 001111
        controls->RegDst = 0;
        controls->Jump = 0;
        controls->Branch = 0;
        controls->MemRead = 0;
        controls->MemWrite = 0;
        controls->MemtoReg = 0;
        controls->ALUOp = 6;
        controls->ALUSrc = 1;
        controls->RegWrite = 1;
        break;
    case 4:
        //branch on equal 000100
        controls->RegDst = 0;
        controls->Jump = 0;
        controls->Branch = 1;
        controls->MemRead = 0;
        controls->MemWrite = 0;
        controls->MemtoReg = 0;
        controls->ALUOp = 1;
        controls->ALUSrc = 0;
        controls->RegWrite = 0;
        break;
    case 10:
        //set less than immediate 001010
        controls->RegDst = 0;
        controls->Jump = 0;
        controls->Branch = 0;
        controls->MemRead = 0;
        controls->MemWrite = 0;
        controls->MemtoReg = 0;
        controls->ALUOp = 2;
        controls->ALUSrc = 1;
        controls->RegWrite = 1;
        break;
    case 11:
        //set less than immediate unsigned 001011
        controls->RegDst = 0;
        controls->Jump = 0;
        controls->Branch = 0;
        controls->MemRead = 0;
        controls->MemWrite = 0;
        controls->MemtoReg = 0;
        controls->ALUOp = 3;
        controls->ALUSrc =1;
        controls->RegWrite = 1;
        break;
    case 2:
        //a jump is occuring 000010
        controls->RegDst = 2;
        controls->Jump = 1;
        controls->Branch = 0;
        controls->MemRead = 0;
        controls->MemWrite = 0;
        controls->MemtoReg = 0;
        controls->ALUOp = 0;
        controls->ALUSrc = 0;
        controls->RegWrite = 0;

       break;
    
    default:
        return 1;
       
   }

   return 0;
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
    
    // Shift the offset over 15 bits to find sign bit
    unsigned sign = offset >> 15;
    
    
    // If the sign bit is 1 (value is negative)
    // Then add 16 1 bits to the offset and return
    //i think you need to put the deop character infront of extended value -> *
    if(sign == 1){
        
        *extended_value = 0xFFFF0000 + offset;
        return;
    }
    
    // The extended value will just equal the offset otherwise 
    // as the first 16 bits are already 0's
    *extended_value = offset;

    
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
    

    unsigned UsedCon;
    unsigned UsedData;
    
    // If ALUSrc is asserted
    // Then do opperations using extended_value and based on function
    if(ALUSrc == 1){
        UsedData = extended_value;
    }
    
    // If ALUSrc is deasserted
    // Then use data2 and do operations based on ALUOp
    if(ALUSrc == 0){
        UsedData = data2;
    }
    
    // If this is a R type
    // Then get the control from the function
    if(ALUOp == 7){
        UsedCon = findFunct(funct);
    }else{
        // Else, use ALUOp as the Control
        UsedCon = ALUOp;
    }
    
    // If it will try to do something that is not in the ALU
    // Then it will halt
    if(UsedCon > 7){
        return 1;
    }
    
    // Do ALU using the parameters which will fit the type
    ALU(data1, UsedData, UsedCon, ALUresult, Zero);

    return 0;
    
    
    
}
/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{   
    //ENSURE ALU RESULT MEMORY IS WITHIN THE BOUNDS AND A MULTIPLE OF 4 FOR WORDX ALIGNMENT
        /*
            1.  Use the value of MemWrite or MemRead to determine if a memory write 
    operation or memory read operation or neither is occurring. 
    2.  If reading from memory, read the content of the memory location addressed by 
    ALUresult to memdata. 
    3.  If writing to memory, write the value of data2 to the memory location 
    addressed by ALUresult. 
    4.  Return 1 if a halt condition occurs; otherwise, return 0.
        */

      

    

    if(MemRead == 1){
        if(ALUresult % 4 != 0 || ALUresult > 0xFFFF || ALUresult < 0x0000){
            return 1;
        }
        ALUresult = (ALUresult >> 2);
        *memdata = Mem[ALUresult];
        
    }
    
    if(MemWrite == 1){
        if(ALUresult % 4 != 0 || ALUresult > 0xFFFF || ALUresult < 0x0000){
            return 1;
        }
        ALUresult = ALUresult >> 2;
        Mem[ALUresult] = data2;

        

    }
    
    
    return 0;
}
/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
    /*
        1.  Write the data (ALUresult or memdata) to a register (Reg) addressed by r2 or r3. 
    */
   
   
   if(RegWrite == 0){
       return;
   }
    
   unsigned toWrite;
   if(MemtoReg == 1){
       toWrite = memdata;
   }else{
       toWrite = ALUresult;
   }




   if(RegDst == 0){
       //write to r2
       Reg[r2] = toWrite;
   }else{
       //write to r3
       Reg[r3] = toWrite;
   }


}
/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{
    //1.  Update the program counter (PC). 
    //to access proper memory index need to divide pc by four 

    
    if(Jump == 1){

        *PC = (jsec << 2);
        
    }else if(Branch == 1 && Zero == 1){
        

        *PC = (extended_value * 4) + *PC + 4;

        
        
    }else{

        *PC += 4;
    }


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

// ADDED FUNCTION TO FIND WHAT FUNC WILL DO
unsigned findFunct(unsigned funct){
    switch(funct){
        
        // Function says to s.l.l - 000000
        // return 6 (s.l.l) for the ALU
        case 0:
            return 6;
        
        // Function says to add - 100000
        // return 0 (add) for the ALU
        case 32:
            return 0;
        
        // Function says to sub - 100010
        // return 1 (sub) for the ALU
        case 34:
            return 1;
        
        // Function says to and - 100100
        // return 4 (and) for the ALU
        case 36:
            return 4;
        
        // Function says to or - 100101
        // return 5 (or) for the ALU
        case 37:
            return 5;
        
        // Function says to not - 100111
        // return 7 (not) for the ALU
        case 39:
            return 7;
        
        // Function says to s.l.t - 101010
        // return 2 (s.l.t) for the ALU
        case 42:
            return 2;
        
        // Function says to s.l.t.u - 101011
        // return 3 (s.l.t.u) for the ALU
        case 43:
            return 3;
        
        
        // Return 9 so nothing will be done in ALU
        default:
            return 9;
    }
}
