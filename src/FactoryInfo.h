#ifndef FACTORYINFO_H_
#define FACTORYINFO_H_

#include <vector>
#include <string>

#if defined _WIN32 || __linux__
#include <stdint.h>
#elif defined __VXWORKS__
#include <types/vxTypes.h>
#endif

class FactoryInfo
{
public:
    typedef void*(*Factory)(std::vector<std::string>, void*);
    typedef void(*Recycler)(void*);

    FactoryInfo();
    FactoryInfo(std::string type, FactoryInfo::Factory factory, FactoryInfo::Recycler recycler);
    FactoryInfo(std::string type, FactoryInfo::Factory factory, FactoryInfo::Recycler recycler,
            bool anyParams, uint32_t params, bool useData);

    ~FactoryInfo();

    std::string getType();

    FactoryInfo::Factory getFactory();
    FactoryInfo::Recycler getRecycler();

    bool anyNumberOfParams();
    uint32_t numberOfParams();
    bool useData();

    uint32_t getObjectCount();

    void* create(std::vector<std::string> params, void* data);
    void clean(void* ptr);

private:
    std::string m_factoryType;

    FactoryInfo::Factory m_factory;
    FactoryInfo::Recycler m_recycler;

    bool m_anyParams;
    uint32_t m_params;
    bool m_useData;

    uint32_t m_objectCount;

};

#endif /* FACTORYINFO_H_ */
