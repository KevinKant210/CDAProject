#include <stdio.h>

unsigned fromBinary(char* bit, int n);

int main(){

    int array[15];
    array[10] = 100;
    
    printf("%d", 0x8);


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

void testFunction(int* array){

    printf("%d", array[10]);
}