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
   _eventRun.store(true);
    _file_io = file_io;
}

SyncManager::~SyncManager() {
    delete _watcher;
    delete _cloud_io;
    delete _logger;
    _eventRun.store(false);
    if(_getEventThread.joinable()) 
       _getEventThread.join();
}

void SyncManager::startSync() {
    _eventRun.store(true);
    _logger->log(LogLevel::INFO,"SyncManager Start Successful");
    _watcher->startWatching();   
    _getEventThread = std::thread(&SyncManager::eventHandle,this);
    std::this_thread::sleep_for(std::chrono::seconds(180));
    stopSync();
}

void SyncManager::stopSync(){
    _eventRun.store(false);
    _watcher->stopWatching();
    _logger->log(LogLevel::WARNING,"SyncManager func start are stopped");
}

void SyncManager::eventHandle()
{
    while (_eventRun.load()) {
        FileEvent f_event =  _watcher->getNewEvent(); //ceci est blocant
        if(f_event.event_type == FileEventType::NOEVENT) {
            _logger->log(LogLevel::WARNING,"No event are detected");
            continue;
        }
        if(f_event.event_type == FileEventType::CREATE){
            if(f_event.file_type == FileType::FILE) {
              _cloud_io->createFileToDrive(f_event.getName());
              _logger->log(LogLevel::INFO,"Great on file are created");
            }
            else if(f_event.file_type == FileType::DIR) {
              _cloud_io->createDirToDrive(f_event.getName());
              _logger->log(LogLevel::INFO,"Great on Dir are created");
            }
        }
        if(f_event.event_type == FileEventType::DELETE)
        {
            _logger->log(LogLevel::INFO,"Great on file are removed"); 
        }
    }
}