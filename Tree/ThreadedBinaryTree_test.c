
#include <stdio.h>
#include <synchapi.h>   //gcc中提供Sleep函数（以毫秒为单位）原型
#include "status.h"
#include "ThreadedBinaryTree.h"

Status PrintElement(TElemType e){
    printf("%c ",e);
    return OK;
}//PrintElement

int main(){
    //Operation;
    //测试用例：表达式a+b*(c-d)-e/f，先序建立序列为:-+a  *b  -c  d  /e  f  EOF
    BiThrTree Thrt;
    InitBiThrTree(&Thrt);
    printf("Create BiThrTree Thrt in preface order,character SPACE (' ') means node is empty:\n");
    CreateBiThrTree(&Thrt);

    printf("PreorderThreading...");         //先序线索化及先序遍历
    PreOrderThreading(Thrt);
    Sleep(500); printf("Succeeded!\n");
    printf("PreOrderTraverse_Thr:");
    PreOrderTraverse_Thr(Thrt,PrintElement);
    printf("\n");

    DeThreading(Thrt);      //去线索化

    printf("InorderThreading...");          //中序线索化及中序遍历
    InOrderThreading(Thrt);
    Sleep(500); printf("Succeeded!\n");
    printf("InOrderTraverse_Thr:");
    InOrderTraverse_Thr(Thrt,PrintElement);
    printf("\n");

    #if NeedPostOrder
    DeThreading(Thrt);

    printf("PostOrderThreading...");        //后序线索化及后序遍历（需要线索三叉链表）
    PostOrderThreading(Thrt);
    Sleep(500); printf("Succeeded!\n");
    printf("PostOrderTraverse_Thr:");
    PostOrderTraverse_Thr(Thrt,PrintElement);
    printf("\n");
    #endif // NeedPostOrder

    ClearBiThrTree(Thrt); (Thrt->lchild==Thrt->rchild) && (Thrt->rchild == Thrt )?
    printf("Thrt is cleared!  "):printf("Thrt isnot cleared!  ");
    DestroyBiThrTree(&Thrt); Thrt?printf("Thrt isnot destroyed!\n"):printf("Thrt is destroyed!\n");

    return 0;
}//线索二叉树的建立/前中后序线索化/前中后序遍历/清空/销毁
