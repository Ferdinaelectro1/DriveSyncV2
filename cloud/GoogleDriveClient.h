#ifndef __GOOGLE_DRIVE_CLIENT_H__
#define __GOOGLE_DRIVE_CLIENT_H__
#include <curl/curl.h>
#include <string>
#include <fstream>

class Logger;

class CloudIO {
    public: 
      CloudIO();
      bool createFileToDrive(const std::string & fileName);
      bool createDirToDrive(const std::string & dirName);
      ~CloudIO();

    private:
      bool createToDrive(const std::string & name,bool isFile);

    private:
      CURL *_curl;
      CURLcode _res;
      Logger *_logger;
};

#endif