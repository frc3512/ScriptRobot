#ifndef PACKAGESECTION_H_
#define PACKAGESECTION_H_

#include <string>

#if defined _WIN32 || __linux__
#include <stdint.h>
#elif defined __VXWORKS__
#include <types/vxTypes.h>
#endif

class PackageSection
{
public:
    PackageSection();

    PackageSection(std::string name, std::string file);

    ~PackageSection();

    void setup(std::string name, std::string file);

    void setName(std::string name);

    std::string getName();

    void setFile(std::string file);

    std::string getFile();

    uint32_t getNameSize();

    uint32_t getFileSize();

private:
    uint32_t m_nameSize;
    std::string m_name;
    uint32_t m_fileSize;
    std::string m_file;

};

#endif /* PACKAGESECTION_H_ */
