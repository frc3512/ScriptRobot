#include "Config.h"
#include <sstream>
#include "loadscript.h"
#include "Convert/convertstring.h"
#include <iostream>

std::string Config::printParseError(ParseError error)
{
    std::string message = "Not a real parse error.";
    switch(error)
    {
    case NoError:
        message = "No error while parsing config.";
        break;
    case NoFile:
        message = "The file was empty, could not parse.";
        break;
    case NoType:
        message = "A Variable is missing it's type.";
        break;
    case NoVarName:
        message = "A section is missing a variable name";
        break;
    case BadValue:
        message = "Could not find value.";
        break;
    case VarNameCollision:
        message = "Two variables have the same name could not build.";
        break;
    }

    return message;

}


Config::Config()
{

}

Config::~Config()
{
    clearSections();

}

Config::ParseError Config::load(std::string path)
{
    std::string file;
    file = loadScript(path);
    return parse(file);

}

Config::ParseError Config::parse(std::string file)
{
    configStr = file;
    clearSections();

    if(configStr == "")
        return NoFile;

    std::istringstream f(configStr);

    unsigned int linenum = 0;
    while(!f.eof())
    {
        //gets the next line in the config
        std::string line;
        std::getline(f, line);

        //splits up sections by ; i.e) section1; section2; section3;
        while(line.find(";") < line.size())
        {
            //gets the next available section as a string
            std::string section = line.substr(0, line.find(";"));
            //moves the current line passed the current section
            line = line.substr(line.find(";")+1);

            //removes spaces before the sections first none space
            while(section.find(" ") == 0)
                section = section.substr(1);

            //removes spaces after the sections last none space character
            while(section.substr(section.size()-1) == " ")
                section = section.substr(0, section.size()-1);

            //finds the indentifier or type of the variable
            std::string identifier = section;
            while(identifier.find(" ") > 0 && identifier.find(" ") < identifier.size())
                identifier = identifier.substr(0, identifier.size()-1);

            //finds the variable name
            std::string key = section.substr(identifier.size());
            while(key.find(" ") == 0)
                key = key.substr(1);

            while(key.find(" ") > 0 && key.find(" ") < key.size())
                key = key.substr(0, key.size()-1);

            if(section.find("=") > section.size())
                return BadValue;

            //gets the value of the variable
            std::string value_str = section.substr(section.find("=")+1);
            while(value_str.find(" ") == 0)
                value_str = value_str.substr(1);

            SectionData data;
            data.value = NULL;
            data.section = section;
            data.line = linenum;
            data.identifier = identifier;
            data.key = key;

            //parses the parameteres of the variable by checking for ,
            std::string param_str = value_str;
            while(param_str.find(",") < param_str.size())
            {
                std::string param = param_str.substr(0, param_str.find(","));
                param_str = param_str.substr(param_str.find(",")+1);

                while(param.find(" ") == 0)
                    param = param.substr(1);

                //removes spaces after
                while(param.substr(param.size()-1) == " ")
                    param = param.substr(0, param.size()-1);

                if(param.find("\"") == 0)
                    param = param.substr(1);

                //need to handle escaped quotes
                if(param.find("\"") == param.size()-1)
                    param = param.substr(0, param.size()-1);

                if(param == "")
                    continue;

                data.params.push_back(param);

            }

            //removes spaces and stuff
            while(param_str.find(" ") == 0)
                param_str = param_str.substr(1);

            //gets strings inbetween quotes
            if(param_str.find("\"") == 0)
                param_str = param_str.substr(1);

            if(param_str.find("\"") == param_str.size()-1)
                param_str = param_str.substr(0, param_str.size()-1);

            data.params.push_back(param_str);

            if(value_str.find("\"") == 0)
                value_str = value_str.substr(1);

            //need to handle escaped quotes
            if(value_str.find("\"") == value_str.size()-1)
                value_str = value_str.substr(0, value_str.size()-1);

            data.value_str = value_str;

            if(key == "")
                return NoVarName;

            if(identifier == "")
                return NoType;

            if(value_str == "")
                return BadValue;

            if(getSectionData(key) != NULL)
                return VarNameCollision;

            sections.push_back(data);

            linenum++;

        }

    }

    std::cout << "finished parsing .. about to build\n";

    build();
    return NoError;

}

