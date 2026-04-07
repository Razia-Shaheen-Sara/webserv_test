#pragma once
#include <string>
#include <vector>



//RouteDecision is the output of the Router. It tells the server what to do with a request, based on the request path and the server configuration. It contains all the information needed to handle the request, such as the root directory for static content, the index file, whether autoindex is enabled, upload path, CGI settings, allowed methods, and redirect information. The Router will fill this struct based on the best matching location block in the server configuration.
//it is just a struct that holds all the information needed to handle a request, and it is filled by the Router based on the server configuration and the request path. It is not responsible for any logic, it just holds data. The Router is responsible for filling this struct with the correct values based on the best matching location block in the server configuration.
struct RouteDecision 
{
    std::string root;      
    std::string index;//index of the directory, default is "index.html", but it should come from location config if defined. this is used when the request path is a directory, and we need to serve a default file. if the location config has an index defined, we use that, otherwise we fall back to "index.html". this allows for flexible configuration of directory indexes on a per-location basis.
    // DEFAULT fallback
    // TODO: override from config (location > server)
    std::string fullPath;
    // FINAL VALUE: computed by Router (keep this, not from config)
    bool autoindex;//default is false, but it should come from location config if defined. this is used when the request path is a directory, and we need to decide whether to generate an autoindex page or not. if the location config has autoindex enabled, we set this to true, otherwise we set it to false. this allows for flexible configuration of autoindex on a per-location basis.      
    //autoindex is a boolean that indicates whether to generate an autoindex page when the request path is a directory. it should come from the location config, and it should override any server-level autoindex setting. if the location config has autoindex enabled, we set this to true, otherwise we set it to false. this allows for flexible configuration of autoindex on a per-location basis.
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
        : root("./www")          // TODO: remove/override after config. this is temporary default, should come from server config
        , index("index.html")    // TODO: remove/override after config. this is temporary default, should come from location config
        , autoindex(false)       // TODO: override from config. this is temporary default, should come from location config
        , redirectCode(0)        // OK default (no redirect). this is a special value to indicate no redirect, since valid HTTP codes are > 0.this should be set from config if you have a redirect, otherwise it stays 0 to indicate no redirect.it is important to have a clear way to indicate "no redirect" without using a separate boolean, and using 0 is a common convention for this purpose. it is permanent until you set it from config, and it is a valid default for "no redirect" scenarios.
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
