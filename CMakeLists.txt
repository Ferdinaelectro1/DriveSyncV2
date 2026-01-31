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
    ../cloud/GoogleDriveClient.h
    ../cloud/GoogleDriveClient.cpp
    ../cloud/key.h
)

add_executable(
    ${PROJECT_NAME}
    ${SRC}
)

target_link_libraries(${PROJECT_NAME} PRIVATE curl)