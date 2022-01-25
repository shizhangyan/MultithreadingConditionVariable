// MultithreadingContitionVariable.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <thread>
#include <condition_variable>
#include <functional>
#include <mutex>
using namespace std;
// using namespace std::placeholders;
class Application
{
    mutex m_mutex;
    condition_variable m_condVar;
    bool m_bDataLoaded;
public:
    Application()
    {
        m_bDataLoaded = false;
    }
    void loadData()
    {
        // Make This Thread sleep for 1 second
        this_thread::sleep_for(chrono::microseconds(1000));
        cout << "Loadiing Data from XML" << endl;
        // Lock the data structure
        lock_guard<mutex> guard(m_mutex);
        // set the flag to true, means data is loaded
        m_bDataLoaded = true;
        // Notify the condition variable
        m_condVar.notify_one();
    }
    bool isDataLoaded()
    {
        return m_bDataLoaded;
    }
    void mainTask()
    {
        cout << "Do some Handshaking " << endl;
        //Acquire the lock
        unique_lock<mutex> mlock(m_mutex);
        //start waiting for the condition variable to get  signaled 
        // wait() will internally  release the lock and make the threaqd to block
        // As soon as condition variable get signaled, resume the thread and 
        // again acquire the lock. Then check if condition is met or not
        // If condition is met then continue else again go in wait.
        m_condVar.wait(mlock, bind(&Application::isDataLoaded, this));
        cout << "Do Processing On Loaded Data" << endl;
    }
};
int main()
{
    Application app;
    thread thread_1(&Application::mainTask, &app);
    thread thread_2(&Application::loadData, &app);
    thread_2.join();
    thread_1.join();

    std::cout << "Hello World!\n";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
