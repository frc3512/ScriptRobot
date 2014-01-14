#ifndef SCRIPTPLUGIN_H_
#define SCRIPTPLUGIN_H_

#include <queue>
#include <vector>
#include <string>
#include <angelscript.h>

#include "PropertyFactory.h"

class ScriptPlugin
{
public:
    ScriptPlugin();

    virtual ~ScriptPlugin();

    void initFactories();

    void initBindings(asIScriptEngine* engine);

    void deinitFactories();

    void deinitBindings();

    bool areFactoriesInitialized();

    bool areBindingsInitialized();

    virtual std::string getName() = 0;

    bool hasType(std::string type);

    void* create(std::string type, std::vector<std::string> params, void* data);

    bool clean(std::string type, void* ptr);

protected:
    virtual void onInitFactories() = 0;
    virtual void onInitBindings() = 0;

    virtual void onDeinitFactories() = 0;
    virtual void onDeinitBindings() = 0;

    asIScriptEngine* m_engine;
    PropertyFactory* m_factory;

    bool factoriesInitialized;
    bool bindingsInitialized;

};

#endif /* SCRIPTPLUGIN_H_ */
