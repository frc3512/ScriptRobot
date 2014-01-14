#include <iostream>
#include "ScriptPackage.h"
#include "Config.h"
#include "Convert/convertstring.h"
#ifdef FAKEWPILIB
#include "FakeWPILib/FakeWPILib.h"
#else
#include <WPILib.h>
#endif

#ifdef _WIN32
#include <winsock2.h>
#elif UNIX
#include <netinet/in.h>
#endif

ScriptPackage::ScriptPackage()
{
    m_engine = NULL;
    m_initRoutine = NULL;
    setError(ScriptPackage::NotRead, "package just initialized");

}

ScriptPackage::~ScriptPackage()
{
    clear();
    unload();

}

ScriptPackage::Error ScriptPackage::getLastError()
{
    return m_lastError;

}

std::string ScriptPackage::getLastErrorMessage()
{
    return m_lastErrorMessage;

}

bool ScriptPackage::isValid()
{
    return getLastError() == ScriptPackage::NoError;

}

ScriptPackage::Error ScriptPackage::read(std::string path)
{
    if(isValid())
    {
        return getLastError();

    }

    clear();

    path = addExstension(path);

    PackageArchive::Error error = m_archive.read(path);   
    if(error != PackageArchive::NoError)
    {
        setError(ScriptPackage::ArchiveError, "Error while reading archive: " + path + "\n\terror: " + toString(error));
        return getLastError();

    }

    setError(ScriptPackage::NotLoaded, "Package read, but not loaded");
    return getLastError();

}

void ScriptPackage::clear()
{
    unload();
    m_archive.clear();

std::cout << "clearing\n";

    while(!m_plugins.empty())
    {
        delete (*m_plugins.begin());
	m_plugins.erase(m_plugins.begin());

    }

    if(getLastError() == ScriptPackage::NoError)
    {
        setError(ScriptPackage::NotRead, "Package not read");

    }

}

ScriptPackage::Error ScriptPackage::load()
{
    if(isValid() || getLastError() == ScriptPackage::NotRead)
    {
        std::cout << "returning here\n";
        return getLastError();

    }

    unload();
std::cout << "initing plugin factories\n";

    {//init all plugin factories
std::cout << "plugins: " << m_plugins.size() << "\n";
        std::list<ScriptPlugin*>::iterator it;
        for(it = m_plugins.begin(); it != m_plugins.end(); it++)
        {
            if((*it) != NULL && !(*it)->areFactoriesInitialized())
            {
		std::cout << "initing plugin: " << (*it)->getName() << "\n";
                (*it)->initFactories();

            }

        }

    }

std::cout << "setting up all routines and properties\n";

    {//setup all routines and add all properties
        std::list<PackageSection*> sections = m_archive.getSections();
        std::list<PackageSection*>::iterator it;

        m_initRoutine = new ScriptRoutine;
        m_initRoutine->setup("init", ScriptRoutine::Init);

        for(it = sections.begin(); it != sections.end(); it++)
        {
            if(ScriptPackage::isScript((*it)->getName()))
            {
                m_initRoutine->addScript((*it)->getName());

            }
            else if(isConfig((*it)->getName()))
            {
                //TODO: redo the config to be more robust and not create types
                Config cfg;
                Config::ParseError error = cfg.parse((*it)->getFile());
                if(error != Config::NoError)
                {
                    setError(ScriptPackage::ConfigError, "Error while parsing config: " + (*it)->getName() + "\n\terror: " + toString(error));
                    return getLastError();

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
                        void* ptr = NULL;

                        std::list<ScriptPlugin*>::iterator it2;
                        for(it2 = m_plugins.begin(); it2 != m_plugins.end(); it2++)
                        {
                            if((*it2)->hasType(type))
                            {
                                ptr = (*it2)->create(type, params, this);
                                break;

                            }

                        }

                        if(ptr == NULL)
                        {
                            continue;

                        }
                        else
                        {
                            addProperty(definition, type, name, ptr);

                        }

                    }

                }

            }

        }

    }

std::cout << "finished loading\n";

    setError(ScriptPackage::NotBuilt, "Package loaded, but not built");
    return getLastError();

}

void ScriptPackage::unload()
{
    release();

    while(!m_routines.empty())
    {
        delete (*m_routines.begin());
        m_routines.erase(m_routines.begin());

    }

    delete m_initRoutine;
    m_initRoutine = NULL;

    while(!m_properties.empty())
    {
        std::string type = (*m_properties.begin())->getType();
        void* ptr = (*m_properties.begin())->getPtr();

        std::list<ScriptPlugin*>::iterator it;
        for(it = m_plugins.begin(); it != m_plugins.end(); it++)
        {
            if((*it)->hasType(type))
            {
                (*it)->clean(type, ptr);

            }

        }

        if(ptr != NULL)
        {
            std::cout << "ScriptPackage::unload() unkown type while cleaning\n";

        }

        delete (GlobalProperty*)(*m_properties.begin());
        m_properties.erase(m_properties.begin());

    }

    if(getLastError() == ScriptPackage::NoError)
    {
        setError(ScriptPackage::NotLoaded, "Package not loaded");

    }

}

