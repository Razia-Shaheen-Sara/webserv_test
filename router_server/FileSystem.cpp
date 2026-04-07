#include "FileSystem.hpp"
#include <fstream>
#include <sstream>
#include <dirent.h>
#include <sys/stat.h>

bool FileSystem::exists(const std::string& path)
{
    struct stat st;
    return stat(path.c_str(), &st) == 0;
}

bool FileSystem::isFile(const std::string& path)
{
    struct stat st;
    if (stat(path.c_str(), &st) != 0)
        return false;
    return S_ISREG(st.st_mode);
}

bool FileSystem::isDir(const std::string& path)
{
    struct stat st;
    if (stat(path.c_str(), &st) != 0)
        return false;
    return S_ISDIR(st.st_mode);
}

bool FileSystem::readFile(const std::string& path, std::string& outContent)
{
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open())
        return false;
    std::ostringstream ss;
    ss << file.rdbuf();
    outContent = ss.str();
    return true;
}

bool FileSystem::listDir(const std::string& path, std::vector<std::string>& outEntries)
{
    DIR* dir = opendir(path.c_str());
    if (!dir)
        return false;
    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr)
    {
        std::string name = entry->d_name;
        if (name == "." || name == "..")
            continue;
        outEntries.push_back(name);
    }
    closedir(dir);
    return true;
}

bool FileSystem::writeFile(const std::string& path, const std::string& content)
{
    std::ofstream file(path, std::ios::binary);
    if (!file.is_open())
        return false;
    file.write(content.c_str(), content.size());
    return file.good();
}

std::string FileSystem::mimeType(const std::string& path)
{
    size_t dot = path.rfind('.');
    if (dot == std::string::npos)
        return "application/octet-stream";

    std::string ext = path.substr(dot);

    if (ext == ".html" || ext == ".htm") return "text/html";
    if (ext == ".css")                   return "text/css";
    if (ext == ".js")                    return "application/javascript";
    if (ext == ".json")                  return "application/json";
    if (ext == ".txt")                   return "text/plain";
    if (ext == ".jpg" || ext == ".jpeg") return "image/jpeg";
    if (ext == ".png")                   return "image/png";
    if (ext == ".gif")                   return "image/gif";
    if (ext == ".ico")                   return "image/x-icon";
    if (ext == ".pdf")                   return "application/pdf";

    return "application/octet-stream";
}
