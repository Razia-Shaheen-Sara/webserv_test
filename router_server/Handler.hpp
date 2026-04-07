#pragma once
#include "Router.hpp"
#include "HttpResponse.hpp"

// Handler receives a RouteDecision + HttpRequest and produces an HttpResponse.
// Each method handles one specific case. The main entry point is handle().
//
// Call order inside handle():
//   1. redirect?   → handleRedirect()
//   2. method allowed? → 405 if not
//   3. build full path
//   4. path is directory?
//        autoindex on  → handleAutoindex()
//        autoindex off → serve index file (back to step 4 with index appended)
//   5. CGI?        → handleCgi()      (stub for now)
//   6. POST/upload?→ handleUpload()
//   7. GET file    → handleStaticFile()
class Handler
{
public:
    // Main entry point. Person 1 will call this.
    HttpResponse handle(const RouteDecision& rd, const HttpRequest& req);

    // Build the filesystem path from root + request path.
    // Kept public so main.cpp test can still call it directly.
    std::string buildPath(const RouteDecision& rd, const HttpRequest& req);

private:
    // Returns true if method string is in the allowed methods list.
    // Empty methods list = allow everything (no restriction configured).
    bool isMethodAllowed(const RouteDecision& rd, const std::string& method);

    // Case 1: location has a redirect configured
    HttpResponse handleRedirect(const RouteDecision& rd);

    // Case 2: serve a static file from disk
    HttpResponse handleStaticFile(const std::string& fullPath);

    // Case 3: generate an HTML directory listing
    HttpResponse handleAutoindex(const std::string& dirPath, const std::string& uriPath);

    // Case 4: CGI execution (stub — fills in later)
    HttpResponse handleCgi(const RouteDecision& rd, const HttpRequest& req,
                           const std::string& fullPath);

    // Case 5: handle file upload (POST body → disk)
    HttpResponse handleUpload(const RouteDecision& rd, const HttpRequest& req);

    // Build a simple error response
    HttpResponse makeError(int code, const std::string& message);
};
