// #include "Includes/Ircserv.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <string>
#include <cstring>
#include <sstream>
#include <iostream>
#include <fcntl.h>
#include <vector>
#include <list>
#include <map>
#include <unistd.h>
#include <cstdlib>
#include <fstream>
#include <arpa/inet.h>
#include <cstdio>

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
            if (pos != std::string::npos)
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

    // std::string key = "new_key";
    // std::string value = "new_value";

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

std::string readFromFile(std::string searhced_id)
{

    const std::string filename = "./download/database";
    std::ifstream file(filename.c_str());
    // std::string searhced_key = "keyTen";
    if (file.is_open())
    {
        std::string line;
        while (std::getline(file, line))
        {
            //  std::cout << line << std::endl;

            std::size_t pos = line.find(":");
            if (pos != std::string::npos)
            {
                std::string file_id = line.substr(0, pos);
                // std::string value = line.substr(pos + 1);
                // std::cout << key << "  |  " << value << std::endl;

                if (file_id == searhced_id)
                {
                    std::string file_value = line.substr(pos + 1);

                    std::cout << "found value : " << file_value << std::endl;

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

void upload()
{
    std::string oldPath;
    std::string filename;
    std::cout << "Enter the filename with path - /path/to/filename -: \n";
    std::getline(std::cin >> std::ws, oldPath);

    std::string newPath = oldPath;

    size_t pos = newPath.find_last_of('/');
    if (pos != std::string::npos)
    {
        for (size_t i = newPath.length() - 1; i >= pos; i--)
        {
            if (newPath[i] == ' ' || newPath[i] == '(' || newPath[i] == ')')
            {
                newPath[i] = '_';
            }
        }
        filename = newPath.substr(pos + 1);
        std::cout << "filename: " << filename << std::endl;
    }
    else
    {
        std::cerr << "path separator not found in path" << std::endl;
    }

    if (std::rename(oldPath.c_str(), newPath.c_str()) != 0)
    {
        std::cout << "Error renaming file" << std::endl;
        return ;
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

    std::cout << "File Uploaded Successfully : " << result << std::endl;

    // Extract the id
    size_t last_pos = result.find_last_of("/");
    size_t second_last_pos = result.find_last_of("/", last_pos - 1);
    // Extract the id between the two /id/
    std::string id = result.substr(second_last_pos + 1, last_pos - second_last_pos - 1);
    std::cout << "ID = " << id << std::endl;

    // map.insert(std::make_pair(id, filename));
    writeToFile(id, filename);
}

void download()
{
    std::string ID;
    std::string filename;
    std::cout << "Enter the ID of the document -use LIST cammand to get IDs-\n";
    std::getline(std::cin >> std::ws, ID);

    // std::map<std::string, std::string>::iterator it = map.find(ID);

    // if (it != map.end())
    // {
    //     std::cout << "filename for ID " << ID << " is " << it->second << std::endl;
    //     filename = it->second;
    // }
    // else
    // {
    //     std::cout << "ID : " << ID << " not found" << std::endl;
    //     return;
    // }

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

int main()
{

    // readFromFile();
    // writeToFile("");

    std::string action;

    std::cout << "input [UP / DOWN / LIST]: \n";
    std::getline(std::cin >> std::ws, action);

    if (action.compare("UP") == 0)
    {
        upload();
        // list(map);
    }
    else if (action.compare("DOWN") == 0)
        download();
    else if (action.compare("LIST") == 0)
        list();

    return 0;
}


// Alex_Charalabidis_-_The_Book_of_IRC-No_Starch_Press__Publishers_Group_West_[distributor]__2000_.pdf
// /Volumes/King28/usb/Alex Charalabidis - The Book of IRC-No Starch Press_ Publishers Group West [distributor] (2000).pdf

// Alex Charalabidis - The Book of IRC-No Starch Press_ Publishers Group West [distributor] (2000).txt

// 33  !    34  "    35  #    36  $    37  %    38  &    39  '
//       40  (    41  )    42  *    43  +    44  ,    45  -

//       58  :    59  ;    60  <    61  =    62  >    63  ?
//       64  @

// 91  [    92  \    93  ]    94  ^    95  _
//       96  `
// 123  {   124  |   125  }   126  ~



