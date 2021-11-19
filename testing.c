#include <stdio.h>

unsigned fromBinary(char* bit, int n);

int main(){

     
     unsigned bit = 0b1100;
    
     unsigned negative = (~bit);
     
     printf("%d %d",bit, negative);

    return 0;
}

unsigned fromBinary(char* bit,int n){
    
    unsigned value = 0;

    for(int i = 0 ; i < n; i++){
        
       if(bit[i] == '1'){
           
           value += (1 << (n-i-1));
       }
    }

    return value;
}

void testFunction(int* value){

    *value = 20;
    return;
}