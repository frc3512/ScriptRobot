#ifndef PACKAGEARCHIVE_H_
#define PACKAGEARCHIVE_H_

#include <list>
#include <string>
#include <string.h>
#include <fstream>

#include "PackageSection.h"

#ifdef _WIN32
#include <winsock2.h>
#include <stdint.h>
#elif __linux__
#include <netinet/in.h>
#include <stdint.h>
#elif __VXWORKS__
#include <netinet/in.h>
#include <types/vxTypes.h>
#endif

class PackageArchive
{
public:
    PackageArchive();

    ~PackageArchive();

    typedef enum Error
    {
        NoError = 0,
        CouldNotOpenFile = -1,
        NotAPackageArchive = -2,
        NothingToWrite = -3

    } Error;

    PackageArchive::Error read(std::string path);
    PackageArchive::Error write(std::string path);

    std::string getPath();

    void add(std::string name, std::string file);

    void rem(std::string name);

    PackageSection getSection(std::string name);

    std::list<PackageSection> getSections();

    void set(std::list<PackageSection> sections);

    void clear();

private:
    uint32_t readUInt(std::fstream& file);
    std::string readStr(std::fstream& file, uint32_t size);

    void writeUInt(std::fstream& file, uint32_t num);
    void writeStr(std::fstream& file, std::string str);

    std::string m_path;

    std::list<PackageSection> m_sections;

};

#endif /* PACKAGEARCHIVE_H_ */
