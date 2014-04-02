#ifndef LAF_LAF_LOG_H
#define LAF_LAF_LOG_H

#include <iostream>
#include <iomanip>

namespace LafLog
{
    enum {
	Debug = 2,
	Info  = 3,
	Warn  = 4,
	Error = 5,
	Fatal = 6
    };

    //2:debug, 3:info, 4:warn, 5:error, 6:fatal
    extern int  LogLevel;
}

#define LogDebug  if (LafLog::LogLevel <= LafLog::Debug ) \
    std::cout << std::setiosflags(std::ios::left) << std::setw(24) << (this->name()+'.'+__func__) << "  DEBUG: "

#define LogInfo  if (LafLog::LogLevel <= LafLog::Info ) \
    std::cout << std::setiosflags(std::ios::left) << std::setw(24) << (this->name()+'.'+__func__) << "   INFO: "

#define LogWarn  if (LafLog::LogLevel <= LafLog::Warn ) \
    std::cout << std::setiosflags(std::ios::left) << std::setw(24) << (this->name()+'.'+__func__) << "   WARN: "

#define LogError  if (LafLog::LogLevel <= LafLog::Error ) \
    std::cout << std::setiosflags(std::ios::left) << std::setw(24) << (this->name()+'.'+__func__) << "  ERROR: "

#define LogFatal  if (LafLog::LogLevel <= LafLog::Fatal ) \
    std::cout << std::setiosflags(std::ios::left) << std::setw(24) << (this->name()+'.'+__func__) << "  FATAL: "

#define LogAlways \
    std::cout << std::setiosflags(std::ios::left) << std::setw(24) << (this->name()+'.'+__func__) << " ALWAYS: "

#endif
