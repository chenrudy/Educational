/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
- Author: Chen Rudy
- Date: 8/9/22
- Test file for waitable queue
- compile with makefile
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#include <thread>
#include <string>
#include <iostream>

#include "waitable_queue.hpp"

using namespace std;
using namespace ilrd;

Waitable_Queue<string> wq;
std::mutex g_lock;

void Write(string msg)
{
    wq.Push(msg);
}

void Read()
{
    string str;
    wq.Pop(str);

    std::unique_lock<std::mutex> lock(g_lock);
    cout << "Read: " << str;
}

void ReadWaitMil()
{
    const chrono::milliseconds t(10);
    string str = "timeout\n";
    wq.Pop(str, t);

    std::unique_lock<std::mutex> lock(g_lock);
    cout << "Mil Read Status " << str;
}

void ReadWaitSec()
{
    const chrono::seconds t(3);
    string str = "timeout\n";
    wq.Pop(str, t);

    std::unique_lock<std::mutex> lock(g_lock);
    cout << "Sec Read Status: " << str;
}

void TestPushPop()
{
    thread t3(Read);
    thread t4(Read);
    thread t6(Read);
    thread t1(Write, "en\n");
    thread t2(Write, "den\n");
    thread t5(Write, "dino\n");

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();
}

void TestPopTimeout()
{
    thread t3(Write, "it\n");
    thread t4(Write, "got\n");
    thread t1(ReadWaitMil);
    thread t2(ReadWaitSec);
    thread t5(ReadWaitMil);
    thread t6(ReadWaitSec);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();
}

int main()
{
    TestPushPop();
    TestPopTimeout();

    return 0;
}
