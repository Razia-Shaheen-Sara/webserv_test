#include "Router.hpp"
#include <iostream>

Router::Router(const ServerConfig& config)
    : _config(&config)
{}

RouteDecision Router::route(const HttpRequest& req) const 
{
    const std::string& uri = req.path;  // e.g. "/images/cat.jpg"

    RouteDecision best;         // starts with defaults from constructor
    size_t        bestLen = 0;  // length of best match so far

    std::cout << "[Router] route called" << std::endl;
    if (!_config)
    {
        std::cerr << "Router error: null config\n";
        return RouteDecision();
    }
    for (const LocationConfig& loc : _config->locations) 
    {
        // Does the URI start with this location's path?
        if (uri.find(loc.path) == 0) 
        {
            // Longer match = more specific = wins
            if (loc.path.size() > bestLen) 
            {
                bestLen = loc.path.size();

                // Fill the decision from this location
                best.root        = loc.root.empty() ? _config->root : loc.root;
                best.index       = loc.index;
                best.autoindex   = loc.autoindex;
                best.allowUpload = !loc.uploadPath.empty();
                best.uploadPath  = loc.uploadPath;
                best.allowCgi    = !loc.cgiPass.empty();
                best.cgiPass     = loc.cgiPass;
                best.methods     = loc.methods;
                best.redirectCode = loc.redirectCode;
                best.redirectUrl  = loc.redirectUrl;
            }
        }
    }
    return best;
}