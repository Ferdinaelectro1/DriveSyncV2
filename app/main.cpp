#include "../sync/SyncManager.h"
#include <csignal>
#include <atomic>
#include <thread>

int stopFd; //le fd permettant d'arrêter le watcher car le watcher est blocant

void signalHandler(int) {
    write(stopFd,"stop",4); //on écrit sur le pipe pour arrêter dans le select
}

int main()
{
    std::signal(SIGINT, signalHandler);
    
    SyncManager syncManager;
    stopFd = syncManager.getStopfd();
    syncManager.startSync();
    syncManager.wait();
    return 0;
}