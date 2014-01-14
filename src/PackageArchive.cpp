#include "PackageArchive.h"

PackageArchive::PackageArchive()
{

}

PackageArchive::~PackageArchive()
{
    clear();

}

PackageArchive::Error PackageArchive::read(std::string path)
{
    clear();

    std::fstream file;
    file.open(path.c_str(), std::fstream::binary | std::fstream::in);

    if(!file)
    {
        return PackageArchive::CouldNotOpenFile;

    }

    m_path = path;

    std::string header = "pkgarc";
    uint32_t headerSize = header.size();
    char headerBuffer[headerSize+1];
    memset(headerBuffer, 0, headerSize+1);
    file.read(headerBuffer, headerSize);

    if(std::string(headerBuffer) != header)
    {
        return PackageArchive::NotAPackageArchive;

    }

    while(!file.eof())
    {
        std::string nameStr;
        uint32_t nameSize = readUInt(file);
        nameStr = readStr(file, nameSize);

        std::string fileStr;
        uint32_t fileSize = readUInt(file);
        fileStr = readStr(file, fileSize);

        if(nameStr != "" && fileStr != "")
        {
            add(nameStr, fileStr);

        }

    }

    return PackageArchive::NoError;

}

PackageArchive::Error PackageArchive::write(std::string path)
{
    if(!(m_sections.size() > 0))
    {
        return PackageArchive::NothingToWrite;

    }

    std::fstream file;
    file.open(path.c_str(), std::fstream::binary | std::fstream::out | std::fstream::trunc);

    if(!file)
    {
        return PackageArchive::CouldNotOpenFile;

    }

    m_path = path;

    std::string header = "pkgarc";
    uint32_t headerSize = header.size();
    file.write(header.c_str(), headerSize);

    std::list<PackageSection*>::iterator it;
    for(it = m_sections.begin(); it != m_sections.end(); it++)
    {
        writeUInt(file, (*it)->getNameSize());
        writeStr(file, (*it)->getName());

        writeUInt(file, (*it)->getFileSize());
        writeStr(file, (*it)->getFile());

    }

    return PackageArchive::NoError;

}

std::string PackageArchive::getPath()
{
    return m_path;

}

void PackageArchive::add(std::string name, std::string file)
{
    PackageSection* section = new PackageSection(name, file);

    rem(name);
    m_sections.push_back(section);

}

void PackageArchive::rem(std::string name)
{
    std::list<PackageSection*>::iterator it;
    for(it = m_sections.begin(); it != m_sections.end(); it++)
    {
        if((*it)->getName() == name)
        {
            delete (*it);
            m_sections.erase(it);
            return;

        }

    }

}

PackageSection* PackageArchive::getSection(std::string name)
{
    std::list<PackageSection*>::iterator it;
    for(it = m_sections.begin(); it != m_sections.end(); it++)
    {
        if((*it)->getName() == name)
        {
            return (*it);

        }

    }

    return NULL;

}

std::list<PackageSection*> PackageArchive::getSections()
{
    return m_sections;

}

void PackageArchive::set(std::list<PackageSection*> sections)
{
    m_sections = sections;

}

void PackageArchive::clear()
{
    while(!m_sections.empty())
    {
        delete (*m_sections.begin());
        m_sections.erase(m_sections.begin());

    }

}

uint32_t PackageArchive::readUInt(std::fstream& file)
{
    uint32_t num = 0;
    file.read((char*)&num, sizeof(unsigned int));
    num = ntohl(num);

    return num;

}

std::string PackageArchive::readStr(std::fstream& file, uint32_t size)
{
    if(size == 0)
    {
        return "";

    }

    std::string str;

    char buffer[size+1];
    memset(buffer, 0, size+1);
    file.read(buffer, size);
    str = std::string(buffer);

    return str;

}

void PackageArchive::writeUInt(std::fstream& file, uint32_t num)
{
    uint32_t swapped = htonl(num);
    file.write((char*)&swapped, sizeof(uint32_t));

}

void PackageArchive::writeStr(std::fstream& file, std::string str)
{
    if(str == "")
    {
        return;

    }

    file.write(str.c_str(), str.size());

}
