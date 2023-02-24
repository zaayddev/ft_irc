#include "../../Includes/Ircserv.hpp"

// using cmd_t = std::string;
// using cmd_func_t = std::function<void(client_t&, size_t, const cmd_t&)>;

void list(int fd, std::string &msg)
{

    (void)msg;
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

        std::cerr << "Failed to open file for reading: " << filename << std::endl;
        std::string tmp = ":FILE NOTICE 1 :Failed to open file for reading: " + filename + "\r\n";
        send(fd, tmp.c_str(), tmp.length(), 0);
    }
}

void writeToFile(int fd, std::string key, std::string value)
{
    const std::string filename = "./srcs/bonus/download/database";
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
        std::string tmp = ":FILE NOTICE 1 :Failed to open file for writing: " + filename + "\r\n";
        send(fd, tmp.c_str(), tmp.length(), 0);
    }
}

std::string readFromFile(int fd, std::string searched_id)
{
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

                    std::cout << "Title of the document: " << file_value << std::endl;
                    std::string tmp = ":FILE NOTICE 1 :Title of the document: " + file_value + "\r\n";
                    send(fd, tmp.c_str(), tmp.length(), 0);
                    return file_value;
                }
            }
        }
        file.close();
    }
    else
    {
        std::cerr << "Failed to open file for reading: " << filename << std::endl;
        std::string tmp = ":FILE NOTICE 1 :Failed to open file for reading: " + filename + "\r\n";
        send(fd, tmp.c_str(), tmp.length(), 0);
    }
    return "";
}

void upload(int fd, std::string &msg)
{
    std::string newPath;
    std::string filename;
     std::string tmp;

    msg.erase(0, 8);
    newPath = path_management(fd, msg, filename);
std::cout << " newPath = path_management = " << newPath << std::endl;
if (newPath.size() == 0) { 
     std::string tmp = ":FILE NOTICE 1 :Enter a Valid Path \r\n";
        send(fd, tmp.c_str(), tmp.length(), 0);
return ;
}
    // oldPath = msg;

    // newPath = rename_if_necessary(filename);

    std::string command = "curl -k --progress-bar --upload-file " + newPath + " https://transfer.sh/" + filename + " 2>&1";
    
    FILE *pipe = popen(command.c_str(), "r");
    if (!pipe)
    {
         tmp = ":FILE NOTICE 1 :popen() failed! \r\n";
        // std::cout << tmp << std::endl;
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

//  send(fd, result.c_str(), result.length(), 0);  

    // Extract the id
    size_t last_pos = result.find_last_of("/");
    size_t second_last_pos = result.find_last_of("/", last_pos - 1);
    // Extract the id between the two /id/
    std::string id = result.substr(second_last_pos + 1, last_pos - second_last_pos - 1);

     tmp = ":FILE NOTICE 1 :ID = " + id + " \r\n";
    // std::cout << tmp << std::endl;
    writeToFile(fd, id, filename);

    tmp += ":FILE NOTICE 1 :File Uploaded Successfully...\r\n";
    send(fd, tmp.c_str(), tmp.length(), 0);
    // std::cout << "ID = " << id << std::endl;
    // std::cout << "File Uploaded Successfully..." << std::endl;
}

void download(int fd, std::string &msg)
{
    std::string ID;
    std::string filename;
    std::string tmp;

    // msg.erase(0, 10);

    // FILE DOWN id
    ID = trimFront(msg, 9);
    // std::cout << "Enter the ID of the document -use LIST cammand to get IDs-\n";
    // std::getline(std::cin >> std::ws, ID);

    filename = readFromFile(fd, ID);
    if (filename.size() == 0)
    {
        tmp =   ":FILE NOTICE 1 :ID not Found\r\n";
        send(fd, tmp.c_str(), tmp.length(), 0);
        return ;
    }

    // Download the file using id
    // std::string command_download = "curl -k https://transfer.sh/" + ID + "/" + filename + " -o ./srcs/bonus/download/downloaded_" + filename + " 2>&1";
    
        std::string cmd ="curl -k https://transfer.sh/" + ID + "/" + filename + " -o ./downloaded_" + filename;

         tmp = ":FILE NOTICE 1 :execute this command on your terminal:\r\n";
         tmp += ":FILE NOTICE 1 :" + cmd  + "\r\n";
        send(fd, tmp.c_str(), tmp.length(), 0);

    // FILE *pipe_download = popen(command_download.c_str(), "r");
    // if (!pipe_download)
    // {

    //     tmp = "popen() failed! \r\n";
    //     std::cout << tmp << std::endl;
    //     send(fd, tmp.c_str(), tmp.length(), 0);
    //     return;
    // }
    // pclose(pipe_download);

    //  tmp = "File Downloaded Successfully..\r\n";
    // std::cout << tmp << std::endl;
    // send(fd, tmp.c_str(), tmp.length(), 0);
}

void exitFunc(int fd, std::string &msg)
{
    (void)msg;
    std::string tmp = ":FILE NOTICE 1 :Thanks For Your Visit...\r\n";
    std::cout << tmp << std::endl;
    send(fd, tmp.c_str(), tmp.length(), 0);
    return;
}

enum command_t
{
    UP = 8,
    DOWN,
    LIST,
    EXIT,
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
        {
            upload(fd, msg);
        }
        else
        {
            tmp = ":FILE NOTICE 1 :Permission Denied For Remote Clients\r\n";
            std::cout << tmp << std::endl;
            send(fd, tmp.c_str(), tmp.length(), 0);
        }
        break;
    case DOWN:
        download(fd, msg);
        break;
    case LIST:
        list(fd, msg);
        break;
    case EXIT:
        exitFunc(fd, msg);
        break;

    default:
        msg = ":FILE NOTICE 1 :Invalid command\r\n";
        send(fd, msg.c_str(), msg.length(), 0);
        break;
    }
}

// client IP = ::1
// client IP = ::ffff:127.0.0.1

// client [127.0.0.1