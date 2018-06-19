
#ifndef STYPEDEF_H
#define STYPEDEF_H

#include <string.h>

// - - - - - 关键字类型说明 - - - - -
// typedef float   KeyType;     //实型
typedef int     KeyType;        //整型
// typedef char    *KeyType;    //字符串型

// - - - - - 数据元素类型定义 - - - - -
typedef struct {
    KeyType key;                //关键字域
    // ...                      //其他域
}STElemType;

// - - - - - 对两个关键字的比较宏定义 - - - - -
    // - - 对数值型关键字
    #define  EQ(a,b)  ((a) == (b))
    #define  LT(a,b)  ((a) <  (b))
    #define  LQ(a,b)  ((a) <= (b))

    // - - 对字符串型关键字
    // #define  EQ(a,b)  (!strcmp((a),(b)))
    // #define  LT(a,b)  (strcmp((a),(b)) < 0)
    // #define  LQ(a,b)  (strcmp((a),(b)) <= 0)
    /**
     *  int strcmp((a),(b)):
     *  (a) == (b), return 0;
     *  (a) <  (b), return <0;
     *  (a) >  (b), return >0;
     */

#endif // !STYPEDEF_H
