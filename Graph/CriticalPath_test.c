
#include <stdio.h>
#include "CriticalPath.h"

int main(int argc, char const *argv[]){
    //Operation;
    ALGraph(G);
    CreateDG(&G);   //此处应在*info中录入弧的权值
    printf("Activity of DN G('*' means it's a critical activity):\n");
    CriticalPath(G);
    printf("Test of the algorithm of getting critical activities by DFS:\n");
    CriticalPath_DFS(G);
    return 0;
}//CriticlaPath_test
