#include "ScriptPlugin.h"

ScriptPlugin::ScriptPlugin()
{
    m_engine = NULL;
    m_factory = NULL;

}

ScriptPlugin::~ScriptPlugin()
{


}

void ScriptPlugin::init(ScriptEngine* engine)
{
    if(engine == NULL || m_engine != NULL)
    {
        return;

    }

    m_engine = engine;
    m_engine->addRef();

    m_factory = new PropertyFactory;

    m_engine->startGroup(getName());

    onInit();

    m_engine->endGroup();

}

void ScriptPlugin::deinit()
{
    m_engine->releaseGroup(getName());
    m_engine->remRef();
    delete m_factory;

}

bool ScriptPlugin::has(std::string type)
{
    return m_factory->has(type);

}

void* ScriptPlugin::create(std::string type, std::vector<std::string> params, void* data)
{
    return m_factory->create(type, params, data);

}

bool ScriptPlugin::clean(std::string type, void* ptr)
{
    return m_factory->clean(type, ptr);

}
