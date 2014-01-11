#include "ScriptRoutine.h"

ScriptRoutine::ScriptRoutine()
{
    m_type = ScriptRoutine::None;

}

ScriptRoutine::~ScriptRoutine()
{
    release();

}

void ScriptRoutine::release()
{
    while(!m_hooks.empty())
    {
        (*m_hooks.begin())->Release();
        m_hooks.erase(m_hooks.begin());

    }

}

void ScriptRoutine::setup(std::string name, ScriptRoutine::Type type)
{
    setName(name);
    setType(type);

}

void ScriptRoutine::setScripts(std::vector<std::string> scripts)
{
    m_scripts = scripts;

}

void ScriptRoutine::loadHooksFromEngine(asIScriptEngine* engine)
{
    for(unsigned int i = 0; i < getNumOfScripts(); i++)
    {
        std::string script = getScript(i);
        asIScriptModule* module = engine->GetModule(script.c_str());
        if(module == NULL)
        {
            continue;

        }

        addHookFromModule(module);

    }

}

void ScriptRoutine::setName(std::string name)
{
    if(m_name != "")
    {
        return;

    }

    m_name = name;

}

std::string ScriptRoutine::getName()
{
    return m_name;

}

void ScriptRoutine::setType(ScriptRoutine::Type type)
{
    if(m_type != ScriptRoutine::None)
    {
        return;

    }

    m_type = type;

}

ScriptRoutine::Type ScriptRoutine::getType()
{
    return m_type;

}

void ScriptRoutine::addScript(std::string script)
{
    if(hasScript(script))
    {
        return;

    }

    m_scripts.push_back(script);

}

void ScriptRoutine::remScript(std::string script)
{
    std::vector<std::string>::iterator it;
    for(it = m_scripts.begin(); it != m_scripts.end(); it++)
    {
        if((*it) == script)
        {
            m_scripts.erase(it);
            return;

        }

    }

}

bool ScriptRoutine::hasScript(std::string script)
{
    for(unsigned int i = 0; i < m_scripts.size(); i++)
    {
        if(m_scripts[i] == script)
        {
            return true;

        }

    }

    return false;

}

unsigned int ScriptRoutine::getNumOfScripts()
{
    return m_scripts.size();

}

std::string ScriptRoutine::getScript(unsigned int i)
{
    if(i >= getNumOfScripts())
    {
        return "";

    }

    return m_scripts[i];

}

std::vector<std::string> ScriptRoutine::getScripts()
{
    return m_scripts;

}

bool ScriptRoutine::addHookFromModule(asIScriptModule* module)
{
    if(module == NULL)
    {
        return false;

    }

    std::string script = module->GetName();
    if(hasScript(script))
    {
        asIScriptFunction* hook = module->GetFunctionByDecl(getHookDecl().c_str());
        if(hook == NULL)
        {
            return false;

        }

        hook->AddRef();
        m_hooks.push_back(hook);
        return true;

    }

    return false;

}

unsigned int ScriptRoutine::getNumOfHooks()
{
    return m_hooks.size();

}

asIScriptFunction* ScriptRoutine::getHook(unsigned int i)
{
    if(i >= getNumOfHooks())
    {
        return NULL;

    }

    std::list<asIScriptFunction*>::iterator it;
    it = m_hooks.begin();
    for(unsigned int u = 0; u < i; u++)
    {
        it++;

    }

    return *it;

}

std::string ScriptRoutine::getHookDecl()
{
    if(m_type == ScriptRoutine::Init)
    {
        return "void onInit(Robot@)";

    }
    else if(m_type == ScriptRoutine::Disabled)
    {
        return "void onDisabled(Robot@)";

    }
    else if(m_type == ScriptRoutine::Autonomous)
    {
        return "void onAutonomous(Robot@)";

    }
    else if(m_type == ScriptRoutine::OperatorControl)
    {
        return "void onOperatorControl(Robot@)";

    }
    else if(m_type == ScriptRoutine::Test)
    {
        return "void onTest(Robot@)";

    }

    return "";

}
