#include "Elevator.h"
#include "Alarm.h"

Building *Building::instance;


//------------------- SINGLE_ELEVATOR Start -------------------//

#ifdef SINGLE_ELEVATOR

Elevator * Building::getElevator() {
    return elevator;
}

void Building::new_instance(char *debugname, int numFloors, int numElevators) {
    instance = new Building(debugname,numFloors,numElevators);
}

Building::Building(char* debugname, int numfloors, int numelevators) {
    name = debugname;
    this->numFloors = numfloors;
    this->numElevators = numelevators;
    elevator =new Elevator("elevator", numfloors, 0);
    BuildingLock = new Lock("Building lock");
    ElevatorUpBarrier = new EventBarrier*[numfloors];
    ElevatorDownBarrier = new EventBarrier*[numfloors];
    for (int i = 0; i < numfloors; i++) {
        ElevatorUpBarrier[i] = new EventBarrier;
        ElevatorDownBarrier[i] = new EventBarrier;
    }
}

Building::~Building() {
    delete elevator;
    delete BuildingLock;
    for (int i = 0; i < numFloors; i++) {
        delete ElevatorUpBarrier[i];
        delete ElevatorDownBarrier[i];
    }
    delete[] ElevatorUpBarrier;
    delete[] ElevatorDownBarrier;
}

Elevator* Building::AwaitDown(int fromFloor) {
    ElevatorDownBarrier[fromFloor]->Wait();
    return elevator;
}

Elevator* Building::AwaitUp(int fromFloor) {
    ElevatorUpBarrier[fromFloor]->Wait();
    return elevator;
}

void Building::CallDown(int fromFloor) {
    elevator->ElevatorLock->Acquire();
    elevator->HaveRequest->Signal(elevator->ElevatorLock);
    elevator->ElevatorLock->Release();
}

void Building::CallUp(int fromFloor) {
    elevator->ElevatorLock->Acquire();
    elevator->HaveRequest->Signal(elevator->ElevatorLock);
    elevator->ElevatorLock->Release();
}

#endif

//------------------- SINGLE_ELEVATOR Finish -------------------//


//------------------- MULTIPLE_ELEVATOR Start -------------------//

#ifdef MULTIPLE_ELEVATOR

Elevator * Building::getElevator(int id) {
    return elevator[id];
}

void Building::new_instance(char *debugname, int numFloors, int numElevators) {
    instance = new Building(debugname, numFloors, numElevators);
}

Building::Building(char* debugname, int numfloors, int numelevators) {
    name = debugname;
    this->numFloors = numfloors;
    this->numElevators = numelevators;
    ElevatorUpBarrier = new EventBarrier*[numfloors];
    ElevatorDownBarrier = new EventBarrier*[numfloors];

    elevator = new Elevator*[numelevators];

    for (int i = 0; i < numelevators; i++)
        elevator[i] = new Elevator("elevator", numfloors, i);

    for (int i = 0; i < numfloors; i++) {
        ElevatorUpBarrier[i] = new EventBarrier;
        ElevatorDownBarrier[i] = new EventBarrier;
    }

    BuildingLock = new Lock("Building lock");
}

Building::~Building() {
    for (int i = 0; i < numElevators; i++)
        delete elevator[i];
    
    for (int i = 0; i < numFloors; i++) {
        delete ElevatorUpBarrier[i];
        delete ElevatorDownBarrier[i];
    }

    delete[] elevator;
    delete[] ElevatorUpBarrier;
    delete[] ElevatorDownBarrier;
    delete BuildingLock;
}

Elevator* Building::AwaitDown(int fromFloor) {
    ElevatorDownBarrier[fromFloor]->Wait();
    BuildingLock->Acquire();
    Elevator *re;
    for (int i = 0; i < numElevators; i++) {
        if (elevator[i]->currentfloor == fromFloor 
            && (elevator[i]->state == DOWN || elevator[i]->state == STOP)
            && elevator[i]->canEnter) {
            re = elevator[i];
            break;
        }
    }
    BuildingLock->Release();
    return re;
}

