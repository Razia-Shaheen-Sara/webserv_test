#include "Router.hpp"
#include "Handler.hpp"
#include "FileSystem.hpp"
#include <iostream>

// Helper: print an HttpResponse summary
static void printResponse(const HttpResponse& res)
{
    std::cout << "  Status : " << res.statusCode << " " << res.statusText << "\n";
    for (const auto& h : res.headers)
        std::cout << "  Header : " << h.first << ": " << h.second << "\n";
    if (!res.body.empty() && res.body.size() < 200)
        std::cout << "  Body   : " << res.body << "\n";
    else if (!res.body.empty())
        std::cout << "  Body   : (" << res.body.size() << " bytes)\n";
}

static void runTest(const std::string& label,
                    Router& router, Handler& handler,
                    const std::string& method, const std::string& path,
                    const std::string& body = "")
{
    std::cout << "\n── " << label << "\n";
    std::cout << "  Request: " << method << " " << path << "\n";

    HttpRequest req;
    req.method = method;
    req.path   = path;
    req.body   = body;

    RouteDecision rd = router.route(req);
    std::cout << "  Matched root   : " << rd.root << "\n";
    std::cout << "  Matched index  : " << rd.index << "\n";
    std::cout << "  Redirect code  : " << rd.redirectCode << "\n";
    std::cout << "  Autoindex      : " << (rd.autoindex ? "yes" : "no") << "\n";

    HttpResponse res = handler.handle(rd, req);
    printResponse(res);
}

int main()
{
    // ── Build fake config ──────────────────────────────────────────
    ServerConfig config;
    config.root  = "./www";
    config.index = "index.html";

    LocationConfig locRoot;
    locRoot.path      = "/";
    locRoot.root      = "./www";
    locRoot.index     = "index.html";
    locRoot.autoindex = false;
    locRoot.methods   = {"GET", "POST", "DELETE"};

    LocationConfig locImages;
    locImages.path      = "/images";
    locImages.root      = "./www/images";
    locImages.autoindex = false;
    locImages.methods   = {"GET"};

    LocationConfig locUpload;
    locUpload.path       = "/upload";
    locUpload.root       = "./www";
    locUpload.uploadPath = "./www/uploads";
    locUpload.methods    = {"POST"};

    LocationConfig locRedirect;
    locRedirect.path        = "/old";
    locRedirect.redirectCode = 301;
    locRedirect.redirectUrl  = "/new";

    LocationConfig locAuto;
    locAuto.path      = "/files";
    locAuto.root      = "./www/files";
    locAuto.autoindex = true;
    locAuto.methods   = {"GET"};

    config.locations.push_back(locRoot);
    config.locations.push_back(locImages);
    config.locations.push_back(locUpload);
    config.locations.push_back(locRedirect);
    config.locations.push_back(locAuto);

    Router  router(config);
    Handler handler;

    std::cout << "══════════════════════════════════════════════\n";
    std::cout << "  ROUTER + HANDLER TEST SUITE\n";
    std::cout << "══════════════════════════════════════════════\n";

    // Test 1: basic longest-match
    runTest("T1 longest match: /images beats /",
            router, handler, "GET", "/images/cat.jpg");

    // Test 2: fallback to /
    runTest("T2 fallback to /",
            router, handler, "GET", "/about.html");

    // Test 3: boundary bug check — /imageset must NOT match /images
    runTest("T3 boundary: /imageset must not match /images",
            router, handler, "GET", "/imageset/x.jpg");

    // Test 4: serve root /
    runTest("T4 root / serves index",
            router, handler, "GET", "/");

    // Test 5: redirect
    runTest("T5 redirect 301",
            router, handler, "GET", "/old");

    // Test 6: method not allowed (POST to /images which only allows GET)
    runTest("T6 method not allowed",
            router, handler, "POST", "/images/cat.jpg");

    // Test 7: autoindex directory
    runTest("T7 autoindex on /files/",
            router, handler, "GET", "/files/");

    // Test 8: upload POST
    runTest("T8 upload POST",
            router, handler, "POST", "/upload/test.txt", "hello upload content");

    // Test 9: DELETE
    runTest("T9 DELETE existing file",
            router, handler, "DELETE", "/images/cat.jpg");

    return 0;
}
