cmake_minimum_required(VERSION 3.2)

project(driveSync)

set(SRC
    ../app/main.cpp
    ../sync/SyncManager.cpp
    ../sync/SyncManager.h
    ../utils/Logger.h
    ../utils/Logger.cpp
    ../watcher/FolderWatcher.h
    ../watcher/FolderWatcher.cpp
)

add_executable(
    ${PROJECT_NAME}
    ${SRC}
)