void Config::build()
{
    for(unsigned int i = 0; i < sections.size(); i++)
    {
        if(sections[i].identifier == "int")
        {
            int v = toInt(sections[i].value_str);
            sections[i].value = new int(v);

        }
        else if(sections[i].identifier == "uint")
        {
            unsigned int v = toUInt(sections[i].value_str);
            sections[i].value = new unsigned int(v);

        }
        else if(sections[i].identifier == "float")
        {
            float v = toFloat(sections[i].value_str);
            sections[i].value = new float(v);

        }
        else if(sections[i].identifier == "string")
        {
            sections[i].value = new std::string(sections[i].value_str);

        }
        else if(sections[i].identifier == "bool")
        {
            bool v = toBool(sections[i].value_str);
            sections[i].value = new bool(v);

        }
        else if(sections[i].identifier == "int@")
        {
            std::vector<int> vec;
            for(unsigned int u = 0; u < sections[i].params.size(); u++)
            {
                vec.push_back(toInt(sections[i].params[u]));

            }
            sections[i].value = new std::vector<int>(vec);

        }
        else if(sections[i].identifier == "uint@")
        {
            std::vector<unsigned int> vec;
            for(unsigned int u = 0; u < sections[i].params.size(); u++)
            {
                vec.push_back(toUInt(sections[i].params[u]));

            }
            sections[i].value = new std::vector<unsigned int>(vec);

        }
        else if(sections[i].identifier == "float@")
        {
            std::vector<float> vec;
            for(unsigned int u = 0; u < sections[i].params.size(); u++)
            {
                vec.push_back(toFloat(sections[i].params[u]));

            }
            sections[i].value = new std::vector<float>(vec);
        }
        else if(sections[i].identifier == "string@")
        {
            std::vector<std::string> vec;
            for(unsigned int u = 0; u < sections[i].params.size(); u++)
            {
                vec.push_back(sections[i].params[u]);

            }
            sections[i].value = new std::vector<std::string>(vec);

        }
        else if(sections[i].identifier == "bool@")
        {
            std::vector<bool> vec;
            for(unsigned int u = 0; u < sections[i].params.size(); u++)
            {
                vec.push_back(toBool(sections[i].params[u]));

            }
            sections[i].value = new std::vector<bool>(vec);

        }

    }

}

std::string Config::write()
{
    std::string file;
    for(unsigned int i = 0; i < sections.size(); i++)
    {
        std::string section;
        section += sections[i].identifier;
        section += " " + sections[i].key;
        section += " " + sections[i].value_str;
        section += ";";
        file += section + "\n";

    }
    return file;

}

void Config::clearSections()
{
    while(!sections.empty())
    {
        if(sections.begin()->value != NULL)
        {
            std::string type = sections.begin()->identifier;
            if(type == "int")
            {
                delete (int*)sections.begin()->value;

            }
            else if(type == "@int")
            {
                delete (std::vector<int>*)sections.begin()->value;

            }
            else if(type == "uint")
            {
                delete (unsigned int*)sections.begin()->value;

            }
            else if(type == "@uint")
            {
                delete (std::vector<unsigned int>*)sections.begin()->value;

            }
            else if(type == "float")
            {
                delete (float*)sections.begin()->value;

            }
            else if(type == "@float")
            {
                delete (std::vector<float>*)sections.begin()->value;

            }
            else if(type == "string")
            {
                delete (std::string*)sections.begin()->value;

            }
            else if(type == "@string")
            {
                delete (std::vector<std::string>*)sections.begin()->value;

            }
            else if(type == "bool")
            {
                delete (bool*)sections.begin()->value;

            }
            else if(type == "@bool")
            {
                delete (std::vector<bool>*)sections.begin()->value;

            }
            else
            {
                ///YAY MEMORY LEAK

            }

        }

        sections.erase(sections.begin());

    }

}

