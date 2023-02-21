#include "../../Includes/Ircserv.hpp"

void list()
{
    const std::string filename = "./download/database";
    std::ifstream file(filename.c_str());
    if (file.is_open())
    {
        std::string line;
        std::cout << std::left << std::setw(14) << "\nID"
                  << "|";
        std::cout << std::right << std::setw(30) << "TITLE"
                  << "|\n";
        std::cout << "---------------------------------------------\n\n";
        while (std::getline(file, line))
        {
            std::size_t pos = line.find(":");
            if (pos != npos)
            {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);

                std::cout << std::left << std::setw(13) << key << "|";
                std::cout << "  " << value << std::endl;
            }
        }
        file.close();
    }
    else
    {
        std::cerr << "Failed to open file for reading: " << filename << std::endl;
    }
}

void writeToFile(std::string key, std::string value)
{
    const std::string filename = "./download/database";
    std::ofstream file(filename.c_str(), std::ios::app);
    std::string str = key + ':' + value;

    if (file.is_open())
    {
        file << str << std::endl;
        file.close();
    }
    else
    {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
    }
}

std::string readFromFile(std::string searched_id)
{
    const std::string filename = "./download/database";
    std::ifstream file(filename.c_str());
    if (file.is_open())
    {
        std::string line;
        while (std::getline(file, line))
        {
            std::size_t pos = line.find(":");
            if (pos != npos)
            {
                std::string file_id = line.substr(0, pos);
                if (file_id == searched_id)
                {
                    std::string file_value = line.substr(pos + 1);
                    std::cout << "Title of the document : " << file_value << std::endl;
                    return file_value;
                }
            }
        }
        file.close();
    }
    else
    {
        std::cerr << "Failed to open file for reading: " << filename << std::endl;
    }
    return NULL;
}

void replaceSpecialChars(std::string &str)
{
    std::string::size_type pos = 0;
    while ((pos = str.find_first_of(" !\"#$%&'()*+,:;<=>?@[\\]^`{|}~")) != npos)
    {
        str.replace(pos, 1, "_");
    }
}

void upload()
{
    std::string oldPath;
    std::string filename;
    oldPath = path_management();

    std::string newPath = oldPath;

    size_t pos = newPath.find_last_of('/');
    if (pos != npos)
    {
        filename = newPath.substr(pos + 1);
        replaceSpecialChars(filename);
        std::cout << "filename: " << filename << std::endl;
    }
    else
    {
        std::cerr << "path separator not found in path" << std::endl;
    }

    if (std::rename(oldPath.c_str(), newPath.c_str()) != 0)
    {
        std::cout << "Error renaming file" << std::endl;
        // if we dont have permissions
    }

    std::string command = "curl -k --progress-bar --upload-file " + newPath + " https://transfer.sh/" + filename + " 2>&1";
    FILE *pipe = popen(command.c_str(), "r");
    if (!pipe)
    {
        std::cerr << "popen() failed!" << std::endl;
        return;
    }
    std::string result;
    char buffer[128];
    while (!feof(pipe))
    {
        if (fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }
    pclose(pipe);

    // Extract the id
    size_t last_pos = result.find_last_of("/");
    size_t second_last_pos = result.find_last_of("/", last_pos - 1);
    // Extract the id between the two /id/
    std::string id = result.substr(second_last_pos + 1, last_pos - second_last_pos - 1);
    std::cout << "ID = " << id << std::endl;
    writeToFile(id, filename);
    std::cout << "File Uploaded Successfully..." << std::endl;
}

void download()
{
    std::string ID;
    std::string filename;
    std::cout << "Enter the ID of the document -use LIST cammand to get IDs-\n";
    std::getline(std::cin >> std::ws, ID);

    filename = readFromFile(ID);

    // Download the file using id
    std::string command_download = "curl -k https://transfer.sh/" + ID + "/" + filename + " -o ./download/downloaded_" + filename + " 2>&1";
    FILE *pipe_download = popen(command_download.c_str(), "r");
    if (!pipe_download)
    {
        std::cerr << "popen() failed!" << std::endl;
        return;
    }
    pclose(pipe_download);

    std::cout << "File Downloaded Successfully.." << std::endl;
}

void transfer(client_t &clients, size_t i)
{
    // std::string action;
    char buffer[1024];
    ssize_t n_read;

    int fd = clients[i].second.get_fd();

    while (true)
    {
        // std::cout << "\ninput [UP / DOWN / LIST / EXIT]: \n";
        // std::getline(std::cin >> std::ws, action);

        
        std::memset(buffer, 0, sizeof(buffer));
        std::string msg = "\ninput [UP / DOWN / LIST / EXIT]: \n";
        send(fd, msg.c_str(), msg.length(), 0);

        n_read = recv(fd, buffer, sizeof(buffer), 0);
        // n_read = read(fd, buffer, sizeof(buffer));
        if (n_read == -1 && errno != EWOULDBLOCK)
        {
            std::cerr << "Error reading from file descriptor\n";
            return;
        }

        std::string action(buffer, n_read);
        if(action.size())
            send(fd, action.c_str(), action.length(), 0);

        if (action.compare("UP") == 0)
            upload();
        else if (action.compare("DOWN") == 0)
            download();
        else if (action.compare("LIST") == 0)
            list();
        else if (action.compare("EXIT") == 0)
        {
            std::cout << "\nThanks For Your Visit..." << std::endl;
            return;
        }
    }

    send(clients[i].second.get_fd(), "hello", 5, 0);
}