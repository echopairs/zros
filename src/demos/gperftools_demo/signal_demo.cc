#include <nsky/common/signal_handle.h>
#include <iostream>


using namespace std;

// for test

void fun1()
{
	char *fun1 = new char(100);
	delete fun1;
	fun1 = NULL;
}

void fun2()
{
    char *fun2 = new char(100);
    delete fun2;
    fun2 = NULL;
}

void fun3()
{
    char *fun3 = new char(100);
    delete fun3;
    fun3 = NULL;
}

// main

int main()
{
	BaseServiceProcess a;
    a.installSignalHandle();
    cout << "hello world" << endl;
    std::this_thread::sleep_for(chrono::seconds(1));
    while(1)
    {
        for (int i = 0; i < 10; i++)
        {
            fun1();
        }
        fun2();
        fun2();
        fun3();
        fun3();
        fun3();
        fun3();
        fun3();
    }

}



