#ifndef __FOLDER_WATCHER_H__
#define __FOLDER_WATCHER_H__

#include <atomic>

class Logger;

enum FileEventType{
    CREATE,
    DELETE,
    NOEVENT
};
struct FileEvent
{
    FileEventType type = FileEventType::NOEVENT;
    const char *file_name;
};

class Watcher{

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