#include "file_io.h"
#include <iostream>
#include <cstring>

FileIO::FileIO(const char *dirpath){
    _dirpath = dirpath;
    _dir = _dirpath;
}

bool FileIO::getDirpath(char* buffer,size_t bufferSize)
{
    if(!_dirpath || (strlen(_dirpath) >= bufferSize))
       return false;
    snprintf(buffer,bufferSize,"%s",_dirpath);
    return true;   
}

bool FileIO::setDirPath(const char * dirpath){
    if(!dirpath)
       return false;
    _dirpath = dirpath;
    return true;
}
// [()()]  [()()]  [()()]  [()()]  [()()]
 
//Cette fonction retourne le nombres de fichiers présents dans le dossier .
//Les noms des fichiers ne sont que des char *
size_t FileIO::getFilesNames(char ** buffer, size_t maxBufferNameLen, size_t maxNameLen)
{
    size_t count = 0;
    for(const auto& entry : fs::directory_iterator(_dir))
    {   
        if (!entry.is_regular_file())
            continue;
        if (count >= maxBufferNameLen) 
           break;  // trop de fichiers        
        if(entry.is_regular_file()){
            snprintf(buffer[count],maxNameLen,"%s",entry.path().filename().c_str());
            count++;
        }
    }
    return count;
}

size_t FileIO::getDirsNames(char ** buffer, size_t maxBufferNameLen, size_t maxNameLen)
{
    size_t count = 0;
    for(const auto& entry : fs::directory_iterator(_dir))
    {   
        if (!entry.is_directory())
            continue;
        if (count >= maxBufferNameLen) 
           break;  // trop de fichiers        
        if(entry.is_directory()){
            snprintf(buffer[count],maxNameLen,"%s",entry.path().filename().c_str());
            count++;
        }
    }
    return count;
}

size_t FileIO::getDirsAndFilesNames(char ** buffer, size_t maxBufferNameLen, size_t maxNameLen)
{
    size_t count = 0;
    for(const auto& entry : fs::directory_iterator(_dir))
    {   
        if (count >= maxBufferNameLen) 
           break;  // trop de fichiers        

        snprintf(buffer[count],maxNameLen,"%s",entry.path().filename().c_str());
        count++;
    }
    return count;
}

