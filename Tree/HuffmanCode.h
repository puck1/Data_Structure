
#ifndef HUFFMANCODE_H
#define HUFFMANCODE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "status.h"

// - - - - - 霍夫曼树和霍夫曼编码的存储表示 - - - - -
typedef struct {
    char character;
    unsigned int weight;
    unsigned int parent,lchild,rchild;
}HTNode, *HuffmanTree;      //动态分配数组存储霍夫曼树
typedef struct{
    char character;
    char* hcode;
}HCItem,*HuffmanCode;       //动态分配数组存储霍夫曼编码表
typedef struct {
    char c;
    unsigned int w;
}WTItem, *WeightTable;       //动态分配数组存储包含character与其weight的权值表

// - - - - - 调用的函数原型说明 - - - - -
Status Select(HuffmanTree HT,int i,int *s1,int *s2);

// - - - - - 基本函数的算法描述 - - - - -
Status InitTable_WT(WeightTable *WT){
    //初始化权值表WT，0号单元弃用
    *WT = NULL;
    return OK;
}//InitTable_WT

Status CreateTable_WT(WeightTable *WT,int n){
    //建立含有n个字符的权值表WT，0号单元弃用
    int i;
    *WT = (WeightTable)malloc((n + 1) * sizeof(WTItem));
    if(!(*WT)) exit(OVERFLOW);
    printf("Enter all characters(Separated by spaces):");
    for (i = 1; i <= n; ++i){
        (*WT)[i].c = getchar();
        getchar();
    }
    printf("Enter weights of the corresponding characters(Separated by spaces):");
    for (i = 1; i <= n; ++i){
        scanf("%d%*c",&(*WT)[i].w);
    }
    printf("Succeeded!\n");
    return OK;
}//CreateTable_WT

Status InitTree_HT(HuffmanTree *HT){
    //初始化霍夫曼树HT
    *HT = NULL;
    return OK;
} //InitTree_HT

Status CreateTree_HT(HuffmanTree *HT,WeightTable WT,int n){
    //w存放n个字符的权值（均>0），构造霍夫曼树HT
    if(n <= 1) return ERROR;
    int m = 2 * n - 1;
    *HT = (HuffmanTree)malloc((m + 1) * sizeof(HTNode)); //0号单元未用
    if(!(*HT)) exit(OVERFLOW);
    int i,s1,s2;
    for (i = 1; i <= n; ++i){   //建霍夫曼树
        (*HT)[i].weight = WT[i].w;
        (*HT)[i].character = WT[i].c;
        (*HT)[i].parent = (*HT)[i].lchild = (*HT)[i].rchild = 0;
    }
    for(;i <= m; ++i){
        (*HT)[i].weight = 0;
        (*HT)[i].character = '\0';
        (*HT)[i].parent = (*HT)[i].lchild = (*HT)[i].rchild = 0;
    }
    for(i = n + 1;i <= m; ++i){
        if(!Select(*HT,i-1,&s1,&s2)) return ERROR;      //在HT[1..i-1]中选择两个weight最小且无双亲的结点
        (*HT)[s1].parent = (*HT)[s2].parent = i;
        (*HT)[i].lchild = s1; (*HT)[i].rchild = s2;
        (*HT)[i].weight = (*HT)[s1].weight + (*HT)[s2].weight;
    }
    return OK;
} //CreateTree_HT

Status Select(HuffmanTree HT,int i,int *s1,int *s2){
    //在HT[1..i]中选择parent为0且weight最小的两个结点，其序号分别赋给s1和s2。
    if(i < 2) return ERROR;
    int j;
    int m = 0,n = 0,tmp = 0;
    for (j = 1; j <= i; ++j)        //先找到parent为0的两个结点m，n
        if(HT[j].parent == 0){
            if(m == 0) m = j;
            else if(n == 0) {n = j; break;}
        }
    if(m ==0 || n == 0) return ERROR;
    if(HT[m].weight > HT[n].weight)
        {tmp = m; m = n; n = tmp;}  //使 m的weight < n的weight
    j = (m>n?m:n) + 1;
    for (;j <= i; ++j)              //继续寻找weight更小的结点
        if(HT[j].parent == 0){
            if(HT[j].weight < HT[m].weight)
                {n = m ; m = j;}
            else if(HT[j].weight < HT[n].weight)
                n = j;
        }
    *s1 = m; *s2 = n;
    return OK;
} //Select

