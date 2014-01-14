#include "ScriptPlugin.h"

ScriptPlugin::ScriptPlugin()
{
    m_engine = NULL;
    m_factory = NULL;
    factoriesInitialized = false;
    bindingsInitialized = false;

}

ScriptPlugin::~ScriptPlugin()
{
    deinitFactories();
    deinitBindings();

}

void ScriptPlugin::initFactories()
{
    deinitFactories();

    m_factory = new PropertyFactory;

    onInitFactories();

    factoriesInitialized = true;

}

void ScriptPlugin::initBindings(asIScriptEngine* engine)
{
    if(engine == NULL)
    {
        return;

    }

    deinitBindings();
    m_engine = engine;

    m_engine->BeginConfigGroup(getName().c_str());

    onInitBindings();

    m_engine->EndConfigGroup();

    bindingsInitialized = true;

}

void ScriptPlugin::deinitFactories()
{
    deinitBindings();

    if(m_factory == NULL)
    {
        return;

    }

    onDeinitFactories();

    delete m_factory;

    factoriesInitialized = false;

}

void ScriptPlugin::deinitBindings()
{
    if(m_engine == NULL)
    {
        return;

    }

    onDeinitBindings();

    m_engine->RemoveConfigGroup(getName().c_str());
    m_engine = NULL;

    bindingsInitialized = false;

}

bool ScriptPlugin::areFactoriesInitialized()
{
    return factoriesInitialized;

}

bool ScriptPlugin::areBindingsInitialized()
{
    return bindingsInitialized;

}

bool ScriptPlugin::hasType(std::string type)
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
