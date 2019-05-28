// threadtest.cc 
//	Simple test case for the threads assignment.
//
//	Create two threads, and have them context switch
//	back and forth between themselves by calling Thread::Yield, 
//	to illustratethe inner workings of the thread system.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
// #include "system.h"
#include "Table.h"
#include "BoundedBuffer.h"
// #include "EventBarrier.h"
#include "Alarm.h"
#include "Elevator.h"
#include <time.h>

#define TABLESIZE 100000
#define BOUNDEDBUFFERSIZE 1000

// testnum is set in main.cc
int testnum = 1;
int T = 2;
int N = 2;

int producersnum = 2;
int consumersnum = 2;

DLList *list = NULL;
Table *table = NULL;
BoundedBuffer *boundedbuffer = NULL;
EventBarrier *barrier = NULL;

//----------------------------------------------------------------------
// SimpleThread
//  Loop 5 times, yielding the CPU to another ready thread 
//  each iteration.
//
//  "which" is simply a number identifying the thread, for debugging
//  purposes.
//
// ThreadTest1
//  Set up a ping-pong between two threads, by forking a thread 
//  to call SimpleThread, and then calling SimpleThread ourselves.
//----------------------------------------------------------------------
void SimpleThread(int which) {
    int num;
    
    for (num = 0; num < 5; num++) {
    printf("*** thread %d looped %d times\n", which, num);
        currentThread->Yield();
    }
}

void ThreadTest1() {
    DEBUG('t', "Entering ThreadTest1 ");

    Thread *t = new Thread("forked thread");

    t->Fork(SimpleThread, 1);
    SimpleThread(0);
}

//----------------------------------------------------------------------
// ThreadTest2
//  -m [P] [C]
//  testDllist with producer-consumer model
//----------------------------------------------------------------------
char test2_name_p[50][20];
char test2_name_c[50][20];

