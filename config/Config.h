#ifndef __CONFIG_H__
#define __CONFIG_H__
#include <iostream>

namespace dsConfig {
    constexpr size_t MAX_FILENAME_LEN = 256;
    inline std::string refresh_token;
    inline  std::string client_secret;
    inline std::string client_id;
    inline std::string access_token;
}

#endif