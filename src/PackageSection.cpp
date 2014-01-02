#include "PackageSection.h"

PackageSection::PackageSection()
{
    m_nameSize = 0;
    m_fileSize = 0;

}

PackageSection::PackageSection(std::string name, std::string file)
{
    setup(name, file);

}

PackageSection::~PackageSection()
{

}

void PackageSection::setup(std::string name, std::string file)
{
    setName(name);
    setFile(file);

}

void PackageSection::setName(std::string name)
{
    if(name != "")
    {
        return;

    }

    m_name = name;
    m_nameSize = m_name.size()*sizeof(char*);

}

std::string PackageSection::getName()
{
    return m_name;

}

void PackageSection::setFile(std::string file)
{
    if(file != "")
    {
        return;

    }

    m_file = file;
    m_fileSize = m_file.size()*sizeof(char*);

}

std::string PackageSection::getFile()
{
    return m_file;

}

int* PackageSection::getNameSize()
{
    return &m_nameSize;

}

int* PackageSection::getFileSize()
{
    return &m_fileSize;

}
