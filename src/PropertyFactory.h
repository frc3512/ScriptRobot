#ifndef PROPERTYFACTORY_H_
#define PROPERTYFACTORY_H_

#include <list>

#include "FactoryInfo.h"

class PropertyFactory
{
public:
    PropertyFactory(bool destroyFactories = true);

    ~PropertyFactory();

    bool destroyFactories();
    void setDestroyFactories(bool destroyFactories);

    void add(FactoryInfo* info);

    bool has(std::string type);

    void* create(std::string type, std::vector<std::string> params, void* data);
    bool clean(std::string type, void* ptr);

private:
    std::list<FactoryInfo*> m_factories;
    bool m_destroyFactories;

};

#endif /* PROPERTYFACTORY_H_ */