Status HuffmanCoding_0(HuffmanTree HT,HuffmanCode *HC,int n){
    //由霍夫曼树HT求出n个字符的霍夫曼编码HC
    //*从叶子到根逆向求每个字符的霍夫曼编码
    char *cd;
    int i;
    int start,c,p;
    *HC = (HuffmanCode)malloc((n + 1)*sizeof(HCItem));  //分配n个字符编码的头指针向量（0号单元弃用）
    if(!(*HC)) exit(OVERFLOW);
    cd = (char *)malloc(n * sizeof(char));          //分配求编码的工作空间（编码最大长度为n-1）
    if(!cd) exit(OVERFLOW);
    cd[n-1] = '\0';                                 //编码结束符
    for (i = 1; i <= n; ++i){                       //逐个字符求霍夫曼编码
        start = n - 1;                              //编码结束符位置
        for (c = i, p = HT[i].parent; p != 0; c = p, p = HT[p].parent){ //从叶子到根逆向求编码
            if(c == HT[p].lchild) cd[--start] = '0';
            else cd[--start] = '1';
        }
        (*HC)[i].character = HT[i].character;
        (*HC)[i].hcode = (char *)malloc((n - start) * sizeof(char));    //为第i个字符编码分配空间
        strcpy((*HC)[i].hcode,&cd[start]);          //从cd赋值编码（串）到HC
        /**
         * char * strcpy ( char * destination, const char * source );
         * Copies the C string pointed by source into the array pointed by destination,
         * including the terminating null character (and stopping at that point).
         * To avoid overflows, the size of the array pointed by destination shall be long enough
         * to contain the same C string as source (including the terminating null character),
         * and should not overlap in memory with source.
         */
    }
    free(cd);       //释放工作空间
    return OK;
} //HuffmanCoding

Status HuffmanCoding_1(HuffmanTree HT,HuffmanCode *HC,int n){
    //由霍夫曼树HT求出n个字符的霍夫曼编码HC
    //*无栈非递归遍历霍夫曼树，求霍夫曼编码
    char *cd;
    int m = 2 * n - 1;
    int p = m , cdlen = 0;
    int mark[m + 1];
    *HC = (HuffmanCode)malloc((n + 1) * sizeof(HCItem));
    if(!(*HC)) exit(OVERFLOW);
    cd = (char *)malloc(n * sizeof(char));
    if(!(cd)) exit(OVERFLOW);
    for (int i = 1; i <= m; ++i) mark[i] = 0;  //霍夫曼树结点的状态标志,0号单元弃用
    while (p){
        if(mark[p] == 0){                       //向左
            mark[p] = 1;
            if(HT[p].lchild != 0){p = HT[p].lchild; cd[cdlen++] = '0';}
            else if (HT[p].rchild == 0){        //登记叶子结点的字符编码
                (*HC)[p].character = HT[p].character;
                (*HC)[p].hcode = (char *)malloc((cdlen + 1) * sizeof(char));
                if(!(*HC)[p].hcode) exit(OVERFLOW);
                cd[cdlen] = '\0'; strcpy((*HC)[p].hcode,cd);    //复制编码（串）
                p = HT[p].parent; --cdlen;      //退到父结点，编码长度减1
            }
        }//if
        else if (mark[p] == 1){                 //向右或回退
            if(mark[HT[p].rchild] == 1 || HT[p].rchild == 0){   //右孩子已经访问或者不存在
                p = HT[p].parent; cdlen--;      //退到父结点，编码长度减1
            }
            else{
                p = HT[p].rchild;
                cd[cdlen++] = '1';
            }
        }//else if
    }//while
    free(cd);
    return OK;
} //HuffmanCoding_1

Status DeCoding(HuffmanTree HT,char *code,char **msg,int n){
    //分解code中字符串，从含有n个字符的霍夫曼树HT根出发，进行译码
    int m;
    int i = 0 , j = 0;
    int msgsize = 100;
    *msg = (char *)malloc(msgsize * sizeof(char)); //动态分配字符串存储译码
    if(!(*msg)) exit(OVERFLOW);
    while(code[i] != '\0'){
        m = 2 * n - 1;      //从根节点开始寻找
        while (HT[m].character == '\0' && code[i] != '\0'){
            if(code[i] == '0') m = HT[m].lchild;
            else if(code[i] == '1') m = HT[m].rchild;
            ++i;
        }
        if(j >= msgsize - 1){
            *msg = (char *)realloc(*msg,(msgsize + 10) * sizeof(char));
            msgsize += 10;
        }
        (*msg)[j++] = HT[m].character;
    }
    (*msg)[j] = '\0';
    return OK;
}//DeCoding

#endif // !HUFFMANCODE_H