ScriptPackage::Error ScriptPackage::build(asIScriptEngine* engine)
{
    if(isValid())
    {
        return getLastError();

    }
    else if(getLastError() != ScriptPackage::NotBuilt)
    {
        unload();
        return getLastError();

    }

    if(engine == NULL)
    {
        setError(ScriptPackage::EngineIsNull, "The engine used to build was null");
        release();
        return getLastError();

    }

    m_engine = engine;

    {//register all plugins
        std::list<ScriptPlugin*>::iterator it;
        for(it = m_plugins.begin(); it != m_plugins.end(); it++)
        {
            if(!(*it)->areBindingsInitialized())
            {
                std::cout << "binding plugin: " << (*it)->getName() << "\n";
                (*it)->initBindings(m_engine);

            }

        }

    }

    std::cout << "finished binding plugins\n";

    {//Register all global properties
        std::list<GlobalProperty*>::iterator it;
        for(it = m_properties.begin(); it != m_properties.end(); it++)
        {
            if(!(*it)->registerProperty(m_engine))
            {
                setError(ScriptPackage::CouldNotRegisterProperty, "Failed to register property: " + (*it)->getType() + " " + (*it)->getName());
                release();
                return getLastError();

            }

        }

    }

    std::cout << "finished binding global properties\n";

    {//Build all script sections
        std::list<PackageSection*> sections = m_archive.getSections();
        std::list<PackageSection*>::iterator it;

        for(it = sections.begin(); it != sections.end(); it++)
        {
            PackageSection* section = (*it);
            if(ScriptPackage::isScript(section->getName()))
            {
                asIScriptModule* module = m_engine->GetModule(section->getName().c_str());
                if(module != NULL)
                {
                    module->Discard();

                }

                module = m_engine->GetModule(section->getName().c_str(), asGM_ALWAYS_CREATE);
                if(module->AddScriptSection(section->getName().c_str(), section->getFile().c_str()) < 0)
                {
                    setError(ScriptPackage::CouldNotBuildScript, "Failed to add script section to the module for: " + section->getName());
                    release();
                    return getLastError();

                }

                if(module->Build() < 0)
                {
                    setError(ScriptPackage::CouldNotBuildScript, "Failed to build script: " + section->getName());
                    release();
                    return getLastError();

                }

            }

        }

    }

    std::cout << "finished building scripts\n";

    {//Fetch all function pointers for routines.
        m_initRoutine->loadHooksFromEngine(m_engine);

        std::list<ScriptRoutine*>::iterator it;
        for(it = m_routines.begin(); it != m_routines.end(); it++)
        {
            (*it)->loadHooksFromEngine(m_engine);

        }

    }

    std::cout << "loaded routines\n";

    setError(ScriptPackage::NoError, "There was no error while building");
    return getLastError();

}

void ScriptPackage::release()
{
    if(m_engine == NULL)
    {
        return;

    }

    m_initRoutine->release();

    {//Release all angelscript function pointers from the ScriptRoutines.
        std::list<ScriptRoutine*>::iterator it;
        for(it = m_routines.begin(); it != m_routines.end(); it++)
        {
            (*it)->release();

        }

    }

    {//Release all angelscript modules created by this ScriptPackage.
        std::list<PackageSection*> sections = m_archive.getSections();
        std::list<PackageSection*>::iterator it;

        for(it = sections.begin(); it != sections.end(); it++)
        {
            asIScriptModule* module = m_engine->GetModule((*it)->getName().c_str());
            if(module != NULL)
            {
                module->Discard();

            }

        }

    }

    m_engine->GarbageCollect(asGC_FULL_CYCLE | asGC_DESTROY_GARBAGE);

    {//Release all global properties created by Configs.
        std::list<GlobalProperty*>::iterator it;
        for(it = m_properties.begin(); it != m_properties.end(); it++)
        {
            (*it)->release();

        }

    }

    {//release plugins
        std::list<ScriptPlugin*>::iterator it;
        for(it = m_plugins.begin(); it != m_plugins.end(); it++)
        {
            (*it)->deinitBindings();

        }

    }

    m_engine = NULL;

    if(getLastError() == ScriptPackage::NoError)
    {
        setError(ScriptPackage::NotBuilt, "Package not built");

    }

}

//TODO: make this actually do things
ScriptPackage::Error ScriptPackage::write(std::string path)
{
    path = addExstension(path);
    PackageArchive::Error error = m_archive.write(path);
    if(error != PackageArchive::NoError)
    {
        return ScriptPackage::ArchiveError;

    }

    return ScriptPackage::NoError;

}

