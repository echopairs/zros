/*********************************************************************
  Copyright (C), 1990-2016, HangZhou RED HEAT Tech. Co., Ltd.
  FileName: slog.h
  Author  : pairs & 736418319@qq.com
  Version : 1.0
  Date    : 2016/11/11
  Description:
      the interfaces to control log
  Function List:
  History:
      <author>    <time>    <version>    <desc>
        pairs     16/11/11      1.0     build this moudle
*********************************************************************/

#ifndef __SLOG_H__
#define __SLOG_H__

#include <map>
#include <chrono>
#include <nsky/common/slog/slogger.h>
#include <nsky/common/singleton.h>

#include <nsky/common/slog/slog_impl.h>

namespace slog {
// init log
void InitLog(const std::map<std::string,std::string>&);
void InitLog(std::string filename);
}



/*
#define INFO 			SPDLOG_INFO("")
#define ERROR 			SPDLOG_ERROR("")
#define DEBUG 			SPDLOG_DEBUG("")
#define WARN 			SPDLOG_WARN("")
#define CRITICAL 		SPDLOG_CRITICAL("")
*/
// macre to log
#define SPDLOG_INFO(...) 		SingleTon<Slogger>::Instance()->GetSpdloger()->info("[" __FILE__ "line #" SPDLOG_STR_HELPER(__LINE__) "] "  __VA_ARGS__)
#define SPDLOG_ERROR(...) 		SingleTon<Slogger>::Instance()->GetSpdloger()->error("[" __FILE__ "line #" SPDLOG_STR_HELPER(__LINE__) "] " __VA_ARGS__)
#define SPDLOG_DEBUG(...) 		SingleTon<Slogger>::Instance()->GetSpdloger()->debug("[" __FILE__ "line #" SPDLOG_STR_HELPER(__LINE__) "] " __VA_ARGS__)
#define SPDLOG_WARN(...) 		SingleTon<Slogger>::Instance()->GetSpdloger()->warn("[" __FILE__ "line #" SPDLOG_STR_HELPER(__LINE__) "] " __VA_ARGS__)
#define SPDLOG_CRITICAL(...) 	SingleTon<Slogger>::Instance()->GetSpdloger()->CRITICAL("[" __FILE__ "line #" SPDLOG_STR_HELPER(__LINE__) "] " __VA_ARGS__)


#endif