Elevator* Building::AwaitUp(int fromFloor) {
    ElevatorUpBarrier[fromFloor]->Wait();
    BuildingLock->Acquire();
    Elevator *re;
    for (int i = 0; i < numElevators; i++) {
        if (elevator[i]->currentfloor == fromFloor 
            && (elevator[i]->state==UP || elevator[i]->state == STOP)
            && elevator[i]->canEnter) {
            re = elevator[i];
            break;
        }
    }
    BuildingLock->Release();
    return re;
}

void Building::CallDown(int fromFloor) {
    BuildingLock->Acquire();
    int MinInterval = numFloors + 1;
    int eleNum = -1;
    for (int i = 0; i < numElevators; i++) {
        if (elevator[i]->state == STOP 
            && abs(elevator[i]->currentfloor-fromFloor) < MinInterval) {
            MinInterval = abs(elevator[i]->currentfloor - fromFloor);
            eleNum = i;
        }
    }
    if (eleNum != -1)
        elevator[eleNum]->HaveRequest->Signal(BuildingLock);

    BuildingLock->Release();
}

void Building::CallUp(int fromFloor) {
    BuildingLock->Acquire();
    int MinInterval = numFloors + 1;
    int eleNum = -1;
    for (int i = 0; i < numElevators; i++) {
        if (elevator[i]->state == STOP 
            && abs(elevator[i]->currentfloor - fromFloor) < MinInterval) {
            MinInterval = abs(elevator[i]->currentfloor - fromFloor);
            eleNum = i;
        }
    }

    if (eleNum != -1)
        elevator[eleNum]->HaveRequest->Signal(BuildingLock);

    BuildingLock->Release();
}

#endif // MULTIPLE_ELEVATOR

//------------------- MULTIPLE_ELEVATOR Finish -------------------//


Elevator::Elevator(char* debugName, int numfloors, int myID) {
    name = debugName;
    this->numFloors = numfloors;
    ElevatorID = myID;
    ElevatorOutBarrier = new EventBarrier*[numfloors];
    for (int i = 0; i < numfloors; i++)
        ElevatorOutBarrier[i] = new EventBarrier;
    
    ElevatorLock = new Lock("Elevator lock");
    state = STOP;
    currentfloor = 0;

    HaveRequest = new Condition("have a elevator requset");
    occupancy=0;

#ifdef MULTIPLE_ELEVATOR
    canEnter=false;
#endif

#ifdef BOUNDED_ELEVATOR
    ElevatorNotFull = new Condition("elevator is not full");
#endif
}

Elevator::~Elevator() {
    for (int i = 0; i < numFloors; ++i)
        delete ElevatorOutBarrier[i];

    delete[] ElevatorOutBarrier;
    delete[] ElevatorLock;
    delete HaveRequest;

#ifdef BOUNDED_ELEVATOR
    delete ElevatorNotFull;
#endif
}

void Elevator::OpenDoors() {
    Alarm::instance->Pause(OPEN_AND_CLOSE_DOOR);
    printf("\n*******now elevator %d open door at %d floor with %d riders*******\n", ElevatorID, currentfloor, occupancy);
    if(ElevatorOutBarrier[currentfloor]->Waiters() != 0)
        ElevatorOutBarrier[currentfloor]->Signal();

#ifdef MULTIPLE_ELEVATOR
    canEnter=true;
#endif

    if (state == UP) {
        if (Building::instance->ElevatorUpBarrier[currentfloor]->Waiters() != 0)
            Building::instance->ElevatorUpBarrier[currentfloor]->Signal();    
    }
    else if (state == DOWN) {
        if (Building::instance->ElevatorDownBarrier[currentfloor]->Waiters() != 0)
            Building::instance->ElevatorDownBarrier[currentfloor]->Signal();
    }
}

