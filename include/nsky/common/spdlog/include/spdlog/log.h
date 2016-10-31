#ifndef __LOG_H__
#define __LOG_H__
#include "spdlog.h"

#define SPLLOG_INFO(logger,...)       logger->info("[" __FILE__ " line #" SPDLOG_STR_HELPER(__LINE__) "] " __VA_ARGS__)
#define SPLLOG_DEBUG(logger,...)      logger->debug("[" __FILE__ " line #" SPDLOG_STR_HELPER(__LINE__) "] " __VA_ARGS__)
#define SPLLOG_WARN(logger,...)       logger->warn("[" __FILE__ " line #" SPDLOG_STR_HELPER(__LINE__) "] " __VA_ARGS__)
#define SPLLOG_CRITICAL(logger,...)   logger->critical("[" __FILE__ " line #" SPDLOG_STR_HELPER(__LINE__) "] " __VA_ARGS__)
#define SPLLOG_ERROR(logger,...)      logger->error("[" __FILE__ " line #" SPDLOG_STR_HELPER(__LINE__) "] " __VA_ARGS__)

#endif
