#ifndef GLOBALPROPERTY_H_
#define GLOBALPROPERTY_H_

#include <angelscript.h>
#include <string>

#include "ScriptEngine.h"

class GlobalProperty
{
public:
    GlobalProperty();

    ~GlobalProperty();

    void setup(std::string definition, std::string type, std::string name, void* ptr);

    bool registerProperty(ScriptEngine* engine);

    void release();

    void* getPtr();

    std::string getDefinition();

    std::string getType();

    std::string getName();

private:
    ScriptEngine* m_engine;
    void* m_ptr;

    std::string m_definition;
    std::string m_propertyType;
    std::string m_name;

};

#endif /* GLOBALPROPERTY_H_ */
