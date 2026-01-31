#ifndef __GOOGLE_DRIVE_CLIENT_H__
#define __GOOGLE_DRIVE_CLIENT_H__
#include <curl/curl.h>
#include <string>
#include <fstream>

class Logger;

class CloudIO {
    public: 
      CloudIO();
      bool sendToDrive(const std::string & fileName);
      ~CloudIO();

    private:
      CURL *_curl;
      CURLcode _res;
      Logger *_logger;
};

#endif