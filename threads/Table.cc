#include "Table.h"

Table::Table(int size) {
    TableSize = size;
    lock = new Lock("Table lock");
    base = new void*[size];
    for (int i = 0; i < size; i++)
        base[i] = NULL;
}

Table::~Table() {
    delete base;
    delete lock;
}

int Table::Alloc(void* object) {
    int re;  //插入位置
    lock->Acquire();
    int NowIndex = 0;   //表的大小

    while (base[NowIndex] != NULL && NowIndex < TableSize)
        NowIndex++;  //遍历表

    if (NowIndex >= TableSize)  
        re = -1;  //表满了，等待
    else {
        base[NowIndex] = object;  //找到空位，插入
        re = NowIndex;   //返回插入值
    }
    ASSERT(re != -1);
    lock->Release();
    return re;  //返回插入位置
}

void* Table::Get(int index) {
    lock->Acquire();
    void* item;       //接收表信息
    ASSERT(index < TableSize);  //判断请求是否越界
    if (index >= TableSize)
        item = NULL;            //请求越界，不存在
    else
        item = base[index];   //请求存在，获取表信息
    lock->Release();
    return item;      //返回表信息

}

void Table::Release(int index) {
	lock->Acquire();
	ASSERT(index<TableSize);
	base[index] = NULL;
	lock->Release();
}
