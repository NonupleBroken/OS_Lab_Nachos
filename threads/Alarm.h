#pragma once
#include "system.h"
#include "dllist.h"

void check(int which);

class Alarm {
    public:
        Alarm();
        ~Alarm();
        void Pause(int howLong);
        DLList *queue;
        int waiters;
        void CheckIfDue();  
        
        static void new_instance();
        static Alarm *instance;
};
