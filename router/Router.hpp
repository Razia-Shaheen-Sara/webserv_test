#pragma once
#include "RouteDecision.hpp"
#include "ServerConfig.hpp"
#include "Http.hpp"

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
