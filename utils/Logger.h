#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <iostream>

#ifndef DV_DEBUG_MODE
#define DV_DEBUG_MODE
#endif

enum class LogLevel{
    WARNING,
    ERROR,
    INFO
};

class Logger {
    
    public: 
      #ifdef DV_DEBUG_MODE
        Logger(const char * fileName);
        Logger() = delete;
      #else 
        Logger();
      #endif
      //Logger(const char * log_path);
      int log(LogLevel logLevel,const char *msg);

    private : 
      Logger(const Logger& ) = delete;
      Logger operator=(const Logger&) = delete;

    private:
      const char * log_file_path ;
      #ifdef DV_DEBUG_MODE
        const char *_fileWhoLogName;
      #endif
      FILE *log_file = nullptr;
};

#endif