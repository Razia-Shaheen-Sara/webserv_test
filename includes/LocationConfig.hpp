#ifndef LOCATIONCONFIG_HPP
#define LOCATIONCONFIG_HPP

#include <string>
#include <vector>

struct LocationConfig {
    std::string              path;         // "/images"
    std::vector<std::string> methods;      // ["GET", "POST"]
    std::string              root;         // overrides server root
    std::string              index;        // "index.html"
    bool                     autoindex;
    std::string              uploadPath;   // empty = uploads not allowed
    std::string              cgiPass;      // empty = CGI not allowed
    int                      redirectCode; // 0 = no redirect
    std::string              redirectUrl;

    LocationConfig()
        : autoindex(false)
        , redirectCode(0)
    {}
};

#endif