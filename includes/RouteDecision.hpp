#pragma once
#include <string>
#include <vector>

// NOTE: Output of Router: tells server *what* to do with a request.
struct RouteDecision 
{
    std::string              root;          // which folder to serve from e.g. "./www"
    std::string              index;         // default file e.g. "index.html"
    bool                     autoindex;     // show directory listing? true/false
    bool                     allowUpload;   // is uploading allowed here?
    std::string              uploadPath;    // where to save uploaded files
    bool                     allowCgi;      // is CGI (scripts) allowed?
    std::string              cgiPass;       // path to interpreter e.g. "/usr/bin/python3"
    std::vector<std::string> methods;       // allowed methods e.g. ["GET", "POST"]
    int                      redirectCode;  // 0 = no redirect, 301 = moved
    std::string              redirectUrl;   // where to redirect to
    bool                     isCgi;

    // Constructor — safe defaults so nothing is garbage
    RouteDecision()
        : root("./www")
        , index("index.html")
        , autoindex(false)
        , allowUpload(false)
        , allowCgi(false)
        , redirectCode(0)
        , isCgi(false)
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
