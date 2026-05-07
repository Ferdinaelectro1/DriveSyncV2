#include "GoogleDriveClient.h"
#include <string>
#include <vector>
#include <iostream>
#include <nlohmann/json.hpp>
#include "../fileManager/LocalSettings.h"
#include "../utils/Logger.h"
#include "../config/Config.h"

static std::string getInput(const std::string& msg, size_t requiredLen) {
    std::string currentInput = "";
    std::cout << msg;
    std::cin  >> currentInput;
    while (currentInput.size() < requiredLen)
    {
        std::cout <<"L'information saisi est incorrect"<<std::endl;
        std::cout << "Cette information contient au moins "<<requiredLen<<" caractère" << std::endl;
        std::cout << "Veuillez réessayer !" << std::endl;
        std::cout << msg;
        std::cin  >> currentInput;
    }
    return currentInput;
}

CloudIO::CloudIO() {
    _logger = new Logger("GoogleDriveClient.cpp");
    _localSettings = new LocalSettings();
    if(!_localSettings->isloadingRequiredSettings()) {
        std::cout << "Certains informations nécessaires n'ont pas été trouvé !"<<std::endl;
        std::cout << "Veuillez s'il vous plaît les renseigner"<< std::endl;
        _localSettings->setLocalConfig("client_secret",getInput("1. Votre Client Secret : ",35));
        _localSettings->setLocalConfig("client_id",    getInput("2. Votre Client Id : ",72));
        _localSettings->setLocalConfig("refresh_token",getInput("3. Votre Refresh Token : ",103));
    }
    auto access_token = _localSettings->getLocalConfig("access_token");
    if(access_token.has_value()) dsConfig::access_token   = _localSettings->getLocalConfig("access_token").value();
    dsConfig::client_secret  = _localSettings->getLocalConfig("client_secret").value();
    dsConfig::client_id      = _localSettings->getLocalConfig("client_id").value();
    dsConfig::refresh_token  = _localSettings->getLocalConfig("refresh_token").value();
    curl_global_init(CURL_GLOBAL_ALL); //Initialisation global de curl
}

CloudIO::~CloudIO() {
    curl_global_cleanup(); //Fermetture global de curl
    delete _logger;
    delete _localSettings;
}

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
    size_t total = size * nmemb;
    s->append((char*)contents, total);
    return total;
}

