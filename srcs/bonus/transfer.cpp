#include "../../Includes/Ircserv.hpp"

void list(int fd)
{
    const std::string filename = "./srcs/bonus/download/database";
    std::ifstream file(filename.c_str());
    if (file.is_open())
    {
        std::string line;
        std::string tmp = "\r\n";
        tmp += ":FILE NOTICE 1 :------------------------------------------\r\n";
        tmp += ":FILE NOTICE 1 :ID           |                       TITLE\r\n";
        tmp += ":FILE NOTICE 1 :------------------------------------------\r\n";
        send(fd, tmp.c_str(), tmp.length(), 0);
        while (std::getline(file, line))
        {
            tmp = "";
            std::size_t pos = line.find(":");
            if (pos != npos)
            {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);

                tmp += ":FILE NOTICE 1 : " + key + "       |" + "  " + value + "\r\n";
                send(fd, tmp.c_str(), tmp.length(), 0);
            }
        }
        file.close();
    }
    else
    {
        std::string tmp = ":FILE ERROR 1 :Failed to Open Database for Reading: " + filename + "\r\n";
        send(fd, tmp.c_str(), tmp.length(), 0);
    }
}

int writeToFile(int fd, std::string key, std::string value)
{
    const std::string filename = "./srcs/bonus/download/database";
    std::ofstream file(filename.c_str(), std::ios::app);
    std::string str = key + ':' + value;

    if (file.is_open())
    {
        file << str << std::endl;
        file.close();
        return 0;
    }
    else
    {
        std::string tmp = ":FILE ERROR 1 :Failed to Open Database for Writing: " + filename + "\r\n";
        send(fd, tmp.c_str(), tmp.length(), 0);
    }
    return -1;
}

std::string readFromFile(int fd, std::string searched_id)
{
    std::string tmp;

    const std::string filename = "./srcs/bonus/download/database";
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

                    tmp = ":FILE NOTICE 1 :\r\n";
                    send(fd, tmp.c_str(), tmp.length(), 0);

                    tmp = ":FILE NOTICE 1 :Title of the document: " + file_value + "\r\n";
                    send(fd, tmp.c_str(), tmp.length(), 0);
                    file.close();
                    return file_value;
                }
            }
        }
        file.close();
    }
    else
    {
        std::string tmp = ":FILE ERROR 1 :Failed to Open Database for Reading: " + filename + "\r\n";
        send(fd, tmp.c_str(), tmp.length(), 0);
    }
    return "";
}

void upload(int fd, std::string &msg)
{
    std::string newPath;
    std::string filename;
    std::string tmp;
    std::string ID;

    msg.erase(0, 8);
    newPath = path_management(fd, msg, filename);
    if (newPath.size() == 0)
    {
        std::string tmp = ":FILE ERROR 1 :Enter a Valid Path \r\n";
        send(fd, tmp.c_str(), tmp.length(), 0);
        return;
    }

    std::string command = "curl -k --max-time 15 --upload-file " + newPath + " https://transfer.sh/" + filename + " 2>&1";

    FILE *pipe = popen(command.c_str(), "r");
    if (!pipe)
    {
        tmp = ":FILE ERROR 1 :popen() failed! \r\n";
        send(fd, tmp.c_str(), tmp.length(), 0);
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

    if (last_pos != npos && second_last_pos != npos)
    {
        // Extract the id between the two /id/
        ID = result.substr(second_last_pos + 1, last_pos - second_last_pos - 1);
    }
    else
    {
        std::string tmp(buffer);
        tmp = ":FILE ERROR 1 :" + tmp + "\r\n";
        send(fd, tmp.c_str(), tmp.length(), 0);
        return;
    }

    if (writeToFile(fd, ID, filename) == -1)
        return;

    tmp = ":FILE NOTICE 1 :File Uploaded Successfully...\r\n";
    send(fd, tmp.c_str(), tmp.length(), 0);
}

void download(int fd, std::string &msg)
{
    std::string ID;
    std::string filename;
    std::string tmp;

    ID = trimFront(msg, 9);
    filename = readFromFile(fd, ID);
    if (filename.size() == 0)
    {
        tmp = ":FILE ERROR 1 :Document with ID = \"" + ID +"\" Not Found\r\n";
        send(fd, tmp.c_str(), tmp.length(), 0);
        return;
    }

    std::string cmd = "curl -k https://transfer.sh/" + ID + "/" + filename + " -o ./downloaded_" + filename;

    tmp = ":FILE NOTICE 1 :\r\n";
    send(fd, tmp.c_str(), tmp.length(), 0);

    tmp = ":FILE NOTICE 1 :Execute this COMMAND on your Terminal to download the document:\r\n";
    tmp += ":FILE NOTICE 1 :" + cmd + "\r\n";
    send(fd, tmp.c_str(), tmp.length(), 0);
}

void exitFunc(int fd)
{
    std::string tmp = ":FILE NOTICE 1 :Thanks For Your Visit...\r\n";
    send(fd, tmp.c_str(), tmp.length(), 0);
    return;
}

enum command_t
{
    UP = 8,
    DOWN,
    LIST,
    EXIT
};

void transfer(client_t &clients, size_t i, int n, std::string &msg)
{
    std::string tmp;
    int fd = clients[i].second.get_fd();
    std::string client_ip = clients[i].second.get_ip();

    switch (n)
    {
    case UP:
        if (client_ip == "::1" || client_ip == "::ffff:127.0.0.1" || client_ip == "127.0.0.1")
            upload(fd, msg);
        else
        {
            tmp = ":FILE ERROR 1 :Permission Denied For Remote Clients\r\n";
            send(fd, tmp.c_str(), tmp.length(), 0);
        }
        break;
    case DOWN:
        download(fd, msg);
        break;
    case LIST:
        list(fd);
        break;
    case EXIT:
        exitFunc(fd);
        break;

    default:
        msg = ":FILE ERROR 1 :Invalid Command\r\n";
        send(fd, msg.c_str(), msg.length(), 0);
        break;
    }
}
