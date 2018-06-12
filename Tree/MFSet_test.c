
#include <stdio.h>

typedef char SetElemType;
#include "MFSet.h"
#define n   8           //集合中总成员数目
#define rn  7           //二元关系数目

int main(int argc, char const *argv[]){
    //测试用例：偶对(1,2),(3,4),(5,6),(7,8),(1,3),(5,7),(1,5)
    char x[n + 1] = {n,'1','2','3','4','5','6','7','8'};     //0号单元弃用
    MFSet S;
    RSet rs;
    Initial_MFSet(&S,n,x);
    BuildRSet(&rs,rn);
    EquivalenceClass_1(&S,rs);

    Initial_MFSet(&S,n,x);
    EquivalenceClass_2(&S,rs);
    Fix_MFSet(&S,8);        //“压缩路径”

    PrintSet(S);
    return 0;
}//MFSet_test（并查集等价类的确定）
