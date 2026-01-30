#include <iostream>
#include "file_io.h"

int main()
{
    FileIO file_io;
    char buffer[256];
    file_io.getDirpath(buffer,sizeof(buffer));
    const char *dirPathName = buffer;
    std::cout << "Dossier de recherche : "<< dirPathName << std::endl;
    
    char files[10][256];
    char* FilesBuffer[10];
    for(int i = 0; i < 10; i++)
    {
        FilesBuffer[i] = files[i];
    }

    size_t TotalFiles  = file_io.getFilesNames(FilesBuffer,10,256);
    for(int i = 0; i < TotalFiles; i++)
    {
        std::cout << "File "<<i+1<<" : "<<FilesBuffer[i]<<std::endl;
    }
    std::cout << std::endl;

    char dirs[4][100];
    char *dirsPtr[4];
    for(int i = 0; i < 4; i++) dirsPtr[i] = dirs[i];
    size_t TotalDirs  = file_io.getDirsNames(dirsPtr,4,100);
    for(int i = 0; i < TotalDirs ; i++) std::cout << "Dir "<<i+1<<" : "<<dirsPtr[i]<<std::endl;
    std::cout << std::endl;

    char DirsAndFiles[10][100];
    char *DirsAndFilesPtr[10];
    for(int i = 0; i < 10; i++) DirsAndFilesPtr[i] = DirsAndFiles[i];
    size_t TotalesDirsAndFiles = file_io.getDirsAndFilesNames(DirsAndFilesPtr,10,100);
    for(int i =  0; i< TotalesDirsAndFiles; i++) std::cout <<"Dirs/Files "<<i<<" : "<<DirsAndFilesPtr[i] << std::endl;

    return 0;
}