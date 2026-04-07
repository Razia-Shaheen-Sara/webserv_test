#include "Router.hpp"
#include <iostream>

//Router constructor
//ServerConfig = struct made for server configuration (root, index, locations)
//servConfig = pointer to ServerConfig- a private member of Router class- connects Router to server data
//servInput = parameter passed to Router constructor- used to initialize servConfig pointer

Router::Router(const ServerConfig& servInput): servConfig(&servInput)// Initialize router with server configuration
{}

//route function has ONE job:
// “Find the best matching location block for the request URI”
// Loop over each LocationConfig inside server configuration
//'loc' refers to one location at a time (read-only)
//servConfig->locations is a dynamic array(vector) of LocationConfig structs defined in ServerConfig
//Router output = direct reflection of config.

RouteDecision Router::route(const HttpRequest& req) const
{
    RouteDecision best;
    size_t bestLen = 0;
    bool found = false;

    if (!servConfig)
        return RouteDecision();
    for (const LocationConfig& loc : servConfig->locations)
    {
        if (req.path.find(loc.path) == 0)
        {
            if (loc.path.size() > bestLen)
            {
                bestLen = loc.path.size();
                found = true;

                best.root = loc.root;
                if (best.root.empty())
                    best.root = servConfig->root;

                best.index = loc.index;
                best.autoindex = loc.autoindex;
                best.uploadPath = loc.uploadPath;
                best.cgiPass = loc.cgiPass;
                best.methods = loc.methods;
                best.redirectCode = loc.redirectCode;
                best.redirectUrl = loc.redirectUrl;
            }
        }
    }
    if (!found)
        return RouteDecision();
    return best;
}

#include "Router.hpp"
#include <iostream>

int main()
{
    ServerConfig config; // 1. FAKE CONFIG (input to Router)
    config.root = "./www";// server root (fallback if location has no root)
    
    // location 1: very generic catch-all route
    LocationConfig loc_root;
    loc_root.path = "/";
    loc_root.root = "./www";

    // location 2: more specific route (should win instead of /)
    LocationConfig loc_images;
    loc_images.path = "/images";
    loc_images.root = "./www/images";

    // add to config
    config.locations.push_back(loc_root);
    config.locations.push_back(loc_images);

    Router router(config); // 2. CREATE ROUTER with config
    HttpRequest req;// 3. FAKE HTTP REQUEST
    req.path = "/images/cat.jpg";
    RouteDecision result = router.route(req); // 4. RUN ROUTER
    std::cout << "==================== ROUTER TEST ====================\n";
    std::cout << "Request path: " << req.path << "\n\n";
    std::cout << "Expected match: /images (because longest prefix wins)\n";
    std::cout << "Expected root: ./www/images\n\n";
    std::cout << "Actual root returned: " << result.root << "\n";
    return 0;
}
