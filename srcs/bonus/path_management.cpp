#include "../../Includes/Ircserv.hpp"

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

void list_dir(std::string &input, std::string &history)
{
    if (input[input.length() - 1] == '/' && input[input.length() - 2] == '/')
    {
        input = remove_extra_slashes(input);
        history = input + "/";
    }

    std::cout << " ==> Contents of directory " << input << ": \n\n";
    DIR *dir;
    struct dirent *ent;
    struct stat st;
    if ((dir = opendir(input.c_str())) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
            {
                continue;
            }

            std::string name = ent->d_name;
            std::string path = input + "/" + name;
            if (stat(path.c_str(), &st) == 0 && S_ISDIR(st.st_mode))
            {
                name += "/";
            }
            std::cout << name << std::endl;
        }
        closedir(dir);
    }
    else
    {
        std::cerr << "Could not open directory " << input << std::endl;
    }
}

std::string path_management()
{
    std::string input;
    std::string history;
    bool valid = false;

    history = "";
    while (!valid)
    {
        std::cout << "\nEnter file path ['clear' to retype]: " << history;
        std::getline(std::cin, input);
        if (input == "clear")
        {
            history = "";
            continue;
        }
        if (input == "." || input == "..")
        {
            if (input == ".")
            {
                size_t pos = history.find_last_of('/');
                input = history.substr(0, pos);
                history = "";
            }
            else
            {
                size_t last_pos = history.find_last_of("/");
                size_t second_last_pos = history.find_last_of("/", last_pos - 1);
                input = history.substr(0, second_last_pos);
                history = "";
            }
        }
        input = history + input;
        (input[input.length() - 1] == '/') ? history = input : history = input + "/";
        // Input is a file
        if (is_file(input.c_str()))
            valid = true;
        // Input is a directory
        else if (is_directory(input.c_str()))
        {
            list_dir(input, history);
        }
        else
        {
            // Input is neither a file nor a directory
            std::cerr << "\n"
                      << input << " is not a valid file or directory\n\n";
            input = remove_extra_slashes(input);
            size_t pos = input.find_last_of('/');

            input = input.substr(0, pos);
            history = input + "/";

            list_dir(input, history);
        }
    }

    std::cout << "chosen File: " << input << std::endl;

    return input;
}