std::string ScriptPackage::getPath()
{
    return m_archive.getPath();

}

//TODO: move these to some util file (FileSystem.h probably)
std::string ScriptPackage::addExstension(std::string path)
{
    if(path.rfind(".scpkg") != path.size() - 6)
    {
        path += ".scpkg";

    }

    return path;

}

bool ScriptPackage::isScript(std::string name)
{
    return name.rfind(".as") == name.size() - 3;

}

bool ScriptPackage::isConfig(std::string name)
{
    return name.rfind(".cfg") == name.size() - 4;

}

void ScriptPackage::addSection(std::string name, std::string file)
{
    m_archive.add(name, file);

}

void ScriptPackage::remSection(std::string name)
{
    m_archive.rem(name);

}

PackageSection* ScriptPackage::getSection(std::string name)
{
    return m_archive.getSection(name);

}

void ScriptPackage::addRoutine(std::string name, std::vector<std::string> params)
{
    std::string type = params[0];
    ScriptRoutine::Type routineType;
    if(type == "Disabled")
    {
       routineType = ScriptRoutine::Disabled;

    }
    else if(type == "Autonomous")
    {
       routineType = ScriptRoutine::Autonomous;

    }
    else if(type == "OperatorControl")
    {
       routineType = ScriptRoutine::OperatorControl;

    }
    else if(type == "Test")
    {
       routineType = ScriptRoutine::Test;

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

    addRoutine(name, routineType, scripts);

}

void ScriptPackage::addRoutine(std::string name, ScriptRoutine::Type type, std::vector<std::string> scripts)
{
    if(isValid())
    {
        return;

    }

    ScriptRoutine* routine = new ScriptRoutine;
    routine->setup(name, type);
    routine->setScripts(scripts);

    remRoutine(name);
    m_routines.push_back(routine);

}

void ScriptPackage::remRoutine(std::string name)
{
    if(isValid())
    {
        return;

    }

    std::list<ScriptRoutine*>::iterator it;
    for(it = m_routines.begin(); it != m_routines.end(); it++)
    {
        if((*it)->getName() == name)
        {
            delete (*it);
            m_routines.erase(it);
            return;

        }

    }

}

ScriptRoutine* ScriptPackage::getRoutine(std::string name)
{
    std::list<ScriptRoutine*>::iterator it;
    for(it = m_routines.begin(); it != m_routines.end(); it++)
    {
        if((*it)->getName() == name)
        {
            return (*it);

        }

    }

    return NULL;

}

ScriptRoutine* ScriptPackage::getInitRoutine()
{
    return m_initRoutine;

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

void ScriptPackage::addProperty(std::string definition, std::string type, std::string name, void* ptr)
{
    if(isValid() || ptr == NULL)
    {
        return;

    }

    GlobalProperty* property = new GlobalProperty;
    property->setup(definition, type, name, ptr);

    remProperty(name);
    m_properties.push_back(property);

}

void ScriptPackage::remProperty(std::string name)
{
    if(isValid())
    {
        return;

    }

    std::list<GlobalProperty*>::iterator it;
    for(it = m_properties.begin(); it != m_properties.end(); it++)
    {
        if((*it)->getName() == name)
        {
            delete (*it);
            m_properties.erase(it);
            return;

        }

    }

}

GlobalProperty* ScriptPackage::getProperty(std::string name)
{
    std::list<GlobalProperty*>::iterator it;
    for(it = m_properties.begin(); it != m_properties.end(); it++)
    {
        if((*it)->getName() == name)
        {
            return (*it);

        }

    }

    return NULL;

}

void ScriptPackage::addPlugin(ScriptPlugin* plugin)
{
    if(isValid() || plugin == NULL)
    {
        return;

    }

    std::cout << "added plugin: " << plugin->getName() << "\n";
    remPlugin(plugin->getName());
    m_plugins.push_back(plugin);

}

void ScriptPackage::remPlugin(std::string name)
{
    if(isValid())
    {
        return;

    }

    std::list<ScriptPlugin*>::iterator it;
    for(it = m_plugins.begin(); it != m_plugins.end(); it++)
    {
        if((*it)->getName() == name)
        {
            delete (*it);
            m_plugins.erase(it);
            return;

        }

    }

}

ScriptPlugin* ScriptPackage::getPlugin(std::string name)
{
    std::list<ScriptPlugin*>::iterator it;
    for(it = m_plugins.begin(); it != m_plugins.end(); it++)
    {
        if((*it)->getName() == name)
        {
            return (*it);

        }

    }

    return NULL;

}

void ScriptPackage::setError(ScriptPackage::Error error, std::string errorMessage)
{
    m_lastError = error;
    m_lastErrorMessage = errorMessage;

}
