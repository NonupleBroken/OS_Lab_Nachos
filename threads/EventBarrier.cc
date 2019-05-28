#include "EventBarrier.h"

EventBarrier::EventBarrier() {
    waiternum = 0;
    lock = new Lock("EventBarrier Lock");
    event = new Condition("Event come cond");
    comp = new Condition("Event complete cond");
    signaled = false;
}

EventBarrier::~EventBarrier() {
    delete lock;
    delete event;
    delete comp;
}

// 由控制栅栏的线程调用，唤醒当前正在等待的所有线程，
// 自己陷入等待直到线程已被全部唤醒，苏醒后重置栅栏状态。
void EventBarrier::Signal() {
    lock->Acquire();
    signaled = true; // 放开栅栏
    event->Broadcast(lock); // 广播，唤醒所有阻塞于Signal的线程
    comp->Wait(lock); // 自己阻塞于Complete
    signaled = false; // 苏醒后重置栅栏
    lock->Release();
}

// 用于线程等待同步：由需要同步的线程调用，调用者陷入等待直到被唤醒。
// 若栅栏已处于放开状态，则直接返回。
void EventBarrier::Wait() {
    lock->Acquire();
    waiternum++;
    // 若栅栏放开，则直接返回
    if (signaled) {
        lock->Release();
        return;
    }
    // 否则，阻塞于Signal
    else {
        event->Wait(lock);  
        lock->Release();
    }
}

// 用于线程唤醒同步：由需要同步的线程调用，
// 从Wait返回（被唤醒）后应接着调用Complete，随后陷入另一种等待直到所有线程都被唤醒。
void EventBarrier::Complete() {
    lock->Acquire();
    waiternum--;
    // 如果是最后一个调用Complete者，则唤醒所有阻塞于Complete的线程
    if (waiternum == 0)
        comp->Broadcast(lock);
    // 否则，阻塞于Complete
    else
        comp->Wait(lock);
    lock->Release();
}

// 返回当前处于等待（或尚未被唤醒）的线程数量，用来标记线程是否已被全部唤醒。
int EventBarrier::Waiters() {
    return waiternum;
}
