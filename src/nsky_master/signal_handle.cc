//
// Created by zsy on 9/29/16.
//

#include <nsky/signal_handle.h>
using namespace std;

BaseServiceProcess::BaseServiceProcess()//:signalRunning_(false)
{
    // TODO
    installSignalHandle();
}

/*
	install signalHandle
	tip:
	pthread_sigmask(int how, const sigset_t *set, sigset_t *oset)
	1) how :SIG_BLOCK:结果集是当前集合参数集的并集；
	   SIG_UNBLOCK:结果集是当前集合参数集的差集;
	   SIG_SETMASK:结果集是由参数集指向的集
	2) notes 每个线程均有自己的信号屏蔽集(掩码),使用pthread_sigmask函数来屏蔽某个线程对某些信号的
		的响应处理,仅留下需要处理该信号的线程来处理指定的信号.实现方式是:利用线程信号屏蔽集的继承关系
		(在主线程中对sigmask进行设置后,主进程创建出来的线程将继承主线程的掩码)
*/
void BaseServiceProcess::installSignalHandle()
{
    // TODO
    sigset_t bset, oset;
    sigemptyset(&bset);
    sigaddset(&bset, SIGHUP);
    sigaddset(&bset, SIGINT);
    sigaddset(&bset, SIGTERM);
    sigaddset(&bset, 60);
    sigaddset(&bset, 61);

    if (pthread_sigmask(SIG_BLOCK, &bset, &oset) != 0)
    {
        // impossible here, TODO: exit or logging.
    }

    // start signal handler thread
    signalRunning_ = true;
    signalThread_ = std::thread(std::bind(&BaseServiceProcess::signalProc, this));
    signalThread_.detach();
}


void BaseServiceProcess::signalHandle(int sig)
{
    static bool profilerStarted = false;
    if (SIGKILL == sig || SIGINT == sig || SIGTERM == sig)
    {
        if (SIG_ERR == ::signal(sig,NULL))
        {
            // noting, not do log
        }
    }
    else if (60 == sig)
    {
        if (profilerStarted)
        {
            ProfilerFlush();
        }
        else
        {
            profilerStarted = true;
            ProfilerStart("/tmp/nros.profile");
        }
    }
    else if (61 == sig)
    {
        ProfilerStop();
        profilerStarted = false;
    }
}

// the special thread to handle singal
void BaseServiceProcess::signalProc()
{
    sigset_t waitset;
    siginfo_t info;
    int rc;
    sigemptyset(&waitset);
    sigaddset(&waitset, SIGINT);
    sigaddset(&waitset, SIGTERM);
    sigaddset(&waitset, 60);
    sigaddset(&waitset, 61);
    while(signalRunning_) {
        struct timespec tm = {1, 0};
        rc = sigtimedwait(&waitset, &info, &tm);
        if (rc > 0) {
            signalHandle(info.si_signo);
        } else if (0 == rc) {
            // timeout
        } else {
            // TODO impossible here, add some log
         //   signalRunning_ = false;
        }

    }
}

