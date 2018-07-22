//
// Created by zsy on 9/29/16.
//

/*********************************************************************
  Copyright (C), 1990-2016, HangZhou RED HEAT Tech. Co., Ltd.
  FileName: sig.cc
  Author  : pairs & 736418319@qq.com
  Version : 1.0
  Date    : 9/29/16
  Description:
        how to deal with signal in Multi-threading and
        getperformance bottleneck with gperftools
  Function List:
  History:
      <author>    <time>    <version>    <desc>
        pairs     16/05/05      1.0     build this moudle
*********************************************************************/

#pragma once


#include <iostream>
#include <signal.h>
#include <gperftools/heap-checker.h>
#include <gperftools/heap-profiler.h> // for heap with HeapProfilerStart,HeapProfileStop,HeapProfileDump
#include <gperftools/profiler.h>      // for cpu with ProfileStart,ProfileStop,ProfilerFlush
#include <thread>                 // c11 for multi-thread

namespace zros {

    class BaseServiceProcess
    {
    public:
        BaseServiceProcess();
        void installSignalHandle();	// init interface
        void wait() {signal_thread_.join();}
    private:
        void signalHandle(int sig);
        void signalProc();				// signal handle function
        std::thread signal_thread_;		// the thread for deal with sig
        bool signal_running_;
    };
}



