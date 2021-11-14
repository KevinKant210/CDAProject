#include <stdio.h>

unsigned fromBinary(char* bit, int n);

int main(){

    char* bit;
    bit = "0000";

    unsigned result = fromBinary("1001",4);

    printf("value: %d", result);
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