#include <fstream>
#include <iostream>
#include "ScriptPackage.h"
#include "Config.h"
#include "Convert/convertstring.h"
#ifdef FAKEWPILIB
#include "FakeWPILib/FakeWPILib.h"
#else
#include <WPILib.h>
#endif

ScriptPackage::ScriptPackage()
{
    m_engine = NULL;
    m_initRoutine = NULL;
    setError(ScriptPackage::NotLoaded, "ScriptPackage just initialized not loaded yet.");


}

ScriptPackage::~ScriptPackage()
{
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

ScriptPackage::Error ScriptPackage::load(std::string path)
{
    unload();

    m_path = addExstension(path);

    std::fstream file;
    file.open(m_path.c_str(), std::fstream::binary | std::fstream::in);

    if(!file)
    {
        setError(ScriptPackage::CouldNotOpenFile, "Could not open file at: " + m_path);
        unload();
        return getLastError();

    }

    std::string header = "scpkg";
    int headerSize = header.size()*sizeof(char*);
    char headerBuffer[headerSize];
    file.read(headerBuffer, headerSize);

    if(std::string(headerBuffer) != header)
    {
        setError(ScriptPackage::NotAScriptPackage, m_path + " is not a ScriptPackage");
        unload();
        return getLastError();

    }

    while(!file.eof())
    {
        std::string nameStr;
        std::string fileStr;

        int nameSize = 0;
        file.read((char*)&nameSize, sizeof(int));

        if(nameSize != 0)
        {
            char buffer[nameSize];
            file.read(buffer, nameSize);
            nameStr = std::string(buffer);


        }

        int fileSize = 0;
        file.read((char*)&fileSize, sizeof(int));

        if(fileSize != 0)
        {
            char buffer[fileSize];
            file.read(buffer, fileSize);
            fileStr = std::string(buffer);

        }

        if(nameStr != "" || fileStr != "")
        {
            addSection(nameStr, fileStr);

        }

    }

    file.close();

    std::string projectConfigStr = getProjectConfig();

    if(projectConfigStr == "")
    {
        setError(ScriptPackage::MissingProjectConfig, "There was no project.cfg in the package");
        unload();
        return getLastError();

    }

    Config projectConfig;
    Config::ParseError error = projectConfig.parse(projectConfigStr);
    if(error != Config::NoError)
    {
        setError(ScriptPackage::ErrorParsingConfig, "Error parsing project.cfg: " + Config::printParseError(error));
        unload();
        return getLastError();

    }

    m_defaultDisabledRoutine = projectConfig.getString("DISABLED_ROUTINE");
    m_defaultAutonomousRoutine = projectConfig.getString("AUTONOMOUS_ROUTINE");
    m_defaultOperatorControlRoutine = projectConfig.getString("OPERATORCONTROL_ROUTINE");
    m_defaultTestRoutine = projectConfig.getString("TEST_ROUTINE");

    std::vector<std::string> routineNames = projectConfig.getNamesOfType("Routine");
    for(unsigned int i = 0; i < routineNames.size(); i++)
    {
       std::vector<std::string> params = projectConfig.getParams(routineNames[i]);
       if(params.size() < 2)
       {
           unload();
           setError(ScriptPackage::ErrorParsingConfig, "Error parsing project.cfg: Routine: " + routineNames[i] + " is missing a parameter");
           return getLastError();

       }

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
           unload();
           setError(ScriptPackage::ErrorParsingConfig, "Error parsing project.cfg: Routine: " + routineNames[i] + " does not have a type");
           return getLastError();

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

       addRoutine(routineNames[i], routineType, scripts);

    }

    {//Prepares the robot init routine.
        m_initRoutine = new ScriptRoutine;
        m_initRoutine->setup("init", ScriptRoutine::Init);

        std::vector<std::string> scripts;

        std::list<PackageSection*>::iterator it;
        for(it = m_sections.begin(); it != m_sections.end(); it++)
        {
            if(ScriptPackage::isScript((*it)->getName()))
            {
                scripts.push_back((*it)->getName());

            }

        }

        m_initRoutine->setScripts(scripts);

    }

    std::string robotConfigStr = getRobotConfig();

    if(robotConfigStr == "")
    {
        setError(ScriptPackage::MissingRobotConfig, "There was no robot.cfg in the package");
        unload();
        return getLastError();

    }

    Config robotConfig;
    error = robotConfig.parse(robotConfigStr);
    if(error != Config::NoError)
    {
        setError(ScriptPackage::ErrorParsingConfig, "Error parsing robot.cfg: " + Config::printParseError(error));
        unload();
        return getLastError();

    }

    {//Prepare all robot devices
        for(unsigned int i = 0; i < robotConfig.sections.size(); i++)
        {
            std::string name = robotConfig.sections[i].key;
            std::string type = robotConfig.sections[i].identifier;
            std::vector<std::string> params = robotConfig.sections[i].params;
            std::string definition = robotConfig.sections[i].section + ";";
            std::cout << "definition " << definition << "\n";

            //TODO: return an error if we can't create a device with the given params
            //TODO: find a better way to do all this nonsense it's all the same
            if(type == "Joystick")
            {
                if(params.size() != 1)
                {
                    continue;

                }

                int port = toInt(params[0]);
                Joystick* temp = new Joystick(port);
                addProperty(definition, type, name, temp);

            }
            else if(type == "Servo")
            {
                if(params.size() != 1)
                {
                    continue;

                }

                int port = toInt(params[0]);
                Servo* temp = new Servo(port);
                addProperty(definition, type, name, temp);

            }
            else if(type == "Relay")
            {
                if(params.size() != 1)
                {
                    continue;

                }

                int port = toInt(params[0]);
                Relay* temp = new Relay(port);
                addProperty(definition, type, name, temp);

            }
            else if(type == "Solenoid")
            {
                if(params.size() != 1)
                {
                    continue;

                }

                int port = toInt(params[0]);
                Solenoid* temp =  new Solenoid(port);
                addProperty(definition, type, name, temp);

            }
            else if(type == "DoubleSolenoid")
            {
                if(params.size() != 2)
                {
                    continue;

                }

                int forwardPort = toInt(params[0]);
                int reversePort = toInt(params[1]);

                DoubleSolenoid* temp = new DoubleSolenoid(forwardPort, reversePort);
                addProperty(definition, type, name, temp);

            }
            else if(type == "Victor")
            {
                if(params.size() != 1)
                {
                    continue;

                }

                int port = toInt(params[0]);

                Victor* temp = new Victor(port);
                addProperty(definition, type, name, temp);

            }
            else if(type == "Jaguar")
            {
                if(params.size() != 1)
                {
                    continue;

                }

                int port = toInt(params[0]);

                Jaguar* temp = new Jaguar(port);
                addProperty(definition, type, name, temp);

            }
            else if(type == "Talon")
            {
                if(params.size() != 1)
                {
                    continue;

                }

                int port = toInt(params[0]);

                Talon* temp = new Talon(port);
                addProperty(definition, type, name, temp);

            }
            else if(type == "Counter")
            {
                if(params.size() != 1)
                {
                    continue;

                }

                int port = toInt(params[0]);

                Counter* temp = new Counter(port);
                addProperty(definition, type, name, temp);

            }
            else if(type == "Encoder")
            {
                if(params.size() != 2)
                {
                    continue;

                }

                int aChan = toInt(params[0]);
                int bChan = toInt(params[1]);

                Encoder* temp = new Encoder(aChan, bChan);
                addProperty(definition, type, name, temp);

            }
            else if(type == "Timer")
            {
                //TODO: make a way that you can make a timer like this: Timer foo;
                //instead of Timer foo = 0; - The config parser isn't really made
                //for construction of random objects
                if(params.size() != 1)
                {
                    continue;

                }

                Timer* temp = new Timer;
                addProperty(definition, type, name, temp);

            }
            else if(type == "RobotDrive")
            {
                RobotDrive* temp;
                if(params.size() == 2)
                {
                    int leftMotorChan = toInt(params[0]);
                    int rightMotorChan = toInt(params[1]);

                    temp = new RobotDrive(leftMotorChan, rightMotorChan);
                    addProperty(definition, type, name, temp);

                }
                else if(params.size() == 4)
                {
                    int frontLMotorChan = toInt(params[0]);
                    int rearLMotorChan = toInt(params[1]);
                    int frontRMotorChan = toInt(params[2]);
                    int rearRMotorChan = toInt(params[3]);

                    temp = new RobotDrive(frontLMotorChan, rearLMotorChan,
                                frontRMotorChan, rearRMotorChan);
                    addProperty(definition, type, name, temp);

                }

            }

        }

    }

    {//Prepares all global properties.
        std::list<PackageSection*>::iterator it;
        for(it = m_sections.begin(); it != m_sections.end(); it++)
        {
            PackageSection* section = (*it);
            if(ScriptPackage::isConfig(section->getName()) && section->getName() != "project.cfg" && section->getName() != "robot.cfg")
            {
                Config cfg;
                error = cfg.parse(section->getFile());
                if(error !=  Config::NoError)
                {
                    setError(ErrorParsingConfig, "Error parsing " + section->getName() + ": " + Config::printParseError(error));
                    unload();
                    return getLastError();

                }

                for(unsigned int i = 0; i < cfg.sections.size(); i++)
                {
                    SectionData data = cfg.sections[i];
                    if(data.identifier == "int")
                    {
                        addProperty(data.section + ";", data.identifier, data.key, new int(*(int*)data.value));

                    }
                    else if(data.identifier == "uint")
                    {
                        addProperty(data.section + ";", data.identifier, data.key, new unsigned int(*(unsigned int*)data.value));

                    }
                    else if(data.identifier == "float")
                    {
                        addProperty(data.section + ";", data.identifier, data.key, new float(*(float*)data.value));

                    }
                    else if(data.identifier == "string")
                    {
                        addProperty(data.section + ";", data.identifier, data.key, new std::string(*(std::string*)data.value));

                    }
                    else if(data.identifier == "bool")
                    {
                        addProperty(data.section + ";", data.identifier, data.key, new bool(*(bool*)data.value));

                    }

                }

            }

        }

    }

    setError(ScriptPackage::NotBuilt, "The package was successfully loaded, but it hasn't been built");
    return getLastError();

}

void ScriptPackage::unload()
{
    release();

    m_path = "";

    m_defaultDisabledRoutine = "";
    m_defaultAutonomousRoutine = "";
    m_defaultOperatorControlRoutine = "";
    m_defaultTestRoutine = "";

    while(!m_sections.empty())
    {
        delete (*m_sections.begin());
        m_sections.erase(m_sections.begin());

    }

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

        //TODO: find a Better way of doing this
        if(type == "int")
        {
            delete (int*)ptr;

        }
        else if(type == "uint")
        {
            delete (unsigned int*)ptr;

        }
        else if(type == "float")
        {
            delete (float*)ptr;

        }
        else if(type == "bool")
        {
            delete (bool*)ptr;

        }
        else if(type == "string")
        {
            delete (std::string*)ptr;

        }
        else if(type == "Joystick")
        {
            delete (Joystick*)ptr;

        }
        else if(type == "Servo")
        {
            delete (Servo*)ptr;

        }
        else if(type == "Relay")
        {
            delete (Relay*)ptr;

        }
        else if(type == "Solenoid")
        {
            delete (Solenoid*)ptr;

        }
        else if(type == "DoubleSolenoid")
        {
            delete (DoubleSolenoid*)ptr;

        }
        else if(type == "Victor")
        {
            delete (Victor*)ptr;

        }
        else if(type == "Jaguar")
        {
            delete (Jaguar*)ptr;

        }
        else if(type == "Talon")
        {
            delete (Talon*)ptr;

        }
        else if(type == "Counter")
        {
            delete (Counter*)ptr;

        }
        else if(type == "Encoder")
        {
            delete (Encoder*)ptr;

        }
        else if(type == "Timer")
        {
            delete (Timer*)ptr;

        }
        else if(type == "RobotDrive")
        {
            delete (RobotDrive*)ptr;

        }
        else
        {
            delete (*m_properties.begin());

        }

        m_properties.erase(m_properties.begin());

    }

    if(getLastError() == ScriptPackage::NoError || getLastError() == ScriptPackage::NotBuilt)
    {
        setError(ScriptPackage::NotLoaded, "The package has not been loaded.");

    }

}

