# 测试 栅栏

在main中写入-e参数，然后在threadtest中加入ThreadTest5函数。

ThreadTest5函数的主要思想是先创建10个线程，如果rand() % 100 == 50，那么就算这个线程完成了工作，进入Wait状态。否则切换到下一个线程。还有一个信号线程，当该线程计算得到rand() % 200 == 50时，发出Singnaled信号，开放栅栏，唤醒所有处在Wait状态中的线程，进入Complete状态，当最后一个处在Wait状态中的线程进入Complete状态时，所有线程穿过栅栏。当然这时候可能有些线程这时才完成工作，那么它们就直接穿过栅栏。
```bash
$ ./nachos -e
```

![1](https://github.com/NonupleBroken/OS_Lab_Nachos/blob/master/pic/1.png)

# 测试 闹钟

## 修改system.cc

初始的nachos代码只有在使用-rs选项的情况下才会启动时间中断，所以在threads/system.cc的Initialize函数中加入Alarm::new_instance()，再稍做修改，使得不论-rs选项使用与否，时间中断都会被启用。

## 测试闹钟

在main中写入-g参数，然后在threadtest中加入ThreadTest6函数。

ThreadTest6的主要思想是创建3个线程，这3个线程分别睡眠3、2、1个单位时间。

由于Nachos中没有线程处于runnable状态，哪怕有线程在等待闹钟，nachos也将自动退出，为了避免这种情况的发生，在Alarm::Pause(int howLong)函数中，当发现自己将成为唯一一个等待闹钟的线程时，创建一个“打酱油”线程，该线程反复检查当前有几个等待闹钟的线程，若为零则结束，若不为零则切换。

因此线程0创建一个休眠2个单位时间的线程3，来测试该功能。
```bash
$ ./nachos -g
```
可见闹钟功能正常，”打酱油“线程也能正常运作。

![2](https://github.com/NonupleBroken/OS_Lab_Nachos/blob/master/pic/2.png)

# 测试 电梯

电梯的算法主要思想：
电梯获取当前方向要去的最高（或最低）楼层，然后在上升（或下降）过程中检测当前楼层是否有请求，若有请求则停下。

在main中写入-g参数，然后在threadtest中加入ThreadTest7函数。

这里定义了一些常量，例如：楼层数 num_floors 为10，总乘客数 total_riders 为20，电梯个数 elevator_num 为 2。

## 单电梯无限容量

在Elevator.h中 define SINGLE_ELEVATOR 以及 define UNBOUNDED_ELEVATOR。

这里定义了一些常量，例如：电梯开关门的时间 OPEN_AND_CLOSE_DOOR 为20，一个乘客进出电梯的时间 RIDER_ENTER_OUT 为10，电梯移动一层楼的时间 ELEVATOR_MOVE_ONE_FLOOR 为40。
```bash
$ ./nachos -h
```

```
###now elevator 0 stop at 0 floor with 0 riders

+++rider 0 start at 560 ticks from 6 floor to 8 floor
+++Rider 0 travelling from 6 to 8
===Rider 0 CallUp(6)
===Rider 0 AwaitUp(6)


+++rider 1 start at 660 ticks from 6 floor to 8 floor
+++Rider 1 travelling from 6 to 8
===Rider 1 CallUp(6)
===Rider 1 AwaitUp(6)


+++rider 2 start at 750 ticks from 6 floor to 8 floor
+++Rider 2 travelling from 6 to 8
===Rider 2 CallUp(6)
===Rider 2 AwaitUp(6)


+++rider 3 start at 840 ticks from 6 floor to 8 floor
+++Rider 3 travelling from 6 to 8
===Rider 3 CallUp(6)
===Rider 3 AwaitUp(6)


+++rider 4 start at 930 ticks from 6 floor to 8 floor
+++Rider 4 travelling from 6 to 8
===Rider 4 CallUp(6)
===Rider 4 AwaitUp(6)


+++rider 5 start at 1020 ticks from 6 floor to 8 floor
+++Rider 5 travelling from 6 to 8
===Rider 5 CallUp(6)
===Rider 5 AwaitUp(6)


+++rider 6 start at 1110 ticks from 6 floor to 8 floor
+++Rider 6 travelling from 6 to 8
===Rider 6 CallUp(6)
===Rider 6 AwaitUp(6)


+++rider 7 start at 1210 ticks from 6 floor to 8 floor
+++Rider 7 travelling from 6 to 8
===Rider 7 CallUp(6)
===Rider 7 AwaitUp(6)


+++rider 8 start at 1300 ticks from 6 floor to 8 floor
+++Rider 8 travelling from 6 to 8
===Rider 8 CallUp(6)
===Rider 8 AwaitUp(6)


+++rider 9 start at 1390 ticks from 6 floor to 8 floor
+++Rider 9 travelling from 6 to 8
===Rider 9 CallUp(6)
===Rider 9 AwaitUp(6)


+++rider 10 start at 1480 ticks from 6 floor to 8 floor
+++Rider 10 travelling from 6 to 8
===Rider 10 CallUp(6)
===Rider 10 AwaitUp(6)


+++rider 11 start at 1570 ticks from 6 floor to 8 floor
+++Rider 11 travelling from 6 to 8
===Rider 11 CallUp(6)
===Rider 11 AwaitUp(6)


+++rider 12 start at 1660 ticks from 6 floor to 8 floor
+++Rider 12 travelling from 6 to 8
===Rider 12 CallUp(6)
===Rider 12 AwaitUp(6)


+++rider 13 start at 1750 ticks from 6 floor to 8 floor
+++Rider 13 travelling from 6 to 8
===Rider 13 CallUp(6)
===Rider 13 AwaitUp(6)


+++rider 14 start at 1840 ticks from 6 floor to 8 floor
+++Rider 14 travelling from 6 to 8
===Rider 14 CallUp(6)
===Rider 14 AwaitUp(6)


+++rider 15 start at 1930 ticks from 6 floor to 8 floor
+++Rider 15 travelling from 6 to 8
===Rider 15 CallUp(6)
===Rider 15 AwaitUp(6)

###now elevator 0 at 0 floor with 0 riders

+++rider 16 start at 2130 ticks from 6 floor to 8 floor
+++Rider 16 travelling from 6 to 8
===Rider 16 CallUp(6)
===Rider 16 AwaitUp(6)


+++rider 17 start at 2220 ticks from 6 floor to 8 floor
+++Rider 17 travelling from 6 to 8
===Rider 17 CallUp(6)
===Rider 17 AwaitUp(6)


+++rider 18 start at 2310 ticks from 6 floor to 8 floor
+++Rider 18 travelling from 6 to 8
===Rider 18 CallUp(6)
===Rider 18 AwaitUp(6)


+++rider 19 start at 2400 ticks from 6 floor to 8 floor
+++Rider 19 travelling from 6 to 8
===Rider 19 CallUp(6)
===Rider 19 AwaitUp(6)

###now elevator 0 at 1 floor with 0 riders
###now elevator 0 at 2 floor with 0 riders
###now elevator 0 at 3 floor with 0 riders
###now elevator 0 at 4 floor with 0 riders
###now elevator 0 at 5 floor with 0 riders
###now elevator 0 at 6 floor with 0 riders

*******now elevator 0 open door at 6 floor with 0 riders*******
>>>Rider 0 Enter() elevator 0
>>>Rider 1 Enter() elevator 0
>>>Rider 2 Enter() elevator 0
>>>Rider 3 Enter() elevator 0
>>>Rider 4 Enter() elevator 0
>>>Rider 5 Enter() elevator 0
>>>Rider 6 Enter() elevator 0
>>>Rider 7 Enter() elevator 0
>>>Rider 8 Enter() elevator 0
>>>Rider 9 Enter() elevator 0
>>>Rider 10 Enter() elevator 0
>>>Rider 11 Enter() elevator 0
>>>Rider 12 Enter() elevator 0
>>>Rider 13 Enter() elevator 0
>>>Rider 14 Enter() elevator 0
>>>Rider 15 Enter() elevator 0
>>>Rider 16 Enter() elevator 0
>>>Rider 17 Enter() elevator 0
>>>Rider 18 Enter() elevator 0
>>>Rider 19 Enter() elevator 0
>>>Rider 19 RequestFloor(8)
*******now elevator 0 close door at 6 floor with 20 riders*******

>>>Rider 0 RequestFloor(8)
>>>Rider 1 RequestFloor(8)
>>>Rider 2 RequestFloor(8)
>>>Rider 3 RequestFloor(8)
>>>Rider 4 RequestFloor(8)
>>>Rider 5 RequestFloor(8)
>>>Rider 6 RequestFloor(8)
>>>Rider 7 RequestFloor(8)
>>>Rider 8 RequestFloor(8)
>>>Rider 9 RequestFloor(8)
>>>Rider 10 RequestFloor(8)
>>>Rider 11 RequestFloor(8)
>>>Rider 12 RequestFloor(8)
>>>Rider 13 RequestFloor(8)
>>>Rider 14 RequestFloor(8)
>>>Rider 15 RequestFloor(8)
>>>Rider 16 RequestFloor(8)
>>>Rider 17 RequestFloor(8)
>>>Rider 18 RequestFloor(8)
###now elevator 0 at 7 floor with 20 riders
###now elevator 0 at 8 floor with 20 riders

*******now elevator 0 open door at 8 floor with 20 riders*******
---Rider 19 Exit()
---Rider 0 Exit()
---Rider 1 Exit()
---Rider 2 Exit()
---Rider 3 Exit()
---Rider 4 Exit()
---Rider 5 Exit()
---Rider 6 Exit()
---Rider 7 Exit()
---Rider 8 Exit()
---Rider 9 Exit()
---Rider 10 Exit()
---Rider 11 Exit()
---Rider 12 Exit()
---Rider 13 Exit()
---Rider 14 Exit()
---Rider 15 Exit()
---Rider 16 Exit()
---Rider 17 Exit()
---Rider 18 Exit()
---Rider 18 finished
------rider 18 costs 44680------
*******now elevator 0 close door at 8 floor with 0 riders*******

---Rider 19 finished
------rider 19 costs 44700------
---Rider 0 finished
------rider 0 costs 46590------
---Rider 1 finished
------rider 1 costs 46540------
---Rider 2 finished
------rider 2 costs 46500------
---Rider 3 finished
------rider 3 costs 46460------
---Rider 4 finished
------rider 4 costs 46420------
---Rider 5 finished
------rider 5 costs 46380------
---Rider 6 finished
------rider 6 costs 46340------
---Rider 7 finished
------rider 7 costs 46290------
---Rider 8 finished
------rider 8 costs 46250------
---Rider 9 finished
------rider 9 costs 46210------
---Rider 10 finished
------rider 10 costs 46170------
---Rider 11 finished
------rider 11 costs 46130------
---Rider 12 finished
------rider 12 costs 46090------
---Rider 13 finished
------rider 13 costs 46050------
---Rider 14 finished
------rider 14 costs 46010------
---Rider 15 finished
------rider 15 costs 45970------
---Rider 16 finished
------rider 16 costs 45820------
---Rider 17 finished
------rider 17 costs 45780------

-------avg turnaround time 46069.00-------
-------max turnaround time 46590-------
-------min turnaround time 44680-------

###now elevator 0 at 8 floor with 0 riders
###now elevator 0 stop at 8 floor with 0 riders
```

单电梯无限容量运行正常。

## 单电梯有限容量

与无限容量不同的是，若电梯已满，乘客需继续等待。

在Elevator.h中 define SINGLE_ELEVATOR 以及 define BOUNDED_ELEVATOR。

```bash
$ ./nachos -h
```

```
###now elevator 0 stop at 0 floor with 0 riders

+++rider 0 start at 560 ticks from 9 floor to 8 floor
+++Rider 0 travelling from 9 to 8
===Rider 0 CallDown(9)
===Rider 0 AwaitDown(9)


+++rider 1 start at 660 ticks from 9 floor to 8 floor
+++Rider 1 travelling from 9 to 8
===Rider 1 CallDown(9)
===Rider 1 AwaitDown(9)


+++rider 2 start at 750 ticks from 9 floor to 8 floor
+++Rider 2 travelling from 9 to 8
===Rider 2 CallDown(9)
===Rider 2 AwaitDown(9)


+++rider 3 start at 840 ticks from 9 floor to 8 floor
+++Rider 3 travelling from 9 to 8
===Rider 3 CallDown(9)
===Rider 3 AwaitDown(9)


+++rider 4 start at 930 ticks from 9 floor to 8 floor
+++Rider 4 travelling from 9 to 8
===Rider 4 CallDown(9)
===Rider 4 AwaitDown(9)


+++rider 5 start at 1020 ticks from 9 floor to 8 floor
+++Rider 5 travelling from 9 to 8
===Rider 5 CallDown(9)
===Rider 5 AwaitDown(9)


+++rider 6 start at 1110 ticks from 9 floor to 8 floor
+++Rider 6 travelling from 9 to 8
===Rider 6 CallDown(9)
===Rider 6 AwaitDown(9)


+++rider 7 start at 1210 ticks from 9 floor to 8 floor
+++Rider 7 travelling from 9 to 8
===Rider 7 CallDown(9)
===Rider 7 AwaitDown(9)


+++rider 8 start at 1300 ticks from 9 floor to 8 floor
+++Rider 8 travelling from 9 to 8
===Rider 8 CallDown(9)
===Rider 8 AwaitDown(9)


+++rider 9 start at 1390 ticks from 9 floor to 8 floor
+++Rider 9 travelling from 9 to 8
===Rider 9 CallDown(9)
===Rider 9 AwaitDown(9)


+++rider 10 start at 1480 ticks from 9 floor to 8 floor
+++Rider 10 travelling from 9 to 8
===Rider 10 CallDown(9)
===Rider 10 AwaitDown(9)


+++rider 11 start at 1570 ticks from 9 floor to 8 floor
+++Rider 11 travelling from 9 to 8
===Rider 11 CallDown(9)
===Rider 11 AwaitDown(9)


+++rider 12 start at 1660 ticks from 9 floor to 8 floor
+++Rider 12 travelling from 9 to 8
===Rider 12 CallDown(9)
===Rider 12 AwaitDown(9)


+++rider 13 start at 1750 ticks from 9 floor to 8 floor
+++Rider 13 travelling from 9 to 8
===Rider 13 CallDown(9)
===Rider 13 AwaitDown(9)


+++rider 14 start at 1840 ticks from 9 floor to 8 floor
+++Rider 14 travelling from 9 to 8
===Rider 14 CallDown(9)
===Rider 14 AwaitDown(9)


+++rider 15 start at 1930 ticks from 9 floor to 8 floor
+++Rider 15 travelling from 9 to 8
===Rider 15 CallDown(9)
===Rider 15 AwaitDown(9)

###now elevator 0 at 0 floor with 0 riders

+++rider 16 start at 2130 ticks from 9 floor to 8 floor
+++Rider 16 travelling from 9 to 8
===Rider 16 CallDown(9)
===Rider 16 AwaitDown(9)


+++rider 17 start at 2220 ticks from 9 floor to 8 floor
+++Rider 17 travelling from 9 to 8
===Rider 17 CallDown(9)
===Rider 17 AwaitDown(9)


+++rider 18 start at 2310 ticks from 9 floor to 8 floor
+++Rider 18 travelling from 9 to 8
===Rider 18 CallDown(9)
===Rider 18 AwaitDown(9)


+++rider 19 start at 2400 ticks from 9 floor to 8 floor
+++Rider 19 travelling from 9 to 8
===Rider 19 CallDown(9)
===Rider 19 AwaitDown(9)

###now elevator 0 at 1 floor with 0 riders
###now elevator 0 at 2 floor with 0 riders
###now elevator 0 at 3 floor with 0 riders
###now elevator 0 at 4 floor with 0 riders
###now elevator 0 at 5 floor with 0 riders
###now elevator 0 at 6 floor with 0 riders
###now elevator 0 at 7 floor with 0 riders
###now elevator 0 at 8 floor with 0 riders
###now elevator 0 at 9 floor with 0 riders

*******now elevator 0 open door at 9 floor with 0 riders*******
>>>Rider 0 Enter() elevator 0
>>>Rider 1 Enter() elevator 0
>>>Rider 2 Enter() elevator 0
>>>Rider 3 Enter() elevator 0
>>>Rider 4 Enter() elevator 0
>>>Rider 5 Enter() elevator 0
>>>Rider 6 Enter() elevator 0
>>>Rider 7 Enter() elevator 0
>>>Rider 8 Enter() elevator 0
>>>Rider 9 Enter() elevator 0
>>>Rider 10 Enter() elevator 0
>>>Rider 11 Enter() elevator 0
>>>Rider 12 Enter() elevator 0
>>>Rider 13 Enter() elevator 0
>>>Rider 14 Enter() elevator 0
>>>Rider 15 Enter() elevator 0
>>>Rider 16 Enter() elevator 0
>>>Rider 17 Enter() elevator 0
>>>Rider 18 Enter() elevator 0
>>>Rider 19 Enter() elevator 0
>>>Rider 5 RequestFloor(8)
*******now elevator 0 close door at 9 floor with 6 riders*******

>>>Rider 0 RequestFloor(8)
>>>Rider 1 RequestFloor(8)
>>>Rider 2 RequestFloor(8)
>>>Rider 3 RequestFloor(8)
>>>Rider 4 RequestFloor(8)
===Rider 6 CallDown(9)
===Rider 6 AwaitDown(9)

===Rider 7 CallDown(9)
===Rider 7 AwaitDown(9)

===Rider 8 CallDown(9)
===Rider 8 AwaitDown(9)

===Rider 9 CallDown(9)
===Rider 9 AwaitDown(9)

===Rider 10 CallDown(9)
===Rider 10 AwaitDown(9)

===Rider 11 CallDown(9)
===Rider 11 AwaitDown(9)

===Rider 12 CallDown(9)
===Rider 12 AwaitDown(9)

===Rider 13 CallDown(9)
===Rider 13 AwaitDown(9)

===Rider 14 CallDown(9)
===Rider 14 AwaitDown(9)

===Rider 15 CallDown(9)
===Rider 15 AwaitDown(9)

===Rider 16 CallDown(9)
===Rider 16 AwaitDown(9)

===Rider 17 CallDown(9)
===Rider 17 AwaitDown(9)

===Rider 18 CallDown(9)
===Rider 18 AwaitDown(9)

===Rider 19 CallDown(9)
===Rider 19 AwaitDown(9)

###now elevator 0 at 8 floor with 6 riders

*******now elevator 0 open door at 8 floor with 6 riders*******
---Rider 5 Exit()
---Rider 0 Exit()
---Rider 1 Exit()
---Rider 2 Exit()
---Rider 3 Exit()
---Rider 4 Exit()
---Rider 4 finished
------rider 4 costs 53100------
*******now elevator 0 close door at 8 floor with 0 riders*******

---Rider 5 finished
------rider 5 costs 53120------
---Rider 0 finished
------rider 0 costs 53630------
---Rider 1 finished
------rider 1 costs 53580------
---Rider 2 finished
------rider 2 costs 53540------
---Rider 3 finished
------rider 3 costs 53500------
###now elevator 0 at 9 floor with 0 riders

*******now elevator 0 open door at 9 floor with 0 riders*******
>>>Rider 6 Enter() elevator 0
>>>Rider 7 Enter() elevator 0
>>>Rider 8 Enter() elevator 0
>>>Rider 9 Enter() elevator 0
>>>Rider 10 Enter() elevator 0
>>>Rider 11 Enter() elevator 0
>>>Rider 12 Enter() elevator 0
>>>Rider 13 Enter() elevator 0
>>>Rider 14 Enter() elevator 0
>>>Rider 15 Enter() elevator 0
>>>Rider 16 Enter() elevator 0
>>>Rider 17 Enter() elevator 0
>>>Rider 18 Enter() elevator 0
>>>Rider 19 Enter() elevator 0
>>>Rider 11 RequestFloor(8)
*******now elevator 0 close door at 9 floor with 6 riders*******

>>>Rider 6 RequestFloor(8)
>>>Rider 7 RequestFloor(8)
>>>Rider 8 RequestFloor(8)
>>>Rider 9 RequestFloor(8)
>>>Rider 10 RequestFloor(8)
===Rider 12 CallDown(9)
===Rider 12 AwaitDown(9)

===Rider 13 CallDown(9)
===Rider 13 AwaitDown(9)

===Rider 14 CallDown(9)
===Rider 14 AwaitDown(9)

===Rider 15 CallDown(9)
===Rider 15 AwaitDown(9)

===Rider 16 CallDown(9)
===Rider 16 AwaitDown(9)

===Rider 17 CallDown(9)
===Rider 17 AwaitDown(9)

===Rider 18 CallDown(9)
===Rider 18 AwaitDown(9)

===Rider 19 CallDown(9)
===Rider 19 AwaitDown(9)

###now elevator 0 at 8 floor with 6 riders

*******now elevator 0 open door at 8 floor with 6 riders*******
---Rider 11 Exit()
---Rider 6 Exit()
---Rider 7 Exit()
---Rider 8 Exit()
---Rider 9 Exit()
---Rider 10 Exit()
---Rider 10 finished
------rider 10 costs 72650------
*******now elevator 0 close door at 8 floor with 0 riders*******

---Rider 11 finished
------rider 11 costs 72670------
---Rider 6 finished
------rider 6 costs 73180------
---Rider 7 finished
------rider 7 costs 73130------
---Rider 8 finished
------rider 8 costs 73090------
---Rider 9 finished
------rider 9 costs 73050------
###now elevator 0 at 9 floor with 0 riders

*******now elevator 0 open door at 9 floor with 0 riders*******
>>>Rider 12 Enter() elevator 0
>>>Rider 13 Enter() elevator 0
>>>Rider 14 Enter() elevator 0
>>>Rider 15 Enter() elevator 0
>>>Rider 16 Enter() elevator 0
>>>Rider 17 Enter() elevator 0
>>>Rider 18 Enter() elevator 0
>>>Rider 19 Enter() elevator 0
>>>Rider 17 RequestFloor(8)
*******now elevator 0 close door at 9 floor with 6 riders*******

>>>Rider 12 RequestFloor(8)
>>>Rider 13 RequestFloor(8)
>>>Rider 14 RequestFloor(8)
>>>Rider 15 RequestFloor(8)
>>>Rider 16 RequestFloor(8)
===Rider 18 CallDown(9)
===Rider 18 AwaitDown(9)

===Rider 19 CallDown(9)
===Rider 19 AwaitDown(9)

###now elevator 0 at 8 floor with 6 riders

*******now elevator 0 open door at 8 floor with 6 riders*******
---Rider 17 Exit()
---Rider 12 Exit()
---Rider 13 Exit()
---Rider 14 Exit()
---Rider 15 Exit()
---Rider 16 Exit()
---Rider 16 finished
------rider 16 costs 92000------
*******now elevator 0 close door at 8 floor with 0 riders*******

---Rider 17 finished
------rider 17 costs 92020------
---Rider 12 finished
------rider 12 costs 92630------
---Rider 13 finished
------rider 13 costs 92590------
---Rider 14 finished
------rider 14 costs 92550------
---Rider 15 finished
------rider 15 costs 92510------
###now elevator 0 at 9 floor with 0 riders

*******now elevator 0 open door at 9 floor with 0 riders*******
>>>Rider 18 Enter() elevator 0
>>>Rider 19 Enter() elevator 0
>>>Rider 19 RequestFloor(8)
*******now elevator 0 close door at 9 floor with 2 riders*******

>>>Rider 18 RequestFloor(8)
###now elevator 0 at 8 floor with 2 riders

*******now elevator 0 open door at 8 floor with 2 riders*******
---Rider 19 Exit()
---Rider 18 Exit()
---Rider 18 finished
------rider 18 costs 111110------
*******now elevator 0 close door at 8 floor with 0 riders*******

---Rider 19 finished
------rider 19 costs 111130------

-------avg turnaround time 76739.00-------
-------max turnaround time 111130-------
-------min turnaround time 53100-------

###now elevator 0 at 8 floor with 0 riders
###now elevator 0 stop at 8 floor with 0 riders
```

单电梯有限容量运行正常。

## 多电梯有限容量

当乘客按下上行（或下行）按钮时，能选择一个合适的电梯提供服务。

在Elevator.h中 define MULTIPLE_ELEVATOR 以及 define BOUNDED_ELEVATOR。

```bash
$ ./nachos -h
```

```
###now elevator 0 stop at 0 floor with 0 riders
###now elevator 1 stop at 0 floor with 0 riders

+++rider 0 start at 660 ticks from 7 floor to 0 floor
+++Rider 0 travelling from 7 to 0
===Rider 0 CallDown(7)
===Rider 0 AwaitDown(7)


+++rider 1 start at 760 ticks from 7 floor to 0 floor
+++Rider 1 travelling from 7 to 0
===Rider 1 CallDown(7)
===Rider 1 AwaitDown(7)


+++rider 2 start at 850 ticks from 7 floor to 0 floor
+++Rider 2 travelling from 7 to 0
===Rider 2 CallDown(7)
===Rider 2 AwaitDown(7)


+++rider 3 start at 940 ticks from 7 floor to 0 floor
+++Rider 3 travelling from 7 to 0
===Rider 3 CallDown(7)
===Rider 3 AwaitDown(7)


+++rider 4 start at 1030 ticks from 7 floor to 0 floor
+++Rider 4 travelling from 7 to 0
===Rider 4 CallDown(7)
===Rider 4 AwaitDown(7)


+++rider 5 start at 1120 ticks from 7 floor to 0 floor
+++Rider 5 travelling from 7 to 0
===Rider 5 CallDown(7)
===Rider 5 AwaitDown(7)


+++rider 6 start at 1210 ticks from 7 floor to 0 floor
+++Rider 6 travelling from 7 to 0
===Rider 6 CallDown(7)
===Rider 6 AwaitDown(7)


+++rider 7 start at 1310 ticks from 7 floor to 0 floor
+++Rider 7 travelling from 7 to 0
===Rider 7 CallDown(7)
===Rider 7 AwaitDown(7)


+++rider 8 start at 1400 ticks from 7 floor to 0 floor
+++Rider 8 travelling from 7 to 0
===Rider 8 CallDown(7)
===Rider 8 AwaitDown(7)


+++rider 9 start at 1490 ticks from 7 floor to 0 floor
+++Rider 9 travelling from 7 to 0
===Rider 9 CallDown(7)
===Rider 9 AwaitDown(7)


+++rider 10 start at 1580 ticks from 7 floor to 0 floor
+++Rider 10 travelling from 7 to 0
===Rider 10 CallDown(7)
===Rider 10 AwaitDown(7)


+++rider 11 start at 1670 ticks from 7 floor to 0 floor
+++Rider 11 travelling from 7 to 0
===Rider 11 CallDown(7)
===Rider 11 AwaitDown(7)


+++rider 12 start at 1760 ticks from 7 floor to 0 floor
+++Rider 12 travelling from 7 to 0
===Rider 12 CallDown(7)
===Rider 12 AwaitDown(7)


+++rider 13 start at 1850 ticks from 7 floor to 0 floor
+++Rider 13 travelling from 7 to 0
===Rider 13 CallDown(7)
===Rider 13 AwaitDown(7)


+++rider 14 start at 1940 ticks from 7 floor to 0 floor
+++Rider 14 travelling from 7 to 0
===Rider 14 CallDown(7)
===Rider 14 AwaitDown(7)


+++rider 15 start at 2030 ticks from 7 floor to 0 floor
+++Rider 15 travelling from 7 to 0
===Rider 15 CallDown(7)
===Rider 15 AwaitDown(7)

###now elevator 0 at 0 floor with 0 riders

+++rider 16 start at 2230 ticks from 7 floor to 0 floor
+++Rider 16 travelling from 7 to 0
===Rider 16 CallDown(7)
===Rider 16 AwaitDown(7)


+++rider 17 start at 2330 ticks from 7 floor to 0 floor
+++Rider 17 travelling from 7 to 0
===Rider 17 CallDown(7)
===Rider 17 AwaitDown(7)


+++rider 18 start at 2420 ticks from 7 floor to 0 floor
+++Rider 18 travelling from 7 to 0
===Rider 18 CallDown(7)
===Rider 18 AwaitDown(7)


+++rider 19 start at 2510 ticks from 7 floor to 0 floor
+++Rider 19 travelling from 7 to 0
===Rider 19 CallDown(7)
===Rider 19 AwaitDown(7)

###now elevator 1 at 0 floor with 0 riders
###now elevator 0 at 1 floor with 0 riders
###now elevator 1 at 1 floor with 0 riders
###now elevator 0 at 2 floor with 0 riders
###now elevator 1 at 2 floor with 0 riders
###now elevator 0 at 3 floor with 0 riders
###now elevator 1 at 3 floor with 0 riders
###now elevator 0 at 4 floor with 0 riders
###now elevator 1 at 4 floor with 0 riders
###now elevator 0 at 5 floor with 0 riders
###now elevator 1 at 5 floor with 0 riders
###now elevator 0 at 6 floor with 0 riders
###now elevator 1 at 6 floor with 0 riders
###now elevator 0 at 7 floor with 0 riders
###now elevator 1 at 7 floor with 0 riders

*******now elevator 0 open door at 7 floor with 0 riders*******
>>>Rider 0 Enter() elevator 0
>>>Rider 1 Enter() elevator 0
>>>Rider 2 Enter() elevator 0
>>>Rider 3 Enter() elevator 0
>>>Rider 4 Enter() elevator 0
>>>Rider 5 Enter() elevator 0
>>>Rider 6 Enter() elevator 0
>>>Rider 7 Enter() elevator 0
>>>Rider 8 Enter() elevator 0
>>>Rider 9 Enter() elevator 0
>>>Rider 10 Enter() elevator 0
>>>Rider 11 Enter() elevator 0
>>>Rider 12 Enter() elevator 0
>>>Rider 13 Enter() elevator 0
>>>Rider 14 Enter() elevator 0
>>>Rider 15 Enter() elevator 0
>>>Rider 16 Enter() elevator 0
>>>Rider 17 Enter() elevator 0

*******now elevator 1 open door at 7 floor with 0 riders*******
>>>Rider 18 Enter() elevator 0
>>>Rider 19 Enter() elevator 0
>>>Rider 5 RequestFloor(0)
*******now elevator 0 close door at 7 floor with 6 riders*******

*******now elevator 1 close door at 7 floor with 0 riders*******

>>>Rider 0 RequestFloor(0)
>>>Rider 1 RequestFloor(0)
>>>Rider 2 RequestFloor(0)
>>>Rider 3 RequestFloor(0)
>>>Rider 4 RequestFloor(0)
===Rider 6 CallDown(7)
===Rider 6 AwaitDown(7)

===Rider 7 CallDown(7)
===Rider 7 AwaitDown(7)

===Rider 8 CallDown(7)
===Rider 8 AwaitDown(7)

===Rider 9 CallDown(7)
===Rider 9 AwaitDown(7)

===Rider 10 CallDown(7)
===Rider 10 AwaitDown(7)

===Rider 11 CallDown(7)
===Rider 11 AwaitDown(7)

===Rider 12 CallDown(7)
===Rider 12 AwaitDown(7)

===Rider 13 CallDown(7)
===Rider 13 AwaitDown(7)

===Rider 14 CallDown(7)
===Rider 14 AwaitDown(7)

===Rider 15 CallDown(7)
===Rider 15 AwaitDown(7)

===Rider 16 CallDown(7)
===Rider 16 AwaitDown(7)

===Rider 17 CallDown(7)
===Rider 17 AwaitDown(7)

===Rider 18 CallDown(7)
===Rider 18 AwaitDown(7)

===Rider 19 CallDown(7)
===Rider 19 AwaitDown(7)

###now elevator 0 at 6 floor with 6 riders
###now elevator 1 at 6 floor with 0 riders
###now elevator 0 at 5 floor with 6 riders
###now elevator 1 at 7 floor with 0 riders

*******now elevator 1 open door at 7 floor with 0 riders*******
>>>Rider 6 Enter() elevator 1
>>>Rider 7 Enter() elevator 1
>>>Rider 8 Enter() elevator 1
>>>Rider 9 Enter() elevator 1
>>>Rider 10 Enter() elevator 1
>>>Rider 11 Enter() elevator 1
>>>Rider 12 Enter() elevator 1
>>>Rider 13 Enter() elevator 1
###now elevator 0 at 4 floor with 6 riders
>>>Rider 14 Enter() elevator 1
>>>Rider 15 Enter() elevator 1
>>>Rider 16 Enter() elevator 1
>>>Rider 17 Enter() elevator 1
>>>Rider 18 Enter() elevator 1
>>>Rider 19 Enter() elevator 1
>>>Rider 11 RequestFloor(0)
*******now elevator 1 close door at 7 floor with 6 riders*******

>>>Rider 6 RequestFloor(0)
>>>Rider 7 RequestFloor(0)
>>>Rider 8 RequestFloor(0)
>>>Rider 9 RequestFloor(0)
>>>Rider 10 RequestFloor(0)
===Rider 12 CallDown(7)
===Rider 12 AwaitDown(7)

===Rider 13 CallDown(7)
===Rider 13 AwaitDown(7)

===Rider 14 CallDown(7)
===Rider 14 AwaitDown(7)

===Rider 15 CallDown(7)
===Rider 15 AwaitDown(7)

===Rider 16 CallDown(7)
===Rider 16 AwaitDown(7)

===Rider 17 CallDown(7)
===Rider 17 AwaitDown(7)

===Rider 18 CallDown(7)
===Rider 18 AwaitDown(7)

===Rider 19 CallDown(7)
===Rider 19 AwaitDown(7)

###now elevator 0 at 3 floor with 6 riders
###now elevator 1 at 6 floor with 6 riders
###now elevator 0 at 2 floor with 6 riders
###now elevator 1 at 5 floor with 6 riders
###now elevator 0 at 1 floor with 6 riders
###now elevator 1 at 4 floor with 6 riders
###now elevator 0 at 0 floor with 6 riders

*******now elevator 0 open door at 0 floor with 6 riders*******
---Rider 5 Exit()
---Rider 0 Exit()
---Rider 1 Exit()
---Rider 2 Exit()
---Rider 3 Exit()
---Rider 4 Exit()
###now elevator 1 at 3 floor with 6 riders
---Rider 4 finished
------rider 4 costs 72000------
*******now elevator 0 close door at 0 floor with 0 riders*******

---Rider 5 finished
------rider 5 costs 72020------
---Rider 0 finished
------rider 0 costs 72530------
---Rider 1 finished
------rider 1 costs 72480------
---Rider 2 finished
------rider 2 costs 72440------
---Rider 3 finished
------rider 3 costs 72400------
###now elevator 1 at 2 floor with 6 riders
###now elevator 0 at 1 floor with 0 riders
###now elevator 1 at 1 floor with 6 riders
###now elevator 0 at 2 floor with 0 riders
###now elevator 1 at 0 floor with 6 riders

*******now elevator 1 open door at 0 floor with 6 riders*******
---Rider 11 Exit()
---Rider 6 Exit()
---Rider 7 Exit()
---Rider 8 Exit()
---Rider 9 Exit()
---Rider 10 Exit()
###now elevator 0 at 3 floor with 0 riders
---Rider 10 finished
------rider 10 costs 86750------
*******now elevator 1 close door at 0 floor with 0 riders*******

---Rider 11 finished
------rider 11 costs 86770------
---Rider 6 finished
------rider 6 costs 87280------
---Rider 7 finished
------rider 7 costs 87230------
---Rider 8 finished
------rider 8 costs 87190------
---Rider 9 finished
------rider 9 costs 87150------
###now elevator 0 at 4 floor with 0 riders
###now elevator 1 at 1 floor with 0 riders
###now elevator 0 at 5 floor with 0 riders
###now elevator 1 at 2 floor with 0 riders
###now elevator 0 at 6 floor with 0 riders
###now elevator 1 at 3 floor with 0 riders
###now elevator 0 at 7 floor with 0 riders

*******now elevator 0 open door at 7 floor with 0 riders*******
>>>Rider 12 Enter() elevator 0
>>>Rider 13 Enter() elevator 0
>>>Rider 14 Enter() elevator 0
>>>Rider 15 Enter() elevator 0
>>>Rider 16 Enter() elevator 0
>>>Rider 17 Enter() elevator 0
>>>Rider 18 Enter() elevator 0
>>>Rider 19 Enter() elevator 0
###now elevator 1 at 4 floor with 0 riders
>>>Rider 17 RequestFloor(0)
*******now elevator 0 close door at 7 floor with 6 riders*******

>>>Rider 12 RequestFloor(0)
>>>Rider 13 RequestFloor(0)
>>>Rider 14 RequestFloor(0)
>>>Rider 15 RequestFloor(0)
>>>Rider 16 RequestFloor(0)
===Rider 18 CallDown(7)
===Rider 18 AwaitDown(7)

===Rider 19 CallDown(7)
===Rider 19 AwaitDown(7)

###now elevator 1 at 5 floor with 0 riders
###now elevator 0 at 6 floor with 6 riders
###now elevator 1 at 6 floor with 0 riders
###now elevator 0 at 5 floor with 6 riders
###now elevator 1 at 7 floor with 0 riders

*******now elevator 1 open door at 7 floor with 0 riders*******
>>>Rider 18 Enter() elevator 1
>>>Rider 19 Enter() elevator 1
###now elevator 0 at 4 floor with 6 riders
>>>Rider 19 RequestFloor(0)
*******now elevator 1 close door at 7 floor with 2 riders*******

>>>Rider 18 RequestFloor(0)
###now elevator 0 at 3 floor with 6 riders
###now elevator 1 at 6 floor with 2 riders
###now elevator 0 at 2 floor with 6 riders
###now elevator 1 at 5 floor with 2 riders
###now elevator 0 at 1 floor with 6 riders
###now elevator 1 at 4 floor with 2 riders
###now elevator 0 at 0 floor with 6 riders

*******now elevator 0 open door at 0 floor with 6 riders*******
---Rider 17 Exit()
---Rider 12 Exit()
---Rider 13 Exit()
---Rider 14 Exit()
---Rider 15 Exit()
---Rider 16 Exit()
###now elevator 1 at 3 floor with 2 riders
---Rider 16 finished
------rider 16 costs 140400------
*******now elevator 0 close door at 0 floor with 0 riders*******

---Rider 17 finished
------rider 17 costs 140410------
---Rider 12 finished
------rider 12 costs 141030------
---Rider 13 finished
------rider 13 costs 140990------
---Rider 14 finished
------rider 14 costs 140950------
---Rider 15 finished
------rider 15 costs 140910------
###now elevator 0 at 0 floor with 0 riders
###now elevator 0 stop at 0 floor with 0 riders
###now elevator 1 at 2 floor with 2 riders
###now elevator 1 at 1 floor with 2 riders
###now elevator 1 at 0 floor with 2 riders

*******now elevator 1 open door at 0 floor with 2 riders*******
---Rider 19 Exit()
---Rider 18 Exit()
---Rider 18 finished
------rider 18 costs 154800------
*******now elevator 1 close door at 0 floor with 0 riders*******

---Rider 19 finished
------rider 19 costs 154820------

-------avg turnaround time 105527.50-------
-------max turnaround time 154820-------
-------min turnaround time 72000-------

###now elevator 1 at 0 floor with 0 riders
###now elevator 1 stop at 0 floor with 0 riders
```

多电梯有限容量运行正常。