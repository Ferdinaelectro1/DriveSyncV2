#include  "Logger.h"
#include <cstring>

enum class Color
{
    RED = 31,
    GREEN = 32,
    YELLOW = 33,
    MAGENTA = 35,
    CYAN = 36
};

// Width for log level strings to ensure alignment
static const int LOG_LEVEL_WIDTH = 7;

//Attention à cette fonction , utiliser la donnée qu'elle retourne imédiatement avant un autre appel ,
//sinon risque que le second appel de la fonction change le contenu du buffer , vu que ce n'est qu'un pointeur
//qui est retourné
static const char * colorStr(const char * str,Color color)
{
    static char buffer[256];
    if(strlen(str) + strlen("\033[m\033[0m") + sizeof(int) >= sizeof(buffer))
    {
        std::cerr << "[ERROR] This chaine : "<<str<<" to convert is too long"<< std::endl;
        exit(1);
    }
    snprintf(buffer,sizeof(buffer),"\033[%dm%s\033[0m",static_cast<int>(color),str);
    const char *result = buffer;
    return result;
}

// Helper function to pad log level strings to a fixed width
static const char * padLogLevel(const char * str)
{
    static char buffer[32];
    int len = strlen(str);
    if(len >= LOG_LEVEL_WIDTH) {
        return str;
    }
    int padding = LOG_LEVEL_WIDTH - len;
    snprintf(buffer, sizeof(buffer), "%s%*s", str, padding, "");
    return buffer;
}

#ifndef DV_DEBUG_MODE
Logger::Logger() {
    log_file_path = std::getenv("DSFILE");
    if(log_file_path) {
        log_file = fopen(log_file_path,"a");
    }
    else {
        std::cerr << "[WARNING] Aucun chemin de fichier pour logger le processus"<<std::endl;
    }
}
#endif

#ifdef DV_DEBUG_MODE
Logger::Logger(const char * fileName)
{
    if(!fileName){
        std::cerr << "[WARNING] Log in debug : No fileName are specid"<<std::endl;
        _fileWhoLogName = "UnknowFile";
    } 
    else _fileWhoLogName = fileName;

    log_file_path = std::getenv("DSFILE");
    if(log_file_path) {
        log_file = fopen(log_file_path,"a");
    }
    else {
        std::cerr << "[WARNING] Aucun chemin de fichier pour logger le processus"<<std::endl;
    }
}
#endif

int Logger::log(LogLevel logLevel,const char *msg) 
{
    const char *logLevelStr;
    const char *noColorlogLevelStr;
    switch (logLevel)
    {
        case LogLevel::INFO :
            logLevelStr = colorStr(padLogLevel("INFO"),Color::GREEN);
            noColorlogLevelStr = padLogLevel("INFO");
            break;
        case LogLevel::WARNING :
            logLevelStr = colorStr(padLogLevel("WARNING"),Color::YELLOW);
            noColorlogLevelStr = padLogLevel("WARNING");
            break;
        case LogLevel::ERROR :
            logLevelStr = colorStr(padLogLevel("ERROR"),Color::RED);
            noColorlogLevelStr = padLogLevel("ERROR");
            break;
        default:
            std::cerr << "Niveau de logging invalide" << std::endl;
            return 1;
            break;
    }

    if(log_file)
    {
        #ifdef DV_DEBUG_MODE
           fprintf(log_file,"(%s)[%s] %s\n",_fileWhoLogName,noColorlogLevelStr,msg);
        #else
           fprintf(log_file,"[%s] %s\n",noColorlogLevelStr,msg);
        #endif
        fflush(log_file);
    }
    else {
        fprintf(stderr,"[WARNING] Your logger file '%s' is not open\n",log_file_path);
    }
    #ifdef DV_DEBUG_MODE
    fprintf(stderr,"(%s)[%s] %s\n",_fileWhoLogName,logLevelStr,msg);
    #else
    fprintf(stderr,"[%s] %s\n",logLevelStr,msg);
    #endif
    return 0;
}
