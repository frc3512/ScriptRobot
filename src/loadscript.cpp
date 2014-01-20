#include <fstream>
#include "loadscript.h"
#include <iostream>

std::string loadScript(std::string path)
{
    std::string file;

    std::fstream f;
    f.open(path.c_str());

    if(!f)
        return "";

    while(!f.eof())
    {
        std::string line;
        std::getline(f, line);

        if(line.find("//") < line.size())
            line = line.substr(0, line.find("//"));

        if(line.find("#include") < line.size())
        {
            path = path.substr(0, path.rfind("/")+1);
            std::string includedFile = line.substr(line.find("\"")+1);
            includedFile = includedFile.substr(0, includedFile.rfind("\""));
            includedFile = path + includedFile;
            line = loadScript(includedFile);

        }

        file += line;

    }

    f.close();
    return file;

}
