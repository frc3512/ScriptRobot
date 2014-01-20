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
    if(m_name != "")
    {
        return;

    }

    m_name = name;
    m_nameSize = m_name.size();

}

std::string PackageSection::getName()
{
    return m_name;

}

void PackageSection::setFile(std::string file)
{
    if(m_file != "")
    {
        return;

    }

    m_file = file;
    m_fileSize = m_file.size();

}

std::string PackageSection::getFile()
{
    return m_file;

}

uint32_t PackageSection::getNameSize()
{
    return m_nameSize;

}

uint32_t PackageSection::getFileSize()
{
    return m_fileSize;

}