void Elevator::CloseDoors() {

#ifdef MULTIPLE_ELEVATOR
    canEnter=false;
#endif

    printf("*******now elevator %d close door at %d floor with %d riders*******\n\n", ElevatorID, currentfloor, occupancy);
    Alarm::instance->Pause(OPEN_AND_CLOSE_DOOR);
}

void Elevator::VisitFloor(int floor) {

#ifdef BOUNDED_ELEVATOR
    ElevatorLock->Acquire();
    ElevatorNotFull->Broadcast(ElevatorLock);
    ElevatorLock->Release();
#endif

    Alarm::instance->Pause(ELEVATOR_MOVE_ONE_FLOOR);
    currentfloor = floor;
}

bool Elevator::Enter()
{
    occupancy++;

#ifdef BOUNDED_ELEVATOR

    if(occupancy>ELEVATOR_CAPACITY) {
        occupancy--;
        if (state == UP)
            Building::instance->ElevatorUpBarrier[currentfloor]->Complete();

        else if (state == DOWN)
            Building::instance->ElevatorDownBarrier[currentfloor]->Complete();

        ElevatorLock->Acquire();
        ElevatorNotFull->Wait(ElevatorLock);
        ElevatorLock->Release();
        return false;
    }

#endif

    Alarm::instance->Pause(RIDER_ENTER_OUT);
    if (state == UP)
        Building::instance->ElevatorUpBarrier[currentfloor]->Complete();

    else if (state == DOWN)
        Building::instance->ElevatorDownBarrier[currentfloor]->Complete();

    return true;
}

void Elevator::Exit() {
    Alarm::instance->Pause(RIDER_ENTER_OUT);
    occupancy--;
    ElevatorOutBarrier[currentfloor]->Complete();
}

void Elevator::RequestFloor(int floor) {
    ElevatorOutBarrier[floor]->Wait();
}

int Elevator::GetLastRequestFloor() {
    ElevatorLock->Acquire();
    int return_value = -1;
    if (state == STOP) {
        for (int i = 0; i < numFloors; i++) {
            if (Building::instance->ElevatorUpBarrier[i]->Waiters() != 0
                || ElevatorOutBarrier[i]->Waiters() != 0
                || Building::instance->ElevatorDownBarrier[i]->Waiters() != 0) {
                return_value = i;
                break;
            }
        }
    }
    else if (state == UP) {
        for (int i = numFloors - 1; i >= currentfloor; i--) {
            if (Building::instance->ElevatorDownBarrier[i]->Waiters() != 0
                ||ElevatorOutBarrier[i]->Waiters() != 0
                ||Building::instance->ElevatorUpBarrier[i]->Waiters() != 0) {
                return_value = i;
                break;
            }
        }
    }
    else {
        for (int i = 0; i <= currentfloor; i++) {
            if (Building::instance->ElevatorDownBarrier[i]->Waiters() != 0
                ||ElevatorOutBarrier[i]->Waiters() != 0
                ||Building::instance->ElevatorUpBarrier[i]->Waiters() != 0) {
                return_value = i;
                break;
            }
        }
    }
    ElevatorLock->Release();
    return return_value;
}