void TestDllist_producer(int which) {
    char items[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int key;
    int item_index;
    srand(time(NULL));
    while (true) {
        key = rand() % 1001;
        item_index = rand() % 26;
        printf("%s in:%d %c\n", currentThread->getName(), key, items[item_index]);
        list->SortedInsert(&items[item_index], key);
    }
}

void TestDllist_consumer(int which) {
    int key;
    void *item;
    while (true) {
        item = list->Remove(&key);
        ASSERT(item != NULL);
        printf("%s out:%d %c\n", currentThread->getName(), key, *((char*)item));
    }
}

void ThreadTest2() {
    list = new DLList();
    for (int i = 0; i < producersnum; i++) {
        sprintf(test2_name_p[i], "producer thread %d", i);

        Thread *t = new Thread(test2_name_p[i]);
        t->Fork(TestDllist_producer, i);
    }
    for (int i = 0; i < consumersnum; i++) {
        sprintf(test2_name_c[i], "consumer thread %d", i);

        Thread *t = new Thread(test2_name_c[i]);
        t->Fork(TestDllist_consumer, i);
    }
}


//----------------------------------------------------------------------
// ThreadTest3
//  -a [P] [C]
//  testTable with producer-consumer model
//----------------------------------------------------------------------

char test3_name_p[50][20];
char test3_name_c[50][20];

void TestTable_producer(int which) {
    srand(time(NULL));
    while (true) {
        void *item;
        int key = rand() % 100;       
        int *items = new int[1];
        items[0] = key;
        item = items;       
        printf("%s in:%d %d\n", currentThread->getName(), table->Alloc(item), *(int*)item);
    }
}

void TestTable_consumer(int which) {
    while (true) {
        void *item;
        int index;
        for (int i = 0; i < TABLESIZE; i++) {
            item = table->Get(i);
            if (item != NULL) {
                index = i;
                break;
            }
        }       
        table->Release(index);          //取出并释放第一个非NULL元素
        printf("%s out:%d %d\n", currentThread->getName(), index, *(int*)item);
    }
}

void ThreadTest3() {
    table = new Table(TABLESIZE);
    for (int i = 0; i < producersnum; i++) {
        sprintf(test3_name_p[i], "producer thread %d", i);

        Thread *t = new Thread(test3_name_p[i]);
        t->Fork(TestTable_producer, i);
    }
    for (int i = 0; i < consumersnum; i++) {
        sprintf(test3_name_c[i], "producer thread %d", i);

        Thread *t = new Thread(test3_name_c[i]);
        t->Fork(TestTable_consumer, i);
    }
}

//----------------------------------------------------------------------
// ThreadTest4
//  -b [P] [C]
//  test boundedbuffer with producer-consumer model
//----------------------------------------------------------------------

char test4_name_p[50][20];
char test4_name_c[50][20];

void TestBuffer_producer(int which) {
    srand(time(NULL));
    while (true) {
        int size = rand() % 20 + 1;
        char *items = new char[size+1];
        for (int i = 0; i < size; i++)
            items[i] = 65 + i;

        items[size] = '\0';
        boundedbuffer->Write(items, size);
        printf("%s in:%s\n size:%d usedsize:%d\n", currentThread->getName(), items, size, boundedbuffer->UsedSize);
    }
}

void TestBuffer_consumer(int which) {
    srand(time(NULL));
    while (true) {
        int size = rand() % 10 + 1;
        char *item= new char[size + 1];
        boundedbuffer->Read(item, size);
        item[size] = '\0';
        printf("%s out:%s\n size:%d usedsize:%d\n", currentThread->getName(), (char*)item, size, boundedbuffer->UsedSize);
    }
}

void ThreadTest4() {
    DEBUG('t', "Entering ThreadTest4 ");
    boundedbuffer = new BoundedBuffer(BOUNDEDBUFFERSIZE);
    for (int i = 0; i < producersnum; i++) {
        sprintf(test4_name_p[i], "producer thread %d", i);

        Thread *t = new Thread(test4_name_p[i]);
        t->Fork(TestBuffer_producer, i);
    }
    for (int i = 0; i < consumersnum; i++) {
        sprintf(test4_name_c[i], "consumer thread %d", i);

        Thread *t = new Thread(test4_name_c[i]);
        t->Fork(TestBuffer_consumer, i);
    }
}


//----------------------------------------------------------------------
// ThreadTest5 -e 
//  test EventBarrier
//----------------------------------------------------------------------

char test5_name[10][10];

void TestEventBarrier_runing(int which) {
    srand(time(NULL));
    while (true) {
        if (rand() % 100 == 50) {
            printf("---- %s wait-----\n", currentThread->getName());
            barrier->Wait();
            break;
        }
        currentThread->Yield();
    }
    
    printf("%s complete\n", currentThread->getName());
    printf("waiters:%d\n", barrier->Waiters());
    
    barrier->Complete();
    printf("%s finished\n", currentThread->getName());
    printf("waiters:%d\n", barrier->Waiters());
}

void TestEventBarrier_signal(int which) {
    srand(time(NULL));
    while (true) {
        if (rand() % 200 == 50) {
            printf("----%s signaled-----\n", currentThread->getName());
            barrier->Signal();
            break;
        }
        currentThread->Yield();
    }   
}

void ThreadTest5() {
    Thread *tthread[10];
    barrier = new EventBarrier();
    
    for (int i = 0; i < 10; i++) {
        sprintf(test5_name[i], "thread %d", i);
        tthread[i] = new Thread(test5_name[i]);
        tthread[i]->Fork(TestEventBarrier_runing, i);
    }
    Thread *sig = new Thread("sig");
    sig->Fork(TestEventBarrier_signal, 10);
}


//----------------------------------------------------------------------
// ThreadTest6 -g 
//  test Alarm
//----------------------------------------------------------------------

void TestAlarm1(int time) {
    printf("set %s: %d\n", currentThread->getName(), time);
    Alarm::instance->Pause(time);
    printf("%s wake up at %d ticks\n", currentThread->getName(), stats->totalTicks);
}

void TestAlarm(int time) {
    printf("set %s: %d\n", currentThread->getName(), time);
    Alarm::instance->Pause(time);
    printf("%s wake up at %d ticks\n", currentThread->getName(), stats->totalTicks);
    if (time == 3) {
        Thread *tthread = new Thread("thread 3");
        tthread->Fork(TestAlarm1, 2);
    }
}

void ThreadTest6() {
    Thread *tthread[3];

    tthread[0] = new Thread("thread 0");
    tthread[1] = new Thread("thread 1");
    tthread[2] = new Thread("thread 2");

    tthread[0]->Fork(TestAlarm, 3);
    tthread[1]->Fork(TestAlarm, 2);
    tthread[2]->Fork(TestAlarm, 1);
}

//----------------------------------------------------------------------
// ThreadTest7 -h
// test elevator
//----------------------------------------------------------------------
int num_floors = 10;                     //楼层数
int total_riders = 20;                  //生成的总乘客数
int E_random_come_time = 2;             //随机时间间隔
int elevator_num = 2;                     //电梯个数

long long int E_sumtime = 0;
int E_End_num = 0;
int E_should_start_time = 0;        //当前进程应当开始的时间
int max_turnaroundtime = 0;
int min_turnaroundtime = 2147483645;

char test7_elevator_name[10][15];
char test7_rider_name[100][15];

void TestElevatorRider(int which) {
    srand(time(NULL));
    int srcfloor;
    int dstfloor;
    do {
        srcfloor = rand() % num_floors;
        dstfloor = rand() % num_floors;
    } while (srcfloor == dstfloor);
    int rand_time = rand() % E_random_come_time;   //随机时间间隔
    E_should_start_time += rand_time;
    Alarm::instance->Pause(E_should_start_time);
    int start_time = stats->totalTicks; 
    printf("\n+++%s start at %d ticks from %d floor to %d floor\n", currentThread->getName(), start_time, srcfloor, dstfloor);
    rider(which, srcfloor, dstfloor);
    printf("------%s costs %d------\n", currentThread->getName(), stats->totalTicks - start_time);
    int turnaroundtime = stats->totalTicks - start_time;
    if (turnaroundtime > max_turnaroundtime)
        max_turnaroundtime = turnaroundtime;
    if (turnaroundtime < min_turnaroundtime)
        min_turnaroundtime = turnaroundtime;
    E_sumtime += turnaroundtime;
    E_End_num++;
    //printf("------%d has finished------\n", E_End_num);
    if (E_End_num == total_riders) {
        printf("\n-------avg turnaround time %.2f-------\n", (float)E_sumtime / total_riders);
        printf("-------max turnaround time %d-------\n", max_turnaroundtime);
        printf("-------min turnaround time %d-------\n\n", min_turnaroundtime);
    }
}

void TestElevatorControl(int which) {
#ifdef MULTIPLE_ELEVATOR
    Building::instance->getElevator(which)->ElevatorControl();
#endif

#ifdef SINGLE_ELEVATOR
    Building::instance->getElevator()->ElevatorControl();
#endif
}

void ThreadTest7() {
    Building::new_instance("building", num_floors, elevator_num);

#ifdef MULTIPLE_ELEVATOR
    for (int i = 0; i < elevator_num; i++) {
        sprintf(test7_elevator_name[i], "elevator %d", i);

        Thread *t = new Thread(test7_elevator_name[i]);
        t->Fork(TestElevatorControl, i);
    }
#endif

#ifdef SINGLE_ELEVATOR
    Thread *t = new Thread("elevator");
    t->Fork(TestElevatorControl, 0);
#endif

    for (int i = 0; i < total_riders; i++) {
        sprintf(test7_rider_name[i], "rider %d", i);

        Thread *t = new Thread(test7_rider_name[i]);
        t->Fork(TestElevatorRider, i);
    }
}


//----------------------------------------------------------------------
// ThreadTest
//  Invoke a test routine.
//----------------------------------------------------------------------

void ThreadTest() {
    switch (testnum) {
    case 1:
        ThreadTest1();
        break;
    case 2:
        ThreadTest2();
        break;
    case 3:
        ThreadTest3();
        break;
    case 4:
        ThreadTest4();
        break;
    case 5:
        ThreadTest5();
        break;
    case 6:
        ThreadTest6();
        break;
    case 7:
        ThreadTest7();
        break;
    default:
        printf("No test specified.\n");
        break;
    }
}
