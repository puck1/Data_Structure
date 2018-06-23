
#include <stdio.h>
#include "SeqSSTable.h"
#include "OrderTable.h"

int main(int argc, char const *argv[]){
    //Opereation;
    SSTable ST;
    printf("Create ordered table ST:\n");
    CreateSSTable_Seq(&ST);

    printf("Test of Binary search:\n");
    printf("Element '%d' found at index %d.\n",21,Search_Bin(ST,21));
    printf("Element '%d' found at index %d.\n",85,Search_Bin(ST,85)); printf("\n");

    printf("Test of Fibonacci search:\n");
    printf("Element '%d' found at index %d.\n",21,Search_Fib(ST,21));
    printf("Element '%d' found at index %d.\n",5,Search_Fib(ST,5)); printf("\n");

    printf("Test of Interpolation search:\n");
    printf("Element '%d' found at index %d.\n",21,Search_Int(ST,21));
    printf("Element '%d' found at index %d.\n",85,Search_Int(ST,85)); printf("\n");

    return 0;
}//OrderTable_test(Binary/Fibonacci/Interpolation)