ScriptPackage::Error ScriptPackage::build(asIScriptEngine* engine)
{
    std::cout << "building\n";
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

    std::cout << "registering global properties\n";
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

    std::cout << "building scripts\n";
    {//Build all script sections
        std::list<PackageSection*>::iterator it;
        for(it = m_sections.begin(); it != m_sections.end(); it++)
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

    std::cout << "loading hooks\n";
    {//Fetch all function pointers for routines.
        m_initRoutine->loadHooksFromEngine(m_engine);

        std::list<ScriptRoutine*>::iterator it;
        for(it = m_routines.begin(); it != m_routines.end(); it++)
        {
            (*it)->loadHooksFromEngine(m_engine);

        }

    }

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
        std::list<PackageSection*>::iterator it;
        for(it = m_sections.begin(); it != m_sections.end(); it++)
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

    m_engine = NULL;

    if(getLastError() == ScriptPackage::NoError)
    {
        setError(ScriptPackage::NotBuilt, "The package has not been built");

    }

}

ScriptPackage::Error ScriptPackage::write(std::string path)
{
    m_path = addExstension(path);

    std::fstream file;
    file.open(m_path.c_str(), std::fstream::binary | std::fstream::out);

    if(!file)
    {
        setError(ScriptPackage::CouldNotOpenFile, "Could not open file at: " + m_path);
        unload();
        return getLastError();

    }

    std::string header = "scpkg";
    int headerSize = header.size()*sizeof(char*);
    file.write(header.c_str(), headerSize);

    std::string projectConfigStr = getProjectConfig();
    std::string robotConfigStr = getRobotConfig();
    std::string globalConfigStr;

    if(projectConfigStr == "")
    {//Add all of the Routines
        std::cout << "package already loaded. writing using objects.\n";
        std::list<ScriptRoutine*>::iterator it;
        for(it = m_routines.begin(); it != m_routines.end(); it++)
        {
            std::string name = (*it)->getName();
            ScriptRoutine::Type routineType = (*it)->getType();
            std::vector<std::string> scripts = (*it)->getScripts();

            std::string type;
            if(routineType == ScriptRoutine::Init)
            {
                type = "Init";

            }
            else if(routineType == ScriptRoutine::Disabled)
            {
                type = "Disabled";

            }
            else if(routineType == ScriptRoutine::Autonomous)
            {
                type = "Autonomous";

            }
            else if(routineType == ScriptRoutine::OperatorControl)
            {
                type = "OperatorControl";

            }
            else if(routineType == ScriptRoutine::Test)
            {
                type = "Test";

            }
            else
            {
                continue;

            }

            std::string routineLine;
            routineLine = "Routine " + name + " = " + type + ", ";
            for(unsigned int u = 0; u < scripts.size(); u++)
            {
                routineLine += scripts[u];
                if(u != scripts.size() - 1)
                {
                    routineLine += ", ";

                }

            }

            routineLine += ";";
            projectConfigStr += routineLine;

        }

    }

    {//Add all of the global properties
        std::list<GlobalProperty*>::iterator it;
        for(it = m_properties.begin(); it != m_properties.end(); it++)
        {
           std::string definition = (*it)->getDefinition();
           std::string type = (*it)->getType();
           std::string name = (*it)->getName();

           if(projectConfigStr != "" && (name == "DISABLED_ROUTINE" || name == "AUTONOMOUS_ROUTINE" || name == "OPERATORCONTROL_ROUTINE" || name == "TEST_ROUTINE"))
           {
               projectConfigStr += definition;

           }
           else if(robotConfigStr != "" && (type == "Joystick" || type == "Servo" || type == "Relay"
                   || type == "Solenoid" || type == "DoubleSolenoid"
                   || type == "Victor" || type == "Jaguar" || type == "Talon"
                   || type == "Counter" || type == "Encoder" || type == "Timer"
                   || type == "RobotDrive"))
           {
               robotConfigStr += definition;

           }
           else
           {
               globalConfigStr += definition;

           }

        }

    }

    addSection("project.cfg", projectConfigStr);
    addSection("robot.cfg", robotConfigStr);
    addSection("global.cfg", globalConfigStr);

    std::list<PackageSection*>::iterator it;
    for(it = m_sections.begin(); it != m_sections.end(); it++)
    {
        if(*(*it)->getNameSize() <= 0 || *(*it)->getFileSize() <= 0)
        {
            continue;

        }

        //name
        file.write((char*)(*it)->getNameSize(), sizeof(int));
        file.write((*it)->getName().c_str(), *(*it)->getNameSize());

        //file
        file.write((char*)(*it)->getFileSize(), sizeof(int));
        file.write((*it)->getFile().c_str(), *(*it)->getFileSize());

    }

    file.close();

    return getLastError();

}

std::string ScriptPackage::getPath()
{
    return m_path;

}

std::string ScriptPackage::addExstension(std::string path)
{
    if(path.rfind(".scpkg") != path.size() - 6)
    {
        path += ".scpkg";

    }

    return path;

}

std::string ScriptPackage::getProjectConfig()
{
    PackageSection* projectSection = getSection("project.cfg");
    if(projectSection != NULL)
    {
        return projectSection->getFile();

    }

    return "";

}

std::string ScriptPackage::getRobotConfig()
{
    PackageSection* robotSection = getSection("robot.cfg");
    if(robotSection != NULL)
    {
        return robotSection->getFile();

    }

    return "";

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
    PackageSection* section = new PackageSection(name, file);

    remSection(name);
    m_sections.push_back(section);

}

void ScriptPackage::remSection(std::string name)
{
    std::list<PackageSection*>::iterator it;
    for(it = m_sections.begin(); it != m_sections.end(); it++)
    {
        if((*it)->getName() == name)
        {
            delete (*it);
            m_sections.erase(it);
            release();
            setError(ScriptPackage::NotBuilt, "A Section was removed you need to rebuild");
            return;

        }

    }

}

PackageSection* ScriptPackage::getSection(std::string name)
{
    std::list<PackageSection*>::iterator it;
    for(it = m_sections.begin(); it != m_sections.end(); it++)
    {
        if((*it)->getName() == name)
        {
            return (*it);

        }

    }

    return NULL;

}

void ScriptPackage::addRoutine(std::string name, ScriptRoutine::Type type, std::vector<std::string> scripts)
{
    ScriptRoutine* routine = new ScriptRoutine;
    routine->setup(name, type);
    routine->setScripts(scripts);

    remRoutine(name);
    m_routines.push_back(routine);

}

void ScriptPackage::remRoutine(std::string name)
{
    std::list<ScriptRoutine*>::iterator it;
    for(it = m_routines.begin(); it != m_routines.end(); it++)
    {
        if((*it)->getName() == name)
        {
            delete (*it);
            m_routines.erase(it);
            release();
            setError(ScriptPackage::NotBuilt, "A Routine was removed you need to rebuild");
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
    return m_defaultDisabledRoutine;

}

std::string ScriptPackage::getDefaultAutonomousRoutine()
{
    return m_defaultAutonomousRoutine;

}

std::string ScriptPackage::getDefaultOperatorControlRoutine()
{
    return m_defaultOperatorControlRoutine;

}

std::string ScriptPackage::getDefaultTestRoutine()
{
    return m_defaultTestRoutine;

}

void ScriptPackage::addProperty(std::string definition, std::string type, std::string name, void* ptr)
{
    GlobalProperty* property = new GlobalProperty;
    property->setup(definition, type, name, ptr);

    remProperty(name);
    m_properties.push_back(property);

}

void ScriptPackage::remProperty(std::string name)
{
    std::list<GlobalProperty*>::iterator it;
    for(it = m_properties.begin(); it != m_properties.end(); it++)
    {
        if((*it)->getName() == name)
        {
            delete (*it);
            m_properties.erase(it);
            release();
            setError(ScriptPackage::NotBuilt, "A property was removed you need to rebuild");
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

void ScriptPackage::setError(ScriptPackage::Error error, std::string errorMessage)
{
    m_lastError = error;
    m_lastErrorMessage = errorMessage;

}
