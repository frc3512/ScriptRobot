#include "ScriptPackage.h"
#include "ScriptRoutine.h"
#include <iostream>
#include "Config.h"

ScriptPackage::ScriptPackage()
{
    setError(NotLoaded);

    m_engine = NULL;

}

ScriptPackage::~ScriptPackage()
{
    unload();

}

bool ScriptPackage::isValid()
{
    return getError() == ScriptPackage::NoError;

}

ScriptPackage::Error ScriptPackage::getError()
{
   return m_lastError;

}

ScriptPackage::Error ScriptPackage::load(std::string path)
{
    PackageArchive::Error error = m_archive.read(path);
    if(error < 0)
    {
        setError(ScriptPackage::ReadError);
        return getError();

    }

    setError(ScriptPackage::NotBuilt);
    return getError();

}

std::string ScriptPackage::getPath()
{
    return m_archive.getPath();

}

ScriptPackage::Error ScriptPackage::build(ScriptEngine* engine)
{
    if(engine == NULL)
    {
        setError(ScriptPackage::NullEngine);
        return getError();

    }

    m_engine = engine;
    m_engine->addRef();

    {
        std::list<PackageSection> sections = m_archive.getSections();
        std::list<PackageSection>::iterator it;

        m_initRoutine.setup("init", "void onInit(Robot@)");

        for(it = sections.begin(); it != sections.end(); it++)
        {
            if(ScriptPackage::isScript(it->getName()))
            {
                m_initRoutine.addScript(it->getName());

            }
            else if(isConfig(it->getName()))
            {
                Config cfg;
                Config::ParseError error = cfg.parse(it->getFile());
                if(error != Config::NoError)
                {
                    unload();
                    setError(ScriptPackage::ConfigError);
                    return getError();

                }

                for(unsigned int i = 0; i < cfg.sections.size(); i++)
                {
                    std::string name = cfg.sections[i].key;
                    std::string type = cfg.sections[i].identifier;
                    std::vector<std::string> params = cfg.sections[i].params;
                    std::string definition = cfg.sections[i].section + ";";

                    if(type == "Routine")
                    {
                        addRoutine(name, params);

                    }
                    else
                    {
                        GlobalProperty* property = m_engine->addProperty(definition, type, name, params, this);
                        if(property != NULL)
                        {
                            std::cout << "added property: " << property->getName() << "\n";
                            m_properties.push_back(property);

                        }
                        else
                        {
                            unload();
                            setError(ScriptPackage::ConfigError);
                            return getError();

                        }

                    }

                }

            }

        }

        for(it = sections.begin(); it != sections.end(); it++)
        {
            PackageSection section = *it;
            if(ScriptPackage::isScript(section.getName()))
            {
                asIScriptModule* module = m_engine->get()->GetModule(section.getName().c_str());
                if(module != NULL)
                {
                    module->Discard();

                }

                module = m_engine->get()->GetModule(section.getName().c_str(), asGM_ALWAYS_CREATE);
                if(module->AddScriptSection(section.getName().c_str(), section.getFile().c_str()) < 0)
                {
                    unload();
                    setError(ScriptPackage::FailedBuild);
                    return getError();

                }

                if(module->Build() < 0)
                {
                    unload();
                    setError(ScriptPackage::FailedBuild);
                    return getError();

                }

            }

        }
    }

    {
        std::list<ScriptRoutine>::iterator it;
        for(it = m_routines.begin(); it != m_routines.end(); it++)
        {
            it->loadHooksFromEngine(m_engine->get());

        }

    }

    setError(ScriptPackage::NoError);
    return getError();

}

bool ScriptPackage::isScript(std::string name)
{
    return name.rfind(".as") == name.size() - 3;

}

bool ScriptPackage::isConfig(std::string name)
{
    return name.rfind(".cfg") == name.size() - 4;

}

void ScriptPackage::unload()
{
    if(m_engine == NULL)
    {
        return;

    }

    m_initRoutine.release();
    m_routines.clear();

    std::list<PackageSection> sections = m_archive.getSections();
    std::list<PackageSection>::iterator it;

    for(it = sections.begin(); it != sections.end(); it++)
    {
        asIScriptModule* module = m_engine->get()->GetModule(it->getName().c_str());
        if(module != NULL)
        {
            std::cout << "discarding module: " << it->getName() << "\n";
            module->Discard();

        }

    }

    m_engine->get()->GarbageCollect(asGC_FULL_CYCLE | asGC_DESTROY_GARBAGE);

    while(!m_properties.empty())
    {
        m_engine->cleanProperty((*m_properties.begin()));
        m_properties.erase(m_properties.begin());

    }

    m_engine->remRef();
    m_engine = NULL;
    setError(ScriptPackage::NotBuilt);

}

void ScriptPackage::addRoutine(std::string name, std::vector<std::string> params)
{
    std::string type = params[0];
    std::string hookDecl;
    if(type == "Disabled")
    {
       hookDecl = "void onDisabled(Robot@)";

    }
    else if(type == "Autonomous")
    {
        hookDecl = "void onAutonomous(Robot@)";

    }
    else if(type == "OperatorControl")
    {
        hookDecl = "void onOperatorControl(Robot@)";

    }
    else if(type == "Test")
    {
        hookDecl = "void onTest(Robot@)";

    }
    else
    {
        return;

    }

    std::vector<std::string> scripts;
    for(unsigned int u = 1; u < params.size(); u++)
    {
       if(params[u] == "")
       {
           continue;

       }

       scripts.push_back(params[u]);

    }

    ScriptRoutine routine;
    routine.setup(name, hookDecl);
    routine.setScripts(scripts);
    m_routines.push_back(routine);

}

GlobalProperty* ScriptPackage::getProperty(std::string name)
{
    std::list<GlobalProperty*>::iterator it;
    for(it = m_properties.begin(); it != m_properties.end(); it++)
    {
        if((*it)->getName() == name)
        {
            return *it;

        }

    }

    return NULL;

}

ScriptRoutine* ScriptPackage::getRoutine(std::string name)
{
    std::list<ScriptRoutine>::iterator it;
    for(it = m_routines.begin(); it != m_routines.end(); it++)
    {
        if(it->getName() == name)
        {
            return &(*it);

        }

    }

    return NULL;

}

std::list<ScriptRoutine> ScriptPackage::getRoutines()
{
    return m_routines;

}

ScriptRoutine* ScriptPackage::getInitRoutine()
{
    return &m_initRoutine;

}

std::string ScriptPackage::getDefaultDisabledRoutine()
{
    GlobalProperty* g = getProperty("DISABLED_ROUTINE");
    if(g == NULL)
    {
        return "";

    }

    return *(std::string*)g->getPtr();

}

std::string ScriptPackage::getDefaultAutonomousRoutine()
{
    GlobalProperty* g = getProperty("AUTONOMOUS_ROUTINE");
    if(g == NULL)
    {
        return "";

    }

    return *(std::string*)g->getPtr();

}

std::string ScriptPackage::getDefaultOperatorControlRoutine()
{
    GlobalProperty* g = getProperty("OPERATORCONTROL_ROUTINE");
    if(g == NULL)
    {
        return "";

    }

    return *(std::string*)g->getPtr();

}

std::string ScriptPackage::getDefaultTestRoutine()
{
    GlobalProperty* g = getProperty("TEST_ROUTINE");
    if(g == NULL)
    {
        return "";

    }

    return *(std::string*)g->getPtr();

}

void ScriptPackage::setError(ScriptPackage::Error error)
{
    m_lastError = error;

}
