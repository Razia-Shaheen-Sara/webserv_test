#include "Router.hpp"
#include <iostream>

Router::Router(const ServerConfig& config)
    : _config(&config)
{}
RouteDecision Router::route(const HttpRequest& req) const 
{
    const std::string& reqPath = req.path;//Creates a reference to req.path
    //reqPath points to the same string inside request
    //reqPath request path from HTTP Example:/images/cat.jpg, /login, /
    RouteDecision best;//Creates an empty result object-Will store the “best matching route” found
    size_t bestLen = 0;
    bool found = false;
    if (!_config)
    {
        std::cerr << "Router error: null config\n";
        return RouteDecision();//Returns empty routing result immediately
    }
    //lop all locations from config
    for (const LocationConfig& loc : _config->locations) 
    {
        if (reqPath.find(loc.path) == 0)//Checks: URI starts with location path-Example:/images/cat.jpg matches /images
        {
            if (loc.path.size() > bestLen)//Longest match wins(nginx-like behavior)
            {
                bestLen = loc.path.size();
                found = true;

                // fallback logic (GOOD)
                best.root = loc.root.empty() ? _config->root : loc.root;//fallback chain: if missing --location → server
               
                // ⚠️ FIX: fallback needed//👉 later change:"index.html" should come from server config, not hardcoded
                best.index = loc.index.empty() ? "index.html" : loc.index;
                best.autoindex = loc.autoindex;
                best.uploadPath = loc.uploadPath;
                best.cgiPass = loc.cgiPass;
                best.methods = loc.methods;

                best.redirectCode = loc.redirectCode;
                best.redirectUrl = loc.redirectUrl;

                // ⚠️ FIX: avoid double slash
                if (reqPath == "/")
                    best.fullPath = best.root + "/" + best.index;
                else
                    best.fullPath = best.root + reqPath;//Builds actual file path👉 core responsibility of Router
            }
        }
    }
    if (!found)
        return RouteDecision();
    return best;//return best ONLY when:at least one valid location matched or you have a defined fallback route logic
}

#include "Router.hpp"
#include <iostream>

int main()
{
    // Fake config
    ServerConfig config;
    config.root = "./www";

    LocationConfig loc;
    loc.path = "/";
    loc.root = "./www";
    config.locations.push_back(loc);
    LocationConfig loc1;
    loc1.path = "/";
    loc1.root = "./www";

    LocationConfig loc2;
    loc2.path = "/images";
    loc2.root = "./www/images";

    config.locations.push_back(loc1);
    config.locations.push_back(loc2);

    // Create router
    Router router(config);

    // Fake request
    HttpRequest req;
    req.path = "/images/cat.jpg";
    //req.path = "/index.html";

    // Route
    RouteDecision d = router.route(req);

    std::cout << "Root: " << d.root << std::endl;
    std::cout << "FullPath: " << d.root + req.path << std::endl;

    return 0;
}


// ⚠️ Things to REMOVE / CHANGE later
// 1. Hardcoded "index.html"
// best.index = loc.index.empty() ? "index.html" : loc.index;

// 👉 later:

// best.index = loc.index.empty() ? _config->index : loc.index;
// 2. Debug print
// std::cout << "[Router] route called" << std::endl;

// 👉 remove later (only for debugging)

// 3. main() test code
// Entire main() is temporary
// Remove when integrating with server
// ❗ Subtle issue (important)

// Your match:

// if (reqPath.find(loc.path) == 0)

// Problem:

// /images123 will match /images ❌

// Better (later improvement):

// if (reqPath.compare(0, loc.path.size(), loc.path) == 0)