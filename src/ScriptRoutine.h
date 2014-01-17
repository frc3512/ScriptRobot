#ifndef SCRIPTROUTINE_H_
#define SCRIPTROUTINE_H_

#include <list>
#include <string>
#include <vector>
#include <angelscript.h>

#if defined _WIN32 || __linux__
#include <stdint.h>
#elif defined __VXWORKS__
#include <types/vxTypes.h>
#endif

class ScriptRoutine
{
public:
    ScriptRoutine();

    ~ScriptRoutine();

    void release();

    void setup(std::string name, std::string hookDecl);

    void setScripts(std::vector<std::string> scripts);

    void loadHooksFromEngine(asIScriptEngine* engine);

    void setName(std::string name);

    std::string getName();

    void setHookDecl(std::string hookDecl);

    std::string getHookDecl();

    void addScript(std::string script);

    void remScript(std::string script);

    bool hasScript(std::string script);

    uint32_t getNumOfScripts();

    std::string getScript(uint32_t i);

    std::vector<std::string> getScripts();

    bool addHookFromModule(asIScriptModule* module);

    uint32_t getNumOfHooks();

    asIScriptFunction* getHook(uint32_t i);

private:
    std::string m_name;
    std::string m_hookDecl;
    std::vector<std::string> m_scripts;
    std::list<asIScriptFunction*> m_hooks;

};

#endif /* SCRIPTROUTINE_H_ */
