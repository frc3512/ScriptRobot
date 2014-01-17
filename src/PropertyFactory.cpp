#include "PropertyFactory.h"

PropertyFactory::PropertyFactory(bool destroyFactories)
{
    m_destroyFactories = destroyFactories;

}

PropertyFactory::~PropertyFactory()
{
    if(m_destroyFactories)
    {
        while(!m_factories.empty())
        {
            delete (FactoryInfo*)(*m_factories.begin());
            m_factories.erase(m_factories.begin());

        }

    }

}

bool PropertyFactory::destroyFactories()
{
    return m_destroyFactories;

}

void PropertyFactory::setDestroyFactories(bool destroyFactories)
{
    m_destroyFactories = destroyFactories;

}

void PropertyFactory::add(FactoryInfo* info)
{
    m_factories.push_back(info);

}

bool PropertyFactory::has(std::string type)
{
    std::list<FactoryInfo*>::iterator it;
    for(it = m_factories.begin(); it != m_factories.end(); it++)
    {
        if((*it)->getType() == type)
        {
            return true;

        }

    }

    return false;

}

void* PropertyFactory::create(std::string type, std::vector<std::string> params, void* data)
{
    bool hasData = false;
    if(data != NULL)
    {
        hasData = true;

    }

    std::list<FactoryInfo*>::iterator it;
    for(it = m_factories.begin(); it != m_factories.end(); it++)
    {
        FactoryInfo* info = *it;
        if(info->getType() == type)
        {
            if(info->anyNumberOfParams() || info->numberOfParams() == params.size())
            {
                if(!info->useData() || (info->useData() && hasData))
                {
                    return info->create(params, data);

                }

            }

        }

    }

    return NULL;

}

bool PropertyFactory::clean(std::string type, void* ptr)
{
    std::list<FactoryInfo*>::iterator it;
    for(it = m_factories.begin(); it != m_factories.end(); it++)
    {
        if((*it)->getType() == type)
        {
            (*it)->clean(ptr);
            if(ptr == NULL)
            {
                return false;

            }
            else
            {
                return true;

            }

        }

    }

    return false;

}
