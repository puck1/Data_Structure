
#include <stdio.h>
#include "OLGraph.h"
#include "SCC.h"


int main(int argc, char const *argv[]){
    OLGraph G;
    CreateDG(&G);
    SCC_Kosaraju(G);
    return 0;
}//Strongly_Connected_Components
