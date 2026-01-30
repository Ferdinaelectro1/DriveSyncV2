#ifndef __SYNC_MANAGER_H__
#define __SYNC_MANAGER_H__ 

class FileIO;
class CloudIO;
class Watcher;

class SyncManager {

    public:
      SyncManager(Watcher* watcher,FileIO* file_io,CloudIO* cloud_io);
      void startSync(int syncIntervalle);
      void stopSync();

    private:
      FileIO *_file_io;
      CloudIO *_cloud_io;
      Watcher *_watcher;
};

#endif