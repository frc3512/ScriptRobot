#include <fstream>
#include "loadscript.h"

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
            line.substr(line.find("//"));

        if(line.find("#include") < line.size())
        {
            std::string includedFile = line.substr(line.find("\"")+1);
            includedFile = includedFile.substr(line.find("\""));

            line = loadScript(includedFile);

        }

        file += line;

    }

    f.close();
    return file;

}
