#include "Logger.h"

int main()
{
    Logger logger;
    logger.log(LogLevel::WARNING,"This platform is not supported");
    logger.log(LogLevel::WARNING,"This can remove your disk");
    logger.log(LogLevel::ERROR,"Access is dinied");
    logger.log(LogLevel::INFO,"This server run to port 3490");
}