void* Config::lookup(std::string name)
{
    SectionData* data = getSectionData(name);
    if(data == NULL)
        return NULL;
    return data->value;

}

//GETTERS
int Config::getInt(std::string name)
{
    if(lookup(name) == NULL)
        return 0;
    return *(int*)lookup(name);

}

unsigned int Config::getUInt(std::string name)
{
    if(lookup(name) == NULL)
        return 0;
    return *(unsigned int*)lookup(name);

}

float Config::getFloat(std::string name)
{
    if(lookup(name) == NULL)
        return 0;
    return *(float*)lookup(name);

}

std::string Config::getString(std::string name)
{
    if(lookup(name) == NULL)
        return "";
    return *(std::string*)lookup(name);

}

bool Config::getBool(std::string name)
{
    if(lookup(name) == NULL)
        return false;
    return *(bool*)lookup(name);

}

std::vector<int> Config::getIntVec(std::string name)
{
    if(lookup(name) == NULL)
    {
        std::vector<int> t;
        return t;

    }

    return *(std::vector<int>*)lookup(name);

}

std::vector<unsigned int> Config::getUIntVec(std::string name)
{
    if(lookup(name) == NULL)
    {
        std::vector<unsigned int> t;
        return t;

    }

    return *(std::vector<unsigned int>*)lookup(name);

}

std::vector<float> Config::getFloatVec(std::string name)
{
    if(lookup(name) == NULL)
    {
        std::vector<float> t;
        return t;

    }

    return *(std::vector<float>*)lookup(name);

}

std::vector<std::string> Config::getStringVec(std::string name)
{
    if(lookup(name) == NULL)
    {
        std::vector<std::string> t;
        return t;

    }

    return *(std::vector<std::string>*)lookup(name);

}

std::vector<bool> Config::getBoolVec(std::string name)
{
    if(lookup(name) == NULL)
    {
        std::vector<bool> t;
        return t;

    }

    return *(std::vector<bool>*)lookup(name);

}

//SETTERS
void Config::set(std::string name, std::string type, std::string value_str, void* value)
{
    std::string section = type + " " + name + " = " + value_str + ";";
    std::vector<std::string> params;
    params.push_back(value_str);

    SectionData* data = getSectionData(name);

    if(data == NULL)
    {
        data = new SectionData;
        data->identifier = type;
        data->key = name;
        data->line = 0;
        data->params = params;
        data->section = section;
        data->value = value;
        data->value_str = value_str;
        sections.push_back(*data);

    }
    else
    {
        data->params = params;
        data->section = section;
        data->value = value;
        data->value_str = value_str;

    }

}

void Config::setInt(std::string name, int v)
{
    std::string value_str = toString(v);
    set(name, "int", value_str, new int(v));

}

void Config::setUInt(std::string name, unsigned int v)
{
    std::string value_str = toString(v);
    set(name, "uint", value_str, new unsigned int(v));

}

void Config::setFloat(std::string name, float v)
{
    std::string value_str = toString(v);
    set(name, "float", value_str, new float(v));

}

void Config::setString(std::string name, std::string v)
{
    std::string value_str = v;
    set(name, "string", value_str, new std::string(v));

}

void Config::setBool(std::string name, bool v)
{
    std::string value_str = toString(v);
    set(name, "int", value_str, new bool(v));
}

std::vector<std::string> Config::getParams(std::string name)
{
    SectionData* data = getSectionData(name);
    if(data == NULL)
    {
        std::vector<std::string> t;
        return t;

    }

    return data->params;

}

SectionData* Config::getSectionData(std::string name)
{
    for(unsigned int i = 0; i < sections.size(); i++)
        if(sections[i].key == name)
            return &sections[i];

    return NULL;

}

std::vector<std::string> Config::getNamesOfType(std::string type)
{
    std::vector<std::string> keys;
    for(unsigned int i = 0; i < sections.size(); i++)
        if(sections[i].identifier == type)
            keys.push_back(sections[i].key);

    return keys;

}