std::string CloudIO::createToDrive(const std::string & name,bool isFile) {
    CURL *_curl;
    CURLcode _res;
    const char *content_type;
    std::string metaData;
    std::string returnId = "";
    const char *driveUrl;
    long http_code;
    if(isFile)  {
        // Lecture du fichier
        std::ifstream file(name, std::ios::binary);
        if (!file) {
            _logger->log(LogLevel::ERROR,"Impossible d'ouvrir le fichier.");
            return returnId;
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
        headers = curl_slist_append(headers, ("Authorization: Bearer " + dsConfig::access_token).c_str());
        headers = curl_slist_append(headers, content_type);

        curl_easy_setopt(_curl, CURLOPT_URL, driveUrl);
        curl_easy_setopt(_curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(_curl, CURLOPT_POSTFIELDS, metaData.data());
        curl_easy_setopt(_curl, CURLOPT_POSTFIELDSIZE,metaData.size());
        std::string response;
        curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &response);

        _res = curl_easy_perform(_curl);
        curl_easy_getinfo(_curl,CURLINFO_RESPONSE_CODE,&http_code);
        if(_res != CURLE_OK) {
            std::string msg = "Erreur curl: ";
            msg += curl_easy_strerror(_res);
            _logger->log(LogLevel::ERROR,msg.c_str());
        } else if(_res == CURLE_OK)
        {
            std::string msg = "Reponse Google Drive : "; 
            msg += response;
            _logger->log(LogLevel::INFO,msg.c_str());
            if(http_code == 401) {
                if(regenaratetoken()) {
                    returnId = createToDrive(name,isFile);
                } else {
                    _logger->log(LogLevel::ERROR,"Impossible de regénerer le token");
                }
            } else if(http_code == 200) {
                nlohmann::json json = nlohmann::json::parse(response);
                if(json.contains("id")) {
                    returnId = json["id"];
                } else {
                    _logger->log(LogLevel::ERROR,"Cette réponse ne contient pas d'id");
                }
            } else {
                _logger->log(LogLevel::ERROR,"Impossible de faire la création depuis google Drive");
            }
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(_curl);
    }
    return returnId;
}

bool CloudIO::createFileToDrive(const std::string & fileName) {
    std::string returnId = createToDrive(fileName,true);
    if(!returnId.empty()) _localSettings->setId(fileName,returnId);
    return !returnId.empty();
}

bool CloudIO::createDirToDrive(const std::string & dirName) {
    std::string returnId = createToDrive(dirName,false);
    if(!returnId.empty()) _localSettings->setId(dirName,returnId);
    return !returnId.empty();
}

bool CloudIO::deleteFileFromDrive(const std::string &elementName)
{
    const auto elementId = _localSettings->getElementId(elementName);
    if(!elementId.has_value()) return false;
    CURL *_curl;
    CURLcode _res;
    long httpCode;
    bool succes = false;
    const std::string driveUrl = std::string("https://www.googleapis.com/drive/v3/files/") +elementId.value();

    _curl = curl_easy_init();
    if(_curl) {
        struct curl_slist *headers = nullptr;
        headers = curl_slist_append(headers, ("Authorization: Bearer " + dsConfig::access_token).c_str());

        curl_easy_setopt(_curl, CURLOPT_URL, driveUrl.c_str());
        curl_easy_setopt(_curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(_curl, CURLOPT_CUSTOMREQUEST, "DELETE");
        std::string response;
        curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &response);

        _res = curl_easy_perform(_curl);
        curl_easy_getinfo(_curl, CURLINFO_RESPONSE_CODE, &httpCode);
        
        if(_res != CURLE_OK) {
            std::string msg = "Erreur curl: ";
            msg += curl_easy_strerror(_res);
            _logger->log(LogLevel::ERROR,msg.c_str());
            succes = false;
        } 
        else if(_res == CURLE_OK)
        {
            if(httpCode == 204) {
                succes = true;
                _localSettings->removeElement(elementName);
                _logger->log(LogLevel::INFO,"Suppression réussie (HTTP 204)");
            } else if(httpCode == 401) {
                if(regenaratetoken()) {
                    succes = deleteFileFromDrive(elementName);
                } else {
                    _logger->log(LogLevel::ERROR,"Impossible de regénerer le token");
                    succes = false;
                }
            }
            else {
                _logger->log(LogLevel::ERROR, std::string("Echec suppression HTTP: " + std::to_string(httpCode)).c_str());
                succes = false;
            }
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(_curl);
    }
    return succes;
}

/**
 * This function is use to regenerate token, because, all token expire after 1 hours
 */
bool CloudIO::regenaratetoken() {
    CURL *curl;
    CURLcode resp;
    long httpCode;
    curl = curl_easy_init();
    bool success = false;
    const char* url = "https://oauth2.googleapis.com/token";
    if(curl) {
        std::string postData = "client_id=" + dsConfig::client_id + 
                    "&client_secret=" + dsConfig::client_secret + 
                    "&refresh_token=" + dsConfig::refresh_token + 
                    "&grant_type=refresh_token";
        curl_easy_setopt(curl,CURLOPT_URL,url);
        curl_easy_setopt(curl,CURLOPT_POSTFIELDS,postData.data());
        curl_easy_setopt(curl,CURLOPT_POSTFIELDSIZE,postData.size());
        std::string response;
        curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,WriteCallback);
        curl_easy_setopt(curl,CURLOPT_WRITEDATA,&response);
        resp = curl_easy_perform(curl);
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
        if(resp != CURLE_OK) {
            std::string msg = "Erreur curl: ";
            msg += curl_easy_strerror(resp);
            _logger->log(LogLevel::ERROR,msg.c_str());
            success = false;
        } 
        else if(resp == CURLE_OK)
        {
            if(httpCode == 200) {
                nlohmann::json json = nlohmann::json::parse(response);
                if(json.contains("access_token")) {
                    const std::string msg = "new token = "+std::string(json["access_token"]);
                    _logger->log(LogLevel::INFO,msg.c_str());
                    dsConfig::access_token = json["access_token"];
                    _localSettings->setLocalConfig("access_token",dsConfig::access_token);
                    success = true;
                } else {
                    _logger->log(LogLevel::ERROR,"Réponse reçu sans le nouveau token");
                    success = false;
                }
            } else if(httpCode == 404) {
                _logger->log(LogLevel::ERROR, std::string("Echec refresh token HTTP: " + std::to_string(httpCode)).c_str());
            }
        }
        curl_easy_cleanup(curl);
    } else {
        _logger->log(LogLevel::ERROR,"Erreur d'initialisation de curl pour refresh le token");
        return false;
    }
    return success;
}