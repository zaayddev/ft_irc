#include "../../Includes/Ircserv.hpp"

void replaceSpecialChars(std::string &str)
{
    std::string::size_type pos = 0;
    while ((pos = str.find_first_of(" !\"#$%&'()*+,:;<=>?@[\\]^`{|}~")) != npos)
    {
        str.replace(pos, 1, "_");
    }
}

std::string rename_if_necessary(int fd, std::string &oldPath, std::string &filename)
{
    std::string newPath = oldPath;
 
    size_t pos = newPath.find_last_of('/');
    if (pos != npos)
    {
        replaceSpecialChars(newPath);
        filename = newPath.substr(pos + 1);
        std::string tmp = ":FILE NOTICE 1 :filename: " + filename + "\r\n";
        send(fd, tmp.c_str(), tmp.length(), 0);
    }
    else
    {
        std::string tmp = ":FILE ERROR 1 :path separator not found in path \r\n";
        send(fd, tmp.c_str(), tmp.length(), 0);
        return "";
    }
    if (std::rename(oldPath.c_str(), newPath.c_str()) != 0)
    {
        std::string tmp = ":FILE ERROR 1 :re-naming File Failed, keep with the old one\r\n";
        send(fd, tmp.c_str(), tmp.length(), 0);
        // if we dont have permissions
        return (oldPath);
    }
    return (newPath);
}

bool is_file(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

bool is_directory(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISDIR(path_stat.st_mode);
}

std::string path_management(int fd, std::string &msg, std::string &filename)
{
    std::string path;

    path = msg;

    if (is_file(path.c_str()))
        return rename_if_necessary(fd, path, filename);
    else if (is_directory(path.c_str()))
    {
        std::string tmp = ":FILE ERROR 1 :" + path + " is a Directory not a File" + "\r\n";
        send(fd, tmp.c_str(), tmp.length(), 0);
        return "";
    }
    else
    {
        std::string tmp = ":FILE ERROR 1 :" + path + " is not a valid File nor a Directory" + "\r\n";
        send(fd, tmp.c_str(), tmp.length(), 0);
        return "";
    }
    return "";
}
