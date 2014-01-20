#include "GlobalProperty.h"
#include <iostream>

GlobalProperty::GlobalProperty()
{
    m_engine = NULL;
    m_ptr = NULL;

}

GlobalProperty::~GlobalProperty()
{
    release();
    m_ptr = NULL;

}

void GlobalProperty::setup(std::string definition, std::string type, std::string name, void* ptr)
{
    if(m_definition == "" && m_propertyType == "" && m_name == "" && m_ptr == NULL)
    {
        m_definition = definition;
        m_propertyType = type;
        m_name = name;
        m_ptr = ptr;

    }

}

bool GlobalProperty::registerProperty(ScriptEngine* engine)
{
    //check if all the values have been setup and the property is ready to be registered.
    if(engine == NULL || m_definition == "" || m_propertyType == "" || m_name == "" || m_ptr == NULL)
    {
        return false;

    }

    m_engine = engine;
    m_engine->addRef();

    if(m_engine->startGroup(m_name) < 0)
    {
        m_engine->endGroup();
        release();
        return false;

    }


    int error = m_engine->get()->RegisterGlobalProperty((m_propertyType + " " + m_name).c_str(), m_ptr);

    if(error < 0)
    {
        m_engine->endGroup();
        release();
        return false;

    }

    m_engine->endGroup();
    return true;

}

void GlobalProperty::release()
{
    if(m_engine == NULL)
    {
        return;

    }

    int32_t error = m_engine->releaseGroup(m_name);
    if(error < 0)
    {
        std::cout << "could not remove config group for: " << m_name << " error: " << error << "\n";

    }

    m_engine->remRef();
    m_engine = NULL;

}

void* GlobalProperty::getPtr()
{
    return m_ptr;

}

std::string GlobalProperty::getDefinition()
{
    return m_definition;

}

std::string GlobalProperty::getType()
{
    return m_propertyType;

}

std::string GlobalProperty::getName()
{
    return m_name;

}
