#include "ScriptEngine.h"
#include "GlobalProperty.h"
#include "ScriptPlugin.h"

ScriptEngine::ScriptEngine()
{
    m_engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
    m_engine->AddRef();

}

ScriptEngine::~ScriptEngine()
{
    m_engine->Release();

}

asIScriptEngine* ScriptEngine::get()
{
    return m_engine;

}

int32_t ScriptEngine::startGroup(std::string name)
{
    if(m_groups.size() != 0)
    {
        m_engine->EndConfigGroup();

    }

    int32_t error = m_engine->BeginConfigGroup(name.c_str());
    m_groups.push(name.c_str());

    return error;

}

void ScriptEngine::endGroup()
{
    if(m_groups.size() == 0)
    {
        return;

    }

    m_engine->EndConfigGroup();
    m_groups.pop();

    if(m_groups.size() == 0)
    {
        return;

    }

    std::string nextGroup = m_groups.top();
    m_engine->BeginConfigGroup(nextGroup.c_str());

}

int32_t ScriptEngine::releaseGroup(std::string name)
{
    return m_engine->RemoveConfigGroup(name.c_str());

}

void ScriptEngine::addPlugin(ScriptPlugin* plugin)
{
    plugin->init(this);
    m_plugins.push_front(plugin);

}

void ScriptEngine::remPlugin()
{
    (*m_plugins.begin())->deinit();
    delete (*m_plugins.begin());
    m_plugins.erase(m_plugins.begin());

}

uint32_t ScriptEngine::plugins()
{
    return m_plugins.size();

}

GlobalProperty* ScriptEngine::addProperty(std::string definition, std::string type,
        std::string name, std::vector<std::string> params, void* data)
{
    std::list<ScriptPlugin*>::iterator it;
    void* ptr = NULL;

    for(it = m_plugins.begin(); it != m_plugins.end(); it++)
    {
        if((*it)->has(type))
        {
            ptr = (*it)->create(type, params, data);
            if(ptr != NULL)
            {
                break;

            }

        }

    }

    if(ptr != NULL)
    {
        return addProperty(definition, type, name, ptr);

    }

    return NULL;

}

GlobalProperty* ScriptEngine::addProperty(std::string definition, std::string type,
        std::string name, void* ptr)
{
    GlobalProperty* temp = new GlobalProperty;
    temp->setup(definition, type, name, ptr);

    if(!temp->registerProperty(this))
    {
        cleanProperty(temp);
        return NULL;

    }

    return temp;

}

void ScriptEngine::cleanProperty(GlobalProperty* property)
{
    if(property == NULL)
    {
        return;

    }

    property->release();
    std::list<ScriptPlugin*>::iterator it;
    for(it = m_plugins.begin(); it != m_plugins.end(); it++)
    {
        if((*it)->has(property->getType()))
        {
            if((*it)->clean(property->getType(), property->getPtr()))
            {
                break;

            }

        }

    }

    delete property;
    property = NULL;

}
