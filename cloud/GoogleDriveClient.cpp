#include "GoogleDriveClient.h"
#include "../utils/Logger.h"
#include "key.h"

CloudIO::CloudIO() {
    _logger = new Logger("GoogleDriveClient.cpp");
}

CloudIO::~CloudIO() {
    delete _logger;
}

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
    size_t total = size * nmemb;
    s->append((char*)contents, total);
    return total;
}

bool CloudIO::sendToDrive(const std::string & fileName) {
    bool success = true;
    // Lecture du fichier
    std::ifstream file(fileName, std::ios::binary);
    if (!file) {
        _logger->log(LogLevel::ERROR,"Impossible d'ouvrir le fichier.");
        success = false;
        return success;
    }
    std::string file_contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    curl_global_init(CURL_GLOBAL_ALL);
    _curl = curl_easy_init();
    if(_curl) {
        struct curl_slist *headers = nullptr;
        headers = curl_slist_append(headers, ("Authorization: Bearer " + _access_token).c_str());
        headers = curl_slist_append(headers, "Content-Type: multipart/related; boundary=boundary123");

        //14NuNzh2ktZswGVtcmlb7dhFkOXcpF0SY
        std::string metadata = 
            "--boundary123\r\n"
            "Content-Type: application/json; charset=UTF-8\r\n\r\n"
             "{ \"name\": \"" + fileName + "\", \"parents\": [\"14NuNzh2ktZswGVtcmlb7dhFkOXcpF0SY\"] }\r\n"
            "--boundary123\r\n"
            "Content-Type: text/plain\r\n\r\n" +
            file_contents +
            "\r\n--boundary123--";

        curl_easy_setopt(_curl, CURLOPT_URL, "https://www.googleapis.com/upload/drive/v3/files?uploadType=multipart");
        curl_easy_setopt(_curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(_curl, CURLOPT_POSTFIELDS, metadata.c_str());
        curl_easy_setopt(_curl, CURLOPT_POSTFIELDSIZE, metadata.size());
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
    curl_global_cleanup();
    return success;
}