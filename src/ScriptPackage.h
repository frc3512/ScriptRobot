#ifndef SCRIPTPACKAGE_H_
#define SCRIPTPACKAGE_H_

#include <string>

#include "ScriptEngine.h"
#include "ScriptRoutine.h"
#include "PackageArchive.h"
#include "GlobalProperty.h"

#if defined _WIN32 || __linux__
#include <stdint.h>
#elif defined __VXWORKS__
#include <types/vxTypes.h>
#endif

class ScriptPackage
{
public:
    ScriptPackage();

    ~ScriptPackage();

    typedef enum Error
    {
        NoError = 0,
        NotLoaded = -1,
        ReadError = -2,
        NotBuilt = -3,
        NullEngine = -4,
        ConfigError = -5,
        FailedBuild = -6,

    } Error;

    bool isValid();

    ScriptPackage::Error getError();

    ScriptPackage::Error load(std::string path);

    std::string getPath();

    ScriptPackage::Error build(ScriptEngine* engine);

    static bool isScript(std::string path);

    static bool isConfig(std::string path);

    void unload();

    void addRoutine(std::string name, std::vector<std::string> params);

    GlobalProperty* getProperty(std::string name);

    ScriptRoutine* getRoutine(std::string name);

    ScriptRoutine* getInitRoutine();

    std::string getDefaultDisabledRoutine();

    std::string getDefaultAutonomousRoutine();

    std::string getDefaultOperatorControlRoutine();

    std::string getDefaultTestRoutine();

private:
    void setError(ScriptPackage::Error error);

    ScriptPackage::Error m_lastError;

    ScriptEngine* m_engine;
    PackageArchive m_archive;

    ScriptRoutine m_initRoutine;

    std::list<ScriptRoutine> m_routines;
    std::list<GlobalProperty*> m_properties;

};

#endif /* SCRIPTPACKAGE_H_ */
