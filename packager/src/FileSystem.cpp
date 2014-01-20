#include "FileSystem.h"

#ifdef _WIN32
#include "dirent.h"
#elif __linux__
#include <dirent.h>
#endif

bool getFilesFromPath(std::string path, std::vector<std::string>& files)
{
    DIR* dirp;
    struct dirent* entry;
    std::string entname;

    dirp = opendir((const char* )path.c_str());
    if(dirp == NULL)
        return false;

    entry = readdir(dirp);
    while(entry != NULL)
    {
        entname = entry->d_name;
        if(entname != "." && entname != "..")
        {
            std::string file = path + "/" + entname;
            if(!getFilesFromPath(file, files))
                files.push_back(file);


        }
        entry = readdir(dirp);

    }

    return true;
}

bool isDirectory(std::string path)
{
    DIR* dirp;
    dirp = opendir((const char*)path.c_str());

    if(dirp == NULL)
        return false;

    return true;

}

std::string getExstension(std::string file)
{
    while(file.find("."))
        file = file.substr(file.find(".")+1);

    return file;

}

std::string getFile(std::string path)
{
    while(path.find("/"))
        path = path.substr(path.find("/")+1);

    return path;

}
