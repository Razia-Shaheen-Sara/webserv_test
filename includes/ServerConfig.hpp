#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include <string>
#include <vector>
#include <map>
#include "LocationConfig.hpp"

struct ServerConfig {
    std::string                  host;
    uint16_t                     port;
    std::vector<std::string>     serverNames;
    std::string                  root;
    size_t                       clientMaxBodySize;
    std::map<int, std::string>   errorPages;
    std::vector<LocationConfig>  locations;

    ServerConfig()
        : host("0.0.0.0")
        , port(8080)
        , clientMaxBodySize(1000000)
    {}
};

#endif