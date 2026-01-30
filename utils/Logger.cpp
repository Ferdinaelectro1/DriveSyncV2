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

Logger::Logger() {
    log_file_path = std::getenv("DSFILE");
    if(log_file_path) {
        log_file = fopen(log_file_path,"a");
    }
    else {
        std::cerr << "[WARNING] Aucun chemin de fichier pour logger le processus"<<std::endl;
    }
}

#ifdef DV_DEBUG_MODE
Logger::Logger(const char * fileName,const char* id)
{
    
}
#endif

Logger::Logger(const char * log_path)
{
    if(!setenv("DSFILE",log_path,0)) {
        Logger();
    }
    else {
        std::cerr <<"[WARNING] La variable d'environnement DSFILE n'a pas pû être modifié"<<std::endl;
    }
}

int Logger::log(LogLevel logLevel,const char *msg) 
{
    const char *logLevelStr;
    const char *noColorlogLevelStr;
    switch (logLevel)
    {
        case LogLevel::INFO :
            logLevelStr = colorStr("INFO",Color::GREEN);
            noColorlogLevelStr = "INFO";
            break;
        case LogLevel::WARNING :
            logLevelStr = colorStr("WARNING",Color::YELLOW);
            noColorlogLevelStr = "WARNING";
            break;
        case LogLevel::ERROR :
            logLevelStr = colorStr("ERROR",Color::RED);
            noColorlogLevelStr = "ERROR";
            break;
        default:
            std::cerr << "Niveau de logging invalide" << std::endl;
            return 1;
            break;
    }

    if(log_file)
    {
        fprintf(log_file,"[%s] %s\n",noColorlogLevelStr,msg);
        fflush(log_file);
    }
    else {
        fprintf(stderr,"[WARNING] Your logger file '%s' is not open\n",log_file_path);
    }
    fprintf(stderr,"[%s] %s\n",logLevelStr,msg);
    return 0;
}
