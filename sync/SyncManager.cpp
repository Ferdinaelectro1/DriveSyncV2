#include "SyncManager.h"
#include <iostream>
#include "../utils/Logger.h"
#include "../watcher/FolderWatcher.h"
#include <thread>
#include <chrono>

SyncManager::SyncManager(Watcher* watcher,FileIO* file_io,CloudIO* cloud_io)
{
    _logger = new Logger("SyncManager.cpp");
   _watcher = new Watcher();
    _file_io = file_io;
    _cloud_io = cloud_io;
}

SyncManager::~SyncManager() {
    delete _logger;
    delete _watcher;
}

void SyncManager::startSync() {
    _logger->log(LogLevel::INFO,"SyncManager Start Successful");
    _watcher->startWatching();   
    std::thread(&SyncManager::eventHandle,this).detach();
    std::this_thread::sleep_for(std::chrono::seconds(20));
    _watcher->stopWatching();
}

void SyncManager::eventHandle()
{
    while (1) {
        FileEvent f_event =  _watcher->getNewEvent(); //ceci est blocant
        if(f_event.type == FileEventType::NOEVENT) {
            _logger->log(LogLevel::ERROR,"No event are detected");
            exit(1);
        }
        if(f_event.type == FileEventType::CREATE){
            _logger->log(LogLevel::INFO,"Great on file are created");
        }
        if(f_event.type == FileEventType::DELETE)
        {
            _logger->log(LogLevel::INFO,"Great on file are removed"); 
        }
    }
}