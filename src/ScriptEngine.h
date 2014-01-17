#ifndef SCRIPTENGINE_H_
#define SCRIPTENGINE_H_

#include <stack>
#include <list>
#include <vector>
#include <string>
#include <angelscript.h>

#include "ReferenceCountedObject.h"

#if defined _WIN32 || __linux__
#include <stdint.h>
#elif defined __VXWORKS__
#include <types/vxTypes.h>
#endif

class ScriptPlugin;
class GlobalProperty;

class ScriptEngine : public ReferenceCountedObject
{
public:
    ScriptEngine();

    asIScriptEngine* get();

    int32_t startGroup(std::string name);
    void endGroup();

    int32_t releaseGroup(std::string name);

    void addPlugin(ScriptPlugin* plugin);
    void remPlugin();

    uint32_t plugins();

    GlobalProperty* addProperty(std::string definition, std::string type,
            std::string name, std::vector<std::string> params, void* data);

    GlobalProperty* addProperty(std::string definition, std::string type,
            std::string name, void* ptr);

    void cleanProperty(GlobalProperty* property);

private:
    ~ScriptEngine();

    asIScriptEngine* m_engine;

    std::stack<std::string> m_groups;
    std::list<ScriptPlugin*> m_plugins;

};

#endif /* SCRIPTENGINE_H_ */
