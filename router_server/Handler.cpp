#include "Handler.hpp"
#include "FileSystem.hpp"
#include <algorithm>
#include <sstream>


HttpResponse Handler::handle(const RouteDecision& rd, const HttpRequest& req)
{
    // 1. Redirect takes priority over everything else
    if (rd.redirectCode != 0)
        return handleRedirect(rd);

    // 2. Check method is allowed
    if (!isMethodAllowed(rd, req.method))
        return makeError(405, "Method Not Allowed");

    // 3. Build the filesystem path for this request
    std::string fullPath = buildPath(rd, req);

    // 4. Path is a directory
    if (FileSystem::isDir(fullPath))
    {
        // Try index file first
        std::string withIndex = fullPath;
        if (withIndex.back() != '/')
            withIndex += '/';
        withIndex += rd.index;

        if (FileSystem::isFile(withIndex))
            return handleStaticFile(withIndex);

        // No index file — autoindex?
        if (rd.autoindex)
            return handleAutoindex(fullPath, req.path);

        return makeError(403, "Forbidden");
    }

    // 5. CGI
    if (!rd.cgiPass.empty())
        return handleCgi(rd, req, fullPath);

    // 6. Upload (POST with upload path configured)
    if (req.method == "POST" && !rd.uploadPath.empty())
        return handleUpload(rd, req);

    // 7. Static file (GET/DELETE or plain POST without upload config)
    if (req.method == "GET")
        return handleStaticFile(fullPath);

    if (req.method == "DELETE")
    {
        if (!FileSystem::exists(fullPath))
            return makeError(404, "Not Found");
        // Actual deletion will use std::remove — fine for now
        if (std::remove(fullPath.c_str()) != 0)
            return makeError(500, "Could not delete file");
        HttpResponse res;
        res.statusCode = 204;
        res.statusText = "No Content";
        return res;
    }

    return makeError(405, "Method Not Allowed");
}

std::string Handler::buildPath(const RouteDecision& rd, const HttpRequest& req)
{
    std::string root = rd.root;

    // Strip trailing slash from root so we don't get double slashes
    if (!root.empty() && root.back() == '/')
        root.pop_back();

    if (req.path == "/")
        return root + "/" + rd.index;

    return root + req.path;
}

// ─────────────────────────────────────────────
// PRIVATE helpers
// ─────────────────────────────────────────────

bool Handler::isMethodAllowed(const RouteDecision& rd, const std::string& method)
{
    // Empty methods list = no restriction configured = allow all
    if (rd.methods.empty())
        return true;

    for (const std::string& m : rd.methods)
    {
        if (m == method)
            return true;
    }
    return false;
}

HttpResponse Handler::handleRedirect(const RouteDecision& rd)
{
    HttpResponse res;
    res.statusCode = rd.redirectCode;

    // Assign reason text for the most common redirect codes
    if (rd.redirectCode == 301)      res.statusText = "Moved Permanently";
    else if (rd.redirectCode == 302) res.statusText = "Found";
    else if (rd.redirectCode == 307) res.statusText = "Temporary Redirect";
    else if (rd.redirectCode == 308) res.statusText = "Permanent Redirect";
    else                             res.statusText = "Redirect";

    res.headers["Location"] = rd.redirectUrl;
    res.headers["Content-Length"] = "0";
    return res;
}

HttpResponse Handler::handleStaticFile(const std::string& fullPath)
{
    if (!FileSystem::exists(fullPath))
        return makeError(404, "Not Found");

    std::string content;
    if (!FileSystem::readFile(fullPath, content))
        return makeError(500, "Could not read file");

    HttpResponse res;
    res.setBody(content, FileSystem::mimeType(fullPath));
    return res;
}

HttpResponse Handler::handleAutoindex(const std::string& dirPath,
                                       const std::string& uriPath)
{
    std::vector<std::string> entries;
    if (!FileSystem::listDir(dirPath, entries))
        return makeError(500, "Could not read directory");

    // Sort alphabetically so the listing is predictable
    std::sort(entries.begin(), entries.end());

    // Build a simple HTML page
    std::ostringstream html;
    html << "<!DOCTYPE html>\n"
         << "<html>\n<head><title>Index of " << uriPath << "</title></head>\n"
         << "<body>\n"
         << "<h1>Index of " << uriPath << "</h1>\n"
         << "<hr>\n<ul>\n";

    // Link back to parent (unless already at root)
    if (uriPath != "/")
        html << "  <li><a href=\"../\">../</a></li>\n";

    for (const std::string& name : entries)
    {
        std::string href = uriPath;
        if (href.back() != '/')
            href += '/';
        href += name;
        html << "  <li><a href=\"" << href << "\">" << name << "</a></li>\n";
    }

    html << "</ul>\n<hr>\n</body>\n</html>\n";

    HttpResponse res;
    res.setBody(html.str(), "text/html");
    return res;
}

HttpResponse Handler::handleCgi(const RouteDecision& rd, const HttpRequest& req,
                                  const std::string& fullPath)
{
    // CGI is the most complex feature — saved for last.
    // This stub returns a clear message so you know the path reached here.
    // Replace this entire function body when you implement CGI.
    (void)rd;
    (void)req;
    (void)fullPath;

    HttpResponse res;
    res.statusCode = 501;
    res.statusText = "Not Implemented";
    res.setBody("CGI not implemented yet\n", "text/plain");
    return res;
}

HttpResponse Handler::handleUpload(const RouteDecision& rd, const HttpRequest& req)
{
    if (rd.uploadPath.empty())
        return makeError(400, "No upload path configured");

    // Build destination: uploadPath + last segment of request path
    std::string filename = req.path;
    size_t slash = filename.rfind('/');
    if (slash != std::string::npos)
        filename = filename.substr(slash + 1);
    if (filename.empty())
        filename = "upload";

    std::string dest = rd.uploadPath;
    if (dest.back() != '/')
        dest += '/';
    dest += filename;

    if (!FileSystem::writeFile(dest, req.body))
        return makeError(500, "Could not write upload");

    HttpResponse res;
    res.statusCode = 201;
    res.statusText = "Created";
    res.headers["Location"] = "/" + filename;
    res.headers["Content-Length"] = "0";
    return res;
}

HttpResponse Handler::makeError(int code, const std::string& message)
{
    HttpResponse res;
    res.statusCode = code;
    res.statusText = message;

    std::string body = "<html><body><h1>"
                     + std::to_string(code) + " " + message
                     + "</h1></body></html>\n";
    res.setBody(body, "text/html");
    return res;
}
