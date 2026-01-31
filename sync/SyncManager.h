#ifndef __SYNC_MANAGER_H__
#define __SYNC_MANAGER_H__ 
#include <thread>
#include <atomic>

class FileIO;
class CloudIO;
class Watcher;
class Logger;

class SyncManager {

    public:
      SyncManager(Watcher* watcher,FileIO* file_io,CloudIO* cloud_io);
      void startSync();
      void stopSync();
      ~SyncManager();

    private:
      void eventHandle();
      
    private:
      FileIO *_file_io;
      CloudIO *_cloud_io;
      Watcher *_watcher;
      Logger *_logger;
      std::thread _getEventThread;
      std::atomic<bool> _eventRun;
};

#endif