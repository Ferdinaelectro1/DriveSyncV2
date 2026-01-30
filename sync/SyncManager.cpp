#include "SyncManager.h"
#include <iostream>
#include "../utils/Logger.h"

SyncManager::SyncManager(Watcher* watcher,FileIO* file_io,CloudIO* cloud_io)
{
    _logger = new Logger("SyncManager.cpp");
    if(!watcher || !file_io || !cloud_io) {
        _logger->log(LogLevel::ERROR,"Watcher or FileIO or CloudIO, don't be nullptr.");
        delete _logger;
        exit(1);
    }
    _file_io = file_io;
    _cloud_io = cloud_io;
    _watcher = watcher;
}

SyncManager::~SyncManager() {
    delete _logger;
}