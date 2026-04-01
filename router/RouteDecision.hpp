#pragma once
#include <string>
#include <vector>

struct RouteDecision 
{
    std::string root;      
    // DEFAULT: used only if config not loaded
    // TODO: value should come from server/location config

    std::string index;     
    // DEFAULT fallback
    // TODO: override from config (location > server)

    std::string fullPath;
    // FINAL VALUE: computed by Router (keep this, not from config)

    bool autoindex;        
    // DEFAULT false
    // TODO: set from config

    std::string uploadPath;
    // TODO: must come from config (no real default)

    std::string cgiPass;   
    // TODO: must come from config (no real default)

    std::vector<std::string> methods;
    // TODO: fill from config (GET, POST, DELETE...)

    int redirectCode;      
    std::string redirectUrl;
    // TODO: set only if config has redirect

    RouteDecision()
        : root("./www")          // TODO: remove/override after config
        , index("index.html")    // TODO: remove/override after config
        , autoindex(false)       // TODO: override from config
        , redirectCode(0)        // OK default (no redirect)
    {}
};

//Placeholder from before
// #pragma once
// #include <string>
// #include <vector>

// // NOTE: Output of Router: tells server *what* to do with a request.
// struct RouteDecision 
// {
//   std::string root = "./www";       // filesystem root for static content
//   std::string resolvedPath;         // filled by Router/Handler
//   bool isCgi = false;
//   bool allowUpload = false;
//   std::string uploadDir = "./uploads";
//   std::vector<std::string> allowedMethods = {"GET", "POST", "DELETE"};
// };
