#include "ScriptRoutine.h"

ScriptRoutine::ScriptRoutine()
{
    m_hookDecl = "";

}

ScriptRoutine::~ScriptRoutine()
{
    release();

}

void ScriptRoutine::release()
{
    m_scripts.clear();
    while(!m_hooks.empty())
    {
        (*m_hooks.begin())->Release();
        m_hooks.erase(m_hooks.begin());

    }

}

void ScriptRoutine::setup(std::string name, std::string hookDecl)
{
    setName(name);
    setHookDecl(hookDecl);

}

void ScriptRoutine::setScripts(std::vector<std::string> scripts)
{
    m_scripts = scripts;

}

void ScriptRoutine::loadHooksFromEngine(asIScriptEngine* engine)
{
    for(uint32_t i = 0; i < getNumOfScripts(); i++)
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

void ScriptRoutine::setHookDecl(std::string hookDecl)
{
    if(m_hookDecl != "")
    {
        return;

    }

    m_hookDecl = hookDecl;

}

std::string ScriptRoutine::getHookDecl()
{
    return m_hookDecl;

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
    for(uint32_t i = 0; i < m_scripts.size(); i++)
    {
        if(m_scripts[i] == script)
        {
            return true;

        }

    }

    return false;

}

uint32_t ScriptRoutine::getNumOfScripts()
{
    return m_scripts.size();

}

std::string ScriptRoutine::getScript(uint32_t i)
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

uint32_t ScriptRoutine::getNumOfHooks()
{
    return m_hooks.size();

}

asIScriptFunction* ScriptRoutine::getHook(uint32_t i)
{
    if(i >= getNumOfHooks())
    {
        return NULL;

    }

    std::list<asIScriptFunction*>::iterator it;
    it = m_hooks.begin();
    for(uint32_t u = 0; u < i; u++)
    {
        it++;

    }

    return *it;

}
