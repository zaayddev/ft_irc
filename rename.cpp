#include <iostream>
#include <cstdio>

int main() {

    std::string oldPath;
    std::string filename;
    std::cout << "Enter the filename with path - /path/to/filename -: \n";
    std::getline(std::cin >> std::ws, oldPath);

std::string newPath = oldPath;

    size_t pos = newPath.find_last_of('/');
    if (pos != std::string::npos)
    {
         for (size_t i = newPath.length() - 1; i >= pos ; i--) 
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

   if (std::rename(oldPath.c_str(), newPath.c_str()) != 0) {
        std::cout << "Error renaming file" << std::endl;
        return 1;
    }
  

    return 0;
}