#ifndef __CONFIG_H__
#define __CONFIG_H__
#include <iostream>

namespace dsConfig {
    constexpr size_t MAX_FILENAME_LEN = 256;
    std::string refresh_token;
    std::string client_secret;
    std::string client_id;
    std::string access_token;
}

#endif