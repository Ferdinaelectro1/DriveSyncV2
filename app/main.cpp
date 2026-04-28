#include "../sync/SyncManager.h"
#include <csignal>
#include <atomic>

std::atomic<bool> running(true);

void signalHandler(int signal) {
    write(1, "Signal reçu!\n", 13); // write() est async-signal-safe
    running.store(false);
}

int main()
{
    std::signal(SIGINT, signalHandler);
    
    SyncManager syncManager(nullptr,nullptr,nullptr);
    syncManager.startSync();
    
    while(running.load()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    syncManager.stopSync();
    return 0;
}