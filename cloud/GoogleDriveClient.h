#ifndef __GOOGLE_DRIVE_CLIENT_H__
#define __GOOGLE_DRIVE_CLIENT_H__
#include <curl/curl.h>
#include <string>
#include <fstream>

class Logger;
class LocalSettings;

class CloudIO {
    public: 
      CloudIO();
      bool createFileToDrive(const std::string & fileName);
      bool createDirToDrive(const std::string & dirName);
      bool deleteFileFromDrive(const std::string & elementName);
      ~CloudIO();

    private:
      std::string createToDrive(const std::string & name,bool isFile);
      bool regenaratetoken();

    private:
      Logger *_logger;
      LocalSettings *_localSettings;
};

#endif