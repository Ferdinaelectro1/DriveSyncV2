#include "SyncManager.h"
#include <iostream>
#include <thread>
#include <chrono>
#include "../utils/Logger.h"
#include "../watcher/FolderWatcher.h"
#include "../cloud/GoogleDriveClient.h"

SyncManager::SyncManager(Watcher* watcher,FileIO* file_io,CloudIO* cloud_io)
{
    _logger = new Logger("SyncManager.cpp");
   _watcher = new Watcher();
   _cloud_io = new CloudIO();
    _file_io = file_io;
}

SyncManager::~SyncManager() {
    delete _watcher;
    delete _cloud_io;
    //_logger->log(LogLevel::WARNING,"Delete are called");
    delete _logger;
}

void SyncManager::startSync() {
    _logger->log(LogLevel::INFO,"SyncManager Start Successful");
    _watcher->startWatching();   
    std::thread(&SyncManager::eventHandle,this).detach();
    std::this_thread::sleep_for(std::chrono::seconds(180));
    _watcher->stopWatching();
    _logger->log(LogLevel::WARNING,"SyncManager func start are stopped");
}

void SyncManager::eventHandle()
{
    while (1) {
        FileEvent f_event =  _watcher->getNewEvent(); //ceci est blocant
        if(f_event.event_type == FileEventType::NOEVENT) {
            _logger->log(LogLevel::ERROR,"No event are detected");
            exit(1);
        }
        if(f_event.event_type == FileEventType::CREATE){
            _logger->log(LogLevel::INFO,"Great on file are created");
            if(f_event.file_type == FileType::FILE) {
              _cloud_io->sendToDrive(f_event.getName());
            }
        }
        if(f_event.event_type == FileEventType::DELETE)
        {
            _logger->log(LogLevel::INFO,"Great on file are removed"); 
        }
    }
}