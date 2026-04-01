#pragma once
#include "RouteDecision.hpp"

// ServerConfig.hpp (temporary)
#include <string>
#include <vector>

struct LocationConfig
{
    std::string path;
    std::string root;
    std::string index;
    bool autoindex;
    std::string uploadPath;
    std::string cgiPass;
    std::vector<std::string> methods;
    int redirectCode;
    std::string redirectUrl;
};


//temporary
struct ServerConfig
{
    std::string root;
    std::string index;
    std::vector<LocationConfig> locations;
};

//temporary
struct HttpRequest
{
    std::string method;  // "GET", "POST"
    std::string path;    // "/images/cat.jpg"
};

class Router 
{
   
  public:
      Router() : _config(nullptr) {}
      Router(const ServerConfig& config);
      RouteDecision route(const HttpRequest& req) const;

  private:
      const ServerConfig* _config;
};


// #pragma once
// #include "Http.hpp"
// #include "RouteDecision.hpp"

// // NOTE: Router decides which handler should run, based on request + (later) config.
// class Router {
// public:
//   RouteDecision route(const HttpRequest& req) const;

//   // NOTE: future extension:
//   // - pass Config reference into Router constructor
//   // - match server_name + listen port + location blocks
// };
