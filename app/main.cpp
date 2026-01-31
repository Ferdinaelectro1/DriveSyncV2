#include "../sync/SyncManager.h"
#include "../watcher/FolderWatcher.h"

int main()
{
    SyncManager syncManager(nullptr,nullptr,nullptr);
    syncManager.startSync();
    return 0;
}