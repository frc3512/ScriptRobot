#ifndef SCRIPTPLUGIN_H_
#define SCRIPTPLUGIN_H_

#include <vector>
#include <string>
#include <angelscript.h>

#include "ScriptEngine.h"
#include "PropertyFactory.h"

class ScriptPlugin
{
public:
    ScriptPlugin();

    virtual ~ScriptPlugin();

    void init(ScriptEngine* engine);

    void deinit();

    virtual std::string getName() = 0;

    bool has(std::string type);

    void* create(std::string type, std::vector<std::string> params, void* data);

    bool clean(std::string type, void* ptr);

protected:
    virtual void onInit() = 0;

    ScriptEngine* m_engine;
    PropertyFactory* m_factory;

};

#endif /* SCRIPTPLUGIN_H_ */
