#ifndef __FOLDER_WATCHER_H__
#define __FOLDER_WATCHER_H__

#include <atomic>
#include <cstring>
#include "../config/Config.h"

class Logger;

enum class FileEventType{
    CREATE,
    DELETE,
    NOEVENT
};

enum class FileType {
    DIR,
    FILE,
    UNKNOW
};

static inline void copyString(char* dest, size_t dest_size, const char* src) {
    if (!dest || dest_size == 0 || !src) return;
    
    size_t i = 0;
    while (i < dest_size - 1 && src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

template<size_t N>
struct FileEventBase
{
    public:
      FileEventType event_type = FileEventType::NOEVENT;
      FileType file_type = FileType::UNKNOW;
    private:
      char file_name[N] = {0};
    public:
      void setName(const char * name){
        copyString(file_name,N,name);
      }
      const char *getName() {return file_name;}
};

using FileEvent = FileEventBase<dsConfig::MAX_FILENAME_LEN>;

class Watcher {

    public: 
       Watcher(const char *folderPath = ".");
       ~Watcher();
       void startWatching();
       void stopWatching();
       FileEvent getNewEvent();

    private:
       const char *_folderPath;
       Logger *_logger;
       int _fd;
       int _wd;
       std::atomic<bool> _run;
};


#endif