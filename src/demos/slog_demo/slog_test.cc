#include <nsky/common/slog.h>
#include <iostream>
#include <string>

using namespace slog;

int main()
{

//	std::string a = "slog_test.log";
	std::map<std::string,std::string> conf = {{"logger_id","nsky"},{"logname","nsky"}};
	slog::InitLog(conf);
//	SPDLOG_INFO("hello world");
	SPDLOG_ERROR("hello world {}" ,1);

//	slog::InitLog("slog_test.log");
	return 0;
}