void Elevator::ElevatorControl() {
    bool no_requset_flag = false;
    while (true) {
        int dest_floor = GetLastRequestFloor();
        if (state == STOP) {
            ElevatorLock->Acquire();
            printf("###now elevator %d stop at %d floor with %d riders\n" ,ElevatorID, currentfloor, occupancy);
            if (dest_floor == -1)
                HaveRequest->Wait(ElevatorLock);

            ElevatorLock->Release();
            printf("###now elevator %d at %d floor with %d riders\n" ,ElevatorID, currentfloor, occupancy);
            dest_floor = GetLastRequestFloor();
            if (dest_floor > currentfloor)
                state = UP;
            else
                state = DOWN;
        }
        else if (state == UP) {
            while (currentfloor != dest_floor && dest_floor != -1) {   
                no_requset_flag = false;
                if (Building::instance->ElevatorUpBarrier[currentfloor]->Waiters() != 0
                    || ElevatorOutBarrier[currentfloor]->Waiters() != 0) {
                    OpenDoors();
                    CloseDoors();
                }
                VisitFloor(currentfloor + 1);
                dest_floor = GetLastRequestFloor();
                printf("###now elevator %d at %d floor with %d riders\n" ,ElevatorID, currentfloor, occupancy);
            }
            //多个电梯时因为请求若被其他电梯满足则会被取消，故需在此处判断
            if (dest_floor == -1) {
                //若在上一个循环中判断的另外一个方向也无请求
                if (no_requset_flag) {
                    state = STOP;
                    continue;
                }
                state = DOWN;
                no_requset_flag = true;
                continue;
            }
            if (Building::instance->ElevatorUpBarrier[currentfloor]->Waiters() == 0) {
                state = DOWN;                       //掉头
                OpenDoors();
                CloseDoors();
                dest_floor = GetLastRequestFloor();
                if (dest_floor == -1)                  //若DOWN方向上也无新的请求则使电梯停下
                    state = STOP;
                else                                //否则向DOWN方向运行
                    VisitFloor(currentfloor - 1);   
            }
            //当所有请求中最上面的请求还是向上时 
            else {
                OpenDoors();
                CloseDoors();
                VisitFloor(currentfloor + 1);
            }
            printf("###now elevator %d at %d floor with %d riders\n", ElevatorID, currentfloor, occupancy);
        }
        else {
            while (currentfloor != dest_floor && dest_floor != -1) {
                no_requset_flag = false;
                if (Building::instance->ElevatorDownBarrier[currentfloor]->Waiters() != 0
                    || ElevatorOutBarrier[currentfloor]->Waiters() != 0) {
                    OpenDoors();
                    CloseDoors();
                }
                VisitFloor(currentfloor - 1);
                dest_floor = GetLastRequestFloor();
                printf("###now elevator %d at %d floor with %d riders\n",ElevatorID, currentfloor, occupancy);
            }
            if (dest_floor == -1) {
                if (no_requset_flag) {
                    state = STOP;
                    continue;
                }
                state = UP;
                no_requset_flag = true;
                continue;
            }
            if (Building::instance->ElevatorDownBarrier[currentfloor]->Waiters() == 0) {
                state = UP;
                OpenDoors();
                CloseDoors();
                dest_floor = GetLastRequestFloor();
                if (dest_floor == -1)
                    state = STOP;
                else
                    VisitFloor(currentfloor + 1);
            }
            else {
                OpenDoors();
                CloseDoors();
                VisitFloor(currentfloor - 1);
            }
            printf("###now elevator %d at %d floor with %d riders\n", ElevatorID, currentfloor, occupancy);

        }
        currentThread->Yield();
    }
}

void rider(int id, int srcFloor, int dstFloor) {
    Elevator *e;
    if (srcFloor == dstFloor)
        return;
    printf("+++Rider %d travelling from %d to %d\n", id, srcFloor, dstFloor);
    do {
        if (srcFloor < dstFloor) {
            printf("===Rider %d CallUp(%d)\n", id, srcFloor);
            Building::instance->CallUp(srcFloor);
            printf("===Rider %d AwaitUp(%d)\n\n", id, srcFloor);
            e = Building::instance->AwaitUp(srcFloor);
        }
        else {
            printf("===Rider %d CallDown(%d)\n", id, srcFloor);
            Building::instance->CallDown(srcFloor);
            printf("===Rider %d AwaitDown(%d)\n\n", id, srcFloor);
            e = Building::instance->AwaitDown(srcFloor);
        }
        printf(">>>Rider %d Enter() elevator %d\n", id,e->getId());
    } while (!e->Enter()); // elevator might be full!

    printf(">>>Rider %d RequestFloor(%d)\n", id, dstFloor);
    e->RequestFloor(dstFloor); // doesn't return until arrival
    printf("---Rider %d Exit()\n", id);
    e->Exit();
    printf("---Rider %d finished\n", id);
}
