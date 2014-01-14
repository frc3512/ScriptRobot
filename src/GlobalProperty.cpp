#include "GlobalProperty.h"

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
    if(m_definition == "" && m_typet == "" && m_name == "" && m_ptr == NULL)
    {
        m_definition = definition;
        m_typet = type;
        m_name = name;
        m_ptr = ptr;

    }

}

bool GlobalProperty::registerProperty(asIScriptEngine* engine, bool configGroup)
{
    if(engine == NULL || m_definition == "" || m_typet == "" || m_name == "" || m_ptr == NULL)
    {
        return false;

    }

    m_engine = engine;

    if(configGroup)
    {
        m_engine->BeginConfigGroup(m_name.c_str());

    }

    int error = m_engine->RegisterGlobalProperty((m_typet + " " + m_name).c_str(), m_ptr);

    if(configGroup)
    {
        m_engine->EndConfigGroup();

    }

    if(error < 0)
    {
        release();
        return false;

    }

    return true;

}

void GlobalProperty::release()
{
    if(m_engine == NULL)
    {
        return;

    }

    m_engine->RemoveConfigGroup(m_name.c_str());
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
    return m_typet;

}

std::string GlobalProperty::getName()
{
    return m_name;

}
