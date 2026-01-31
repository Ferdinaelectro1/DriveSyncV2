#include "FolderWatcher.h"
#include <sys/inotify.h>
#include <unistd.h>
#include "../utils/Logger.h"

Watcher::Watcher(const char *folderPath)
{
    _folderPath = folderPath;
    _logger = new Logger("FolderWatcher.cpp");
    _run.store(false);
}

Watcher::~Watcher(){
    delete _logger;
}

void Watcher::startWatching()
{
    _logger->log(LogLevel::INFO,"Init watching");
    _fd = inotify_init();
   if(_fd < 0) {
      _logger->log(LogLevel::ERROR,"Inotify Init Failed");
      delete _logger;
      exit(1);
   };   

    _wd = inotify_add_watch(_fd, ".", IN_CREATE | IN_DELETE);
    if (_wd < 0) {
      _logger->log(LogLevel::ERROR,"Inotify Add Watch Failed");
      delete _logger;
      exit(1);
    }
    _run.store(true);
}

void Watcher::stopWatching()
{
    _logger->log(LogLevel::WARNING,"Watching are stopped");
    _run.store(false);
    inotify_rm_watch(_fd, _wd);
    close(_fd);
}

//Cette fonction bloque jusqu'à ce qu'elle retourne un évèment
FileEvent Watcher::getNewEvent()
{
   FileEvent f_event;
   _logger->log(LogLevel::INFO,"Init get Event");
    while (_run.load()) {
        char buffer[1024];
        int length = read(_fd, buffer, sizeof(buffer));
        int i = 0;
        while (i < length) {
            auto* event = (struct inotify_event*)&buffer[i];
            char logMsgBuffer[512];
            if (event->mask & IN_CREATE) {
                snprintf(logMsgBuffer,sizeof(logMsgBuffer),"Fichier créé : %s",event->name);
                const char *logMsg = logMsgBuffer;
                _logger->log(LogLevel::INFO,logMsg);
                f_event.type = FileEventType::CREATE;
                f_event.file_name = event->name;
            }
            if (event->mask & IN_DELETE) {
                snprintf(logMsgBuffer,sizeof(logMsgBuffer),"Fichier supprimé : %s",event->name);
                const char *logMsg = logMsgBuffer;
                _logger->log(LogLevel::INFO,logMsg);
                f_event.type = FileEventType::DELETE;
                f_event.file_name = event->name;
            }
            i += sizeof(struct inotify_event) + event->len;
            return f_event;
        }
    }
    _run.store(false);
    f_event.type = FileEventType::NOEVENT;
    return f_event;
}