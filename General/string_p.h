/** @workspace/General/string_p.h
 *  This file includes several functions to process string.
 */

#ifndef STRING_P_H
#define STRING_P_H

#include <stdio.h>
#include <stdlib.h>

#define INIT_LENGTH 10
#define INCREASE_LENGTH 5

char *getstr(void){
    //从标准输入中，利用动态分配字符数组，读取不定长字符串
    int i = 0 , len = INIT_LENGTH;
    char e;
    char *tmp;
    tmp = (char *)malloc((INIT_LENGTH * sizeof(char)));
    e = getchar();
    while(e != '\n'){
        if(i >= len - 1){
            len += INCREASE_LENGTH;
            tmp = (char *)realloc(tmp,len * sizeof(char));
        }
        tmp[i++] = e;
        e = getchar();
    }
    tmp = (char *)realloc(tmp,(i + 1) * sizeof(char));
    tmp[i] = '\0';
    return tmp;
}//getstr

#endif // !STRING_P_H