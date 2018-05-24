
#include <stdio.h>
#include <synchapi.h>   //gcc中提供Sleep函数（以毫秒为单位）原型
#include "HuffmanCode.h"

int main(){
    char ch;
    char code[100];     //暂定要译码的编码长度最大为100
    char *msg = NULL;
    int n = 4;          //字符数量
    WeightTable WT;
    HuffmanTree HT;
    HuffmanCode HC;

    InitTable_WT(&WT);
    printf("Create weight table:\n");
    CreateTable_WT(&WT,n);      //建立字符对应权值表
    Sleep(500); printf("\n");

    InitTree_HT(&HT);
    printf("Create Huffman tree...");
    CreateTree_HT(&HT,WT,n);    //建立霍夫曼树
    Sleep(500); printf("Succeeded!\n"); printf("\n");

    printf("Select the method to finding the Huffman Codes of Characters(0/1):\n");
    printf("0:Searching from leaf to root;\n1:Traverse from root without stack;\n");
    printf("Your choice:");
    do{
        ch = getchar();
        if(ch == '0') HuffmanCoding_0(HT,&HC,n);
        else if (ch == '1') HuffmanCoding_1(HT,&HC,n);
        getchar();
    }while (ch != '0' && ch != '1');    //求霍夫曼编码
    Sleep(500); printf("Succeeded!\n"); printf("\n");

    printf("Huffman codes:");
    for (size_t i = 1; i <= n; i++){
        printf("'%c':",HC[i].character);
        printf("%s; ",HC[i].hcode);
    }
    printf("\n");

    printf("Enter a code string to decode:");
    scanf("%s",code);
    DeCoding(HT,code,&msg,n);   //译码
    Sleep(500); printf("Succeeded!\n"); printf("\n");
    Sleep(500); printf("Message:"); puts(msg); printf("\n");
    return 0;
}//HuffmanCode
