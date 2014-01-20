#include <iostream>
#include "FactoryInfo.h"

FactoryInfo::FactoryInfo()
{
    m_factory = NULL;
    m_recycler = NULL;

    m_anyParams = true;
    m_params = 0;
    m_useData = false;

    m_objectCount = 0;

}

FactoryInfo::FactoryInfo(std::string type, FactoryInfo::Factory factory, FactoryInfo::Recycler recycler)
{
    m_factoryType = type;
    m_factory = factory;
    m_recycler = recycler;

    m_anyParams = true;
    m_params = 0;
    m_useData = false;

    m_objectCount = 0;

}

FactoryInfo::FactoryInfo(std::string type, FactoryInfo::Factory factory, FactoryInfo::Recycler recycler,
        bool anyParams, uint32_t params, bool useData)
{
    m_factoryType = type;
    m_factory = factory;
    m_recycler = recycler;

    m_anyParams = anyParams;
    m_params = params;
    m_useData = useData;

    m_objectCount = 0;

}

FactoryInfo::~FactoryInfo()
{
    if(m_objectCount != 0)
    {
        std::cout << "/////WARNING/////\n" << "Factory for type: " << m_factoryType << " did not destroy " << m_objectCount << "\n";

    }

}

std::string FactoryInfo::getType()
{
    return m_factoryType;

}

FactoryInfo::Factory FactoryInfo::getFactory()
{
    return m_factory;

}

FactoryInfo::Recycler FactoryInfo::getRecycler()
{
    return m_recycler;

}

bool FactoryInfo::anyNumberOfParams()
{
    return m_anyParams;

}

uint32_t FactoryInfo::numberOfParams()
{
    return m_params;

}

bool FactoryInfo::useData()
{
    return m_useData;

}

void* FactoryInfo::create(std::vector<std::string> params, void* data)
{
    if(m_factory == NULL)
    {
        return NULL;

    }

    void* ptr = m_factory(params, data);
    if(ptr != NULL)
    {
        m_objectCount++;

    }

    return ptr;

}

void FactoryInfo::clean(void* ptr)
{
    if(m_recycler == NULL)
    {
        return;

    }

    if(ptr != NULL)
    {
        m_objectCount--;

    }

    m_recycler(ptr);

}
