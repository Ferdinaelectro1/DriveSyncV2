#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <iostream>

enum class LogLevel{
    WARNING,
    ERROR,
    INFO
};

class Logger {
    
    public: 
      Logger();
      #ifdef DV_DEBUG_MODE
        Logger(const char * fileName,const char* id);
      #endif
      Logger(const char * log_path);
      int log(LogLevel logLevel,const char *msg);

    private : 
      Logger(const Logger& ) = delete;
      Logger operator=(const Logger&) = delete;

    private:
      const char * log_file_path ;
      FILE *log_file = nullptr;
};

#endif