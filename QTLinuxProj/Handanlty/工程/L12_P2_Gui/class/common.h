#ifndef COMMON_H
#define COMMON_H

typedef struct _node
{
    //unsigned int len;
    unsigned int addr;  //保存地址
    char _type;         //类型
    //void * content;
    struct _node *next;
}S_Node;




#endif // COMMON_H
