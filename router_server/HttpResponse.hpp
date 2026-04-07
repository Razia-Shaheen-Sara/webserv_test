#pragma once
#include <string>
#include <map>

// Output of all Handlers. Person 1 will read this and send it over the socket.
// This is the permanent interface between Person 3 (handlers) and Person 1 (networking).
struct HttpResponse
{
    int                             statusCode;
    std::string                     statusText;
    std::map<std::string, std::string> headers;
    std::string                     body;

    HttpResponse()
        : statusCode(200)
        , statusText("OK")
    {}

    // Convenience: set Content-Type and Content-Length together
    void setBody(const std::string& content, const std::string& contentType)
    {
        body = content;
        headers["Content-Type"]   = contentType;
        headers["Content-Length"] = std::to_string(content.size());
    }
};
