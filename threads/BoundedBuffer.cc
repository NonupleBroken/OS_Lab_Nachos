#include  "BoundedBuffer.h"

BoundedBuffer::BoundedBuffer(int size) {
    this->maxsize = size;   //缓冲区大小
    lock = new Lock("Buffer lock");
    NotFull = new Condition("Buffer not full cond");
    NotEmpty = new Condition("Buffer not empty cond");
    FrontPlace = 0;
    BackPlace = 0;
    UsedSize = 0;
    buffer = new char[size];  //缓冲区
}

BoundedBuffer::~BoundedBuffer() {
    delete lock;
    delete NotFull;
    delete NotEmpty;
    delete buffer;
}

void BoundedBuffer::Write(void* data, int size) {
    lock->Acquire();
     //缓冲区是否够写
    while (maxsize - UsedSize < size)
        NotFull->Wait(lock); //不够写，等待

     //开始写
    for (int i = 0; i < size; i++)
        *((char*)buffer + (FrontPlace + i) % maxsize) = *((char*)data + i);

    FrontPlace = (FrontPlace + size) % maxsize; //可写位置增加
    UsedSize += size;  //缓冲区使用区间增加
    NotEmpty->Signal(lock);    //非空，可读
    lock->Release();  //释放锁
}

void BoundedBuffer::Read(void* data, int size) {
    lock->Acquire();
     //判断是否够读
    while (UsedSize - size < 0)
        NotEmpty->Wait(lock);  //不够读，等待

    //开始读
    for(int i=0;i<size;i++)
        *((char*)data + i) = *((char*)buffer + (BackPlace + i) % maxsize);

    BackPlace = (BackPlace + size) % maxsize;  //已读位置增加
    UsedSize -= size; //缓冲区可写空间增大
    NotFull->Signal(lock);   //非满，可写
    lock->Release(); //释放锁
}
