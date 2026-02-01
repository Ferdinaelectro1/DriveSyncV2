#include "GoogleDriveClient.h"
#include <string>
#include <vector>
#include "../utils/Logger.h"
#include "key.h"


CloudIO::CloudIO() {
    _logger = new Logger("GoogleDriveClient.cpp");
    curl_global_init(CURL_GLOBAL_ALL); //Initialisation global de curl
}

CloudIO::~CloudIO() {
    curl_global_cleanup(); //Fermetture global de curl
    delete _logger;
}

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
    size_t total = size * nmemb;
    s->append((char*)contents, total);
    return total;
}

bool CloudIO::createToDrive(const std::string & name,bool isFile) {
    bool success = true;
    const char *content_type;
    std::string metaData;
    const char *driveUrl;
    if(isFile)  {
        // Lecture du fichier
        std::ifstream file(name, std::ios::binary);
        if (!file) {
            _logger->log(LogLevel::ERROR,"Impossible d'ouvrir le fichier.");
            success = false;
            return success;
        }
        std::vector<char> buffer(
            (std::istreambuf_iterator<char>(file)),
            std::istreambuf_iterator<char>()
        );

        file.close();

        content_type = "Content-Type: multipart/related; boundary=boundary123";
        driveUrl = "https://www.googleapis.com/upload/drive/v3/files?uploadType=multipart";
        metaData =
            "--boundary123\r\n"
            "Content-Type: application/json; charset=UTF-8\r\n"
            "\r\n"
            "{ \"name\": \"" + name + "\", "
            "\"mimeType\": \"application/octet-stream\", "
            "\"parents\": [\"14NuNzh2ktZswGVtcmlb7dhFkOXcpF0SY\"] }\r\n"
            "--boundary123\r\n"
            "Content-Type: application/octet-stream\r\n"
            "\r\n";
        metaData.append(buffer.begin(),buffer.end());
        metaData.append("\r\n--boundary123--\r\n");
    } 
    //Dossier
    else {
        content_type = "Content-Type: application/json; charset=UTF-8";
        driveUrl = "https://www.googleapis.com/drive/v3/files";
        metaData = "{ \"name\": \""+name+"\", "
                    "\"mimeType\": \"application/vnd.google-apps.folder\""
                ", \"parents\": [\"14NuNzh2ktZswGVtcmlb7dhFkOXcpF0SY\"] }";
    }

    _curl = curl_easy_init();
    if(_curl) {
        struct curl_slist *headers = nullptr;
        headers = curl_slist_append(headers, ("Authorization: Bearer " + _access_token).c_str());
        headers = curl_slist_append(headers, content_type);

        curl_easy_setopt(_curl, CURLOPT_URL, driveUrl);
        curl_easy_setopt(_curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(_curl, CURLOPT_POSTFIELDS, metaData.data());
        curl_easy_setopt(_curl, CURLOPT_POSTFIELDSIZE,metaData.size());
        std::string response;
        curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &response);

        _res = curl_easy_perform(_curl);
        if(_res != CURLE_OK) {
            std::string msg = "Erreur curl: ";
            msg += curl_easy_strerror(_res);
            _logger->log(LogLevel::ERROR,msg.c_str());
            success = false;
        } else if(_res == CURLE_OK)
        {
           std::string msg = "Reponse Google Drive : "; 
           msg += response;
           _logger->log(LogLevel::INFO,msg.c_str());
           success = true;
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(_curl);
    }
    return success;
}

bool CloudIO::createFileToDrive(const std::string & fileName) {
    return createToDrive(fileName,true);
}

bool CloudIO::createDirToDrive(const std::string & dirName) {
    return createToDrive(dirName,false);
}