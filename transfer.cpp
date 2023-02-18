#include <string>
#include <cstring>
#include <sstream>
#include <fcntl.h>
#include <vector>
#include <list>
#include <map>
#include <unistd.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>

void affich()
{
    // ID=$(echo "$res" | awk -F'/' '{print $4}')
    // printf "\nfile uploaded successfully .\n"
    // echo "File name is : $file_name"
    // echo "Your file ID is : $ID"
}

void upload()
{
    // printf "Where the file is located -path-: "
    // read path
    // printf "Enter the File Name: "
    // read file_name
    // cd $path
    // res=$(curl --progress-bar --upload-file ./$file_name https://transfer.sh/$file_name)
    // ID=$(echo "$res" | awk -F'/' '{print $4}')
    // affich file_name res
    // exit 0
}

void download()
{
    // 	printf "Enter the File Name: "
    // 	read file_name
    // 	printf "Enter the ID : "
    // 	read ID
    // #	user=$(echo ~$username | awk -F'/' '{print $3}')
    // #	cd && cd Desktop
    // 	curl https://transfer.sh/$ID/$file_name -o $file_name
    // 	exit 0
}

int main()
{

    // std::string action;
    // std::cout << "do You want to upload or download a file: \n";
    // std::getline(std::cin >> std::ws, action);

    // if (action.compare("upload") == 0)
    // 	upload();
    // else if (action.compare("download") == 0)
    // 	download();
    // else
    // 	std::cout << "Usage :  tape upload or download \n";

    std::string path;
    std::string filename;
    std::cout << "Enter the filename with path - /path/to/filename -: \n";
    std::getline(std::cin >> std::ws, path);

    size_t pos = path.find_last_of('/');
    if (pos != std::string::npos)
    {
        filename = path.substr(pos + 1);
        std::cout << "filename: " << filename << std::endl;
    }
    else
    {
        std::cerr << "path separator not found in path" << std::endl;
    }

    std::string command = "curl -k --progress-bar --upload-file " + path  + " https://transfer.sh/" + filename + " 2>&1";
    FILE *pipe = popen(command.c_str(), "r");
    if (!pipe)
    {
        std::cerr << "popen() failed!" << std::endl;
        return 1;
    }
    std::string result;
    char buffer[128];
    while (!feof(pipe))
    {
        if (fgets(buffer, 128, pipe) != NULL)
        {
            result += buffer;
        }
    }
    pclose(pipe);

    std::cout << "uploaded: " << result << std::endl;

    // Extract the id
    size_t last_pos = result.find_last_of("/");
    size_t second_last_pos = result.find_last_of("/", last_pos - 1);
    // Extract the id between the two /id/
    std::string id = result.substr(second_last_pos + 1, last_pos - second_last_pos - 1);
    std::cout << "ID = " << id << std::endl;

    // Download the file using id
    std::string command_download = "curl -k https://transfer.sh/" + id + "/" + filename + " -o ./download/downloaded_" + filename + " 2>&1";
    FILE *pipe_download = popen(command_download.c_str(), "r");
    if (!pipe_download)
    {
        std::cerr << "popen() failed!" << std::endl;
        return 1;
    }
    pclose(pipe_download);

    std::cout << "Download completed." << std::endl;

    return 0;
}