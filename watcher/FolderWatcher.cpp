#include "FolderWatcher.h"
#include <sys/inotify.h>
#include <unistd.h>
#include "../utils/Logger.h"


Watcher::Watcher(const char *folderPath)
{
    _folderPath = folderPath;
    _logger = new Logger("FolderWatcher.cpp");
    _run.store(false);
    pipe(_pipe_fd);
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
   f_event.event_type = FileEventType::NOEVENT;
   _logger->log(LogLevel::INFO,"Init get Event");
    while (_run.load()) {
        char buffer[1024];
        //_pipe_fd[0] le fd du tunel de lecture du pipe serait surveillé par select
        int maxFd = std::max(_fd,_pipe_fd[0]) + 1;
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(_fd,&fds);
        FD_SET(_pipe_fd[0],&fds);
        select(maxFd,&fds,nullptr,nullptr,nullptr);
        int length = -1;
        if(FD_ISSET(_fd,&fds)) { //le inotify a des données presents
            length = read(_fd, buffer, sizeof(buffer));   
        }
        if(FD_ISSET(_pipe_fd[0],&fds)) {//le pipe a reçu des données "stop"
            stopWatching();
        }
        while (length > 0) {
            auto* event = (struct inotify_event*)&buffer[0];
            char logMsgBuffer[512];
            if (event->mask & IN_CREATE) {
                if(event->mask & IN_ISDIR) {
                    //Dossier
                    snprintf(logMsgBuffer,sizeof(logMsgBuffer),"Dossier créé : %s",event->name);
                    f_event.file_type = FileType::DIR;
                }
                else {
                    //Fichier
                    snprintf(logMsgBuffer,sizeof(logMsgBuffer),"Fichier créé : %s",event->name);
                    f_event.file_type = FileType::FILE;
                }
                const char *logMsg = logMsgBuffer;
                _logger->log(LogLevel::INFO,logMsg);
                f_event.event_type = FileEventType::CREATE;
                f_event.setName(event->name);
            }
            if (event->mask & IN_DELETE) {
                if(event->mask & IN_ISDIR) {
                    //Dossier
                    snprintf(logMsgBuffer,sizeof(logMsgBuffer),"Dossier supprimé : %s",event->name);
                    f_event.file_type = FileType::DIR;
                }
                else {
                    //Fichier
                    snprintf(logMsgBuffer,sizeof(logMsgBuffer),"Fichier supprimé : %s",event->name);
                    f_event.file_type = FileType::FILE;
                }                
                const char *logMsg = logMsgBuffer;
                _logger->log(LogLevel::INFO,logMsg);
                f_event.event_type = FileEventType::DELETE;
                f_event.setName(event->name);
            }
            return f_event;
        }
    }
    f_event.event_type = FileEventType::STOP_EVENT;
    return f_event;
}

int Watcher::getStopfd()
{
    /**
     * On retourne ici le fd du pipe d'écriture pour pouvoir
     * utiliser le fd d'écriture dans le main pour 
     * écrire deçu quand le signal serait reçu afin de couper la 
     * boucle du watcher
     */
    return _pipe_fd[1];
}
