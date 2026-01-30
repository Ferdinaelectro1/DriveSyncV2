#include "SyncManager.h"
#include <iostream>

SyncManager::SyncManager(Watcher* watcher,FileIO* file_io,CloudIO* cloud_io)
{
    if(!watcher || !file_io || !cloud_io) {
        exit(1);
    }
    _file_io = file_io;
    _cloud_io = cloud_io;
    _watcher = watcher;
}