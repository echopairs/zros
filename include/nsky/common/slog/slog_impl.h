/*********************************************************************
  Copyright (C), 1990-2016, HangZhou RED HEAT Tech. Co., Ltd.
  FileName: slog_impl.h
  Author  : pairs & 736418319@qq.com
  Version : 1.0
  Date    : 2016/11/11
  Description:
      the impl of slog's interface
  Function List:
  History:
      <author>    <time>    <version>    <desc>
        pairs     16/11/11      1.0     build this moudle
*********************************************************************/
#ifndef __SLOG_IMPL_H__
#define __SLOG_IMPL_H__

#include <map>
#include <string>
#include "../singleton.h"
#include "slogger.h"

namespace spd = spdlog;
namespace slog {
std::map<std::string, std::string> CONFIG_MAP_DEFAULT = {
	{"logger_id", "default"},
	{"logname","default"},
	{"async_mode","8192"},
	{"pattern","[%Y-%m-%d %H:%M:%S:%e] [%l] [%n] [thread %t] %v"},
	{"level","debug"},
	{"file_size","5"},
	{"rotate_num","3"},
	{"flush_on","error"},
	{"flush_time","1"}
};



std::string InitConfMap(std::map<std::string,std::string> conf)
{
	std::string logger_id = (conf["logger_id"] != "")? conf["logger_id"]:CONFIG_MAP_DEFAULT["logger_id"];
	std::string logname = (conf["logname"] != "")? conf["logname"]:CONFIG_MAP_DEFAULT["logname"];
	std::string pattern = (conf["pattern"] != "")? conf["pattern"]:CONFIG_MAP_DEFAULT["pattern"];
	std::string level = (conf["level"] != "")? conf["level"]:CONFIG_MAP_DEFAULT["level"];
	std::string flush_on = (conf["flush_on"] != "")? conf["flush_on"]:CONFIG_MAP_DEFAULT["flush_on"];
	std::string async_mode = (conf["async_mode"] != "")? conf["async_mode"]:CONFIG_MAP_DEFAULT["async_mode"];
	std::string file_size = (conf["file_size"] != "")? conf["file_size"]:CONFIG_MAP_DEFAULT["file_size"];
	std::string flush_time = (conf["flush_time"] != "")? conf["flush_time"]:CONFIG_MAP_DEFAULT["flush_time"];
	std::string rotate_num = (conf["rotate_num"] != "")? conf["rotate_num"]:CONFIG_MAP_DEFAULT["rotate_num"];

	// set pattern
	spd::set_pattern(pattern);

	// Runtime log levels
	if (level == "info")
	{
		spd::set_level(spd::level::info);
	}
	else if(level == "debug")
	{
		spd::set_level(spd::level::debug);
	}
	else if(level == "critical")
	{
		spd::set_level(spd::level::critical);
	}
	else if(level == "warning")
	{
		spd::set_level(spd::level::warn);
	}
	else
	{
		// error or the other
		spd::set_level(spd::level::err);
	}

	// asyn mode && and max flush time
	spdlog::set_async_mode(std::stoi(async_mode), spdlog::async_overflow_policy::block_retry,
                   nullptr,
                   std::chrono::seconds(std::stoi(flush_time)));

	// create the only global logger
	auto logger = spd::rotating_logger_mt(logger_id, logname, std::stoi(file_size)*1024*1024, std::stoi(rotate_num));

	// default when error must flush log
	logger->flush_on(spd::level::err);

	return logger_id;
}

void InitLog(std::string id)
{
	std::map<std::string,std::string> m_ = {{"logname",id},{"logger_id",id}};
	std::string logger_id = InitConfMap(m_);
	SingleTon<Slogger>::Instance(logger_id);
}

void InitLog(const std::map<std::string,std::string> &conf)
{
	// TODO
	if (!conf.size())
	{
		slog::InitLog("default");
		return ;
	}
	else
	{
		std::string logger_id = InitConfMap(conf);
		SingleTon<slog::Slogger>::Instance(logger_id);
	}

}


} // end of slog

#endif
