/*********************************************************************
  Copyright (C), 1990-2016, HangZhou RED HEAT Tech. Co., Ltd.
  FileName: slogger.h
  Author  : pairs & 736418319@qq.com
  Version : 1.0
  Date    : 2016/11/11
  Description:
      for spdlog logger wrap
  Function List:
  	1. Slogger(std::string id) 		// init Slogger,before init Slogger we need build spdlog logger with id
  	2. GetSpdloger(std::string id)  // get spdlogger ,now param id is not use
  History:
      <author>    <time>    <version>    <desc>
        pairs     16/11/13      1.0     build this moudle
*********************************************************************/
#ifndef __SLOGGER_H__
#define __SLOGGER_H__

#include <spdlog/spdlog.h>
#include <memory>
#include <string>

namespace slog
{
	class Slogger
	{
	public:
		Slogger(std::string id) :id_(id)
		{
			logger_ = spdlog::get(id_);
		}
		Slogger() = default;
		std::shared_ptr<spdlog::logger> GetSpdloger(std::string id = "default")
		{
			if (logger_ != nullptr)
			{
				return logger_;
			}
			else
			{
				logger_ = spdlog::stdout_color_mt("default");
				return logger_;

			}
		}
		~Slogger() {}
	private:
		std::string id_;
		std::shared_ptr<spdlog::logger> logger_;
	};

}  // end of slog

#endif
