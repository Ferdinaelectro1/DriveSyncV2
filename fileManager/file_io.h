#ifndef __FILE_IO_H__
#define __FILE_IO_H__

#include <unistd.h>
#include <filesystem>

namespace fs = std::filesystem;

class FileIO {

    public:
       FileIO(const char *dirpath = ".");
       bool getDirpath(char* buffer,size_t bufferLen);
       bool setDirPath(const char * dirpath);
       size_t getFilesNames(char ** buffer, size_t maxBufferNameLen, size_t maxNameLen);
       size_t getDirsNames(char ** buffer, size_t maxBufferNameLen, size_t maxNameLen);
       size_t getDirsAndFilesNames(char ** buffer, size_t maxBufferNameLen, size_t maxNameLen);

    private: 
       const char *_dirpath;
       fs::path _dir;
};

#endif