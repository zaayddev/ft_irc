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
std::cout << "befor rename = " << newPath << std::endl;
    size_t pos = newPath.find_last_of('/');
    if (pos != npos)
    {
        replaceSpecialChars(newPath);
        filename = newPath.substr(pos + 1);
        std::string tmp = ":FILE NOTICE 1 :filename: " + filename + "\r\n";
        // std::cout << tmp << std::endl;
        send(fd, tmp.c_str(), tmp.length(), 0);
    }
    else
    {
        std::string tmp = ":FILE NOTICE 1 :path separator not found in path \r\n";
        // std::cout << tmp << std::endl;
        send(fd, tmp.c_str(), tmp.length(), 0);
        return "";
    }
// replaceSpecialChars(filename);
    if (std::rename(oldPath.c_str(), newPath.c_str()) != 0)
    {
        std::string tmp = ":FILE NOTICE 1 :re-naming File Failed, keep with the old one\r\n";
        std::cout << tmp << std::endl;
        send(fd, tmp.c_str(), tmp.length(), 0);
        // if we dont have permissions
        return (oldPath);
    }
std::cout << "after rename = " << newPath << std::endl;
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

std::string remove_extra_slashes(const std::string &path)
{
    std::string result = "";
    bool slash_found = false;
    for (std::string::const_iterator it = path.begin(); it != path.end(); ++it)
    {
        if (*it == '/')
        {
            if (!slash_found)
            {
                result += '/';
                slash_found = true;
            }
        }
        else
        {
            result += *it;
            slash_found = false;
        }
    }
    if (result.size() > 1 && result[result.size() - 1] == '/')
    {
        result.erase(result.size() - 1);
    }
    return result;
}

std::string path_management(int fd, std::string &msg, std::string &filename)
{
    std::string path;

    path = msg;

    if (is_file(path.c_str()))
    {
        // ret = rename_if_necessary(fd, path, filename);
        return rename_if_necessary(fd, path, filename);
        // if (ret.size() == 0)
        //     return "";
    }
    // Input is a directory
    else if (is_directory(path.c_str()))
    {
        std::string tmp = ":FILE NOTICE 1 :" + path + " is a directory not a file" + "\r\n";
        send(fd, tmp.c_str(), tmp.length(), 0);
        return "";
    }
    else
    {
        std::string tmp = ":FILE NOTICE 1 :" + path + " is not a valid file nor a directory" + "\r\n";
        send(fd, tmp.c_str(), tmp.length(), 0);
        return "";
    }

    // std::cout << "chosen File: " << path << std::endl;
    // std::string tmp = "chosen File: " + ret + "\r\n";
    // send(fd, tmp.c_str(), tmp.length(), 0);

    // return path;
    // (int fd, std::string &oldPath, std::string &filename)

    return "";
}
