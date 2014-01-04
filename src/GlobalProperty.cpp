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
    if(m_definition == "" && m_type == "" && m_name == "" && m_ptr == NULL)
    {
        m_definition = definition;
        m_type = type;
        m_name = name;
        m_ptr = ptr;

    }

}

bool GlobalProperty::registerProperty(asIScriptEngine* engine)
{
    if(engine == NULL || m_definition == "" || m_type == "" || m_name == "" || m_ptr == NULL)
    {
        return false;

    }

    m_engine = engine;
    m_engine->BeginConfigGroup(m_name.c_str());

    int error = m_engine->RegisterGlobalProperty((m_type + " " + m_name).c_str(), m_ptr);

    m_engine->EndConfigGroup();

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
    return m_type;

}

std::string GlobalProperty::getName()
{
    return m_name;

}
