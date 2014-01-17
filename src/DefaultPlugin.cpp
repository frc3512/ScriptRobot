#include "DefaultPlugin.h"
#include "add_on/scriptarray/scriptarray.h"
#include "add_on/scriptstdstring/scriptstdstring.h"
#include "add_on/scriptdictionary/scriptdictionary.h"
#include "add_on/scriptmath/scriptmath.h"
#include "add_on/scriptany/scriptany.h"
#include "Convert/convertstring.h"
#include "Convert/units.h"
#include <iostream>

DefaultPlugin::DefaultPlugin()
{

}

DefaultPlugin::~DefaultPlugin()
{

}

void DefaultPlugin::onInit()
{
    RegisterScriptArray(m_engine->get(), true);
    RegisterStdString(m_engine->get());
    RegisterScriptDictionary_Generic(m_engine->get());
    RegisterScriptMath(m_engine->get());
    registerConvertString(m_engine->get());
    registerConvertUnits(m_engine->get());

    m_engine->get()->RegisterGlobalFunction("void print(string)", asFUNCTION(DefaultPlugin::print), asCALL_CDECL);

    m_factory->add(new FactoryInfo("int", intFactory, intRecycler, false, 1, false));
    m_factory->add(new FactoryInfo("uint", uintFactory, uintRecycler, false, 1, false));
    m_factory->add(new FactoryInfo("float", floatFactory, floatRecycler, false, 1, false));
    m_factory->add(new FactoryInfo("string", stringFactory, stringRecycler, false, 1, false));
    m_factory->add(new FactoryInfo("bool", boolFactory, boolRecycler, false, 1, false));

}

std::string DefaultPlugin::getName()
{
    return "default";

}

void* DefaultPlugin::intFactory(std::vector<std::string>params, void* data)
{
    return new int32_t(toInt(params[0]));

}

void DefaultPlugin::intRecycler(void* ptr)
{
    delete (int32_t*)ptr;

}

void* DefaultPlugin::uintFactory(std::vector<std::string>params, void* data)
{
    return new uint32_t(toUInt(params[0]));

}

void DefaultPlugin::uintRecycler(void* ptr)
{
    delete (uint32_t*)ptr;

}

void* DefaultPlugin::floatFactory(std::vector<std::string>params, void* data)
{
    return new float(toFloat(params[0]));

}

void DefaultPlugin::floatRecycler(void* ptr)
{
    delete (float*)ptr;

}

void* DefaultPlugin::stringFactory(std::vector<std::string>params, void* data)
{
    return new std::string(params[0]);

}

void DefaultPlugin::stringRecycler(void* ptr)
{
    delete (std::string*)ptr;

}

void* DefaultPlugin::boolFactory(std::vector<std::string>params, void* data)
{
    return new bool(toBool(params[0]));

}

void DefaultPlugin::boolRecycler(void* ptr)
{
    delete (bool*)ptr;

}

void DefaultPlugin::messageCallback(const asSMessageInfo* msg)
{
    std::string s;
    if(msg->type == asMSGTYPE_WARNING)
    {
        s = "WARN ";

    }
    else if(msg->type == asMSGTYPE_INFORMATION)
    {
        s = "INFO ";

    }

    s = s
        + "an error occurred on line: "
        + toString(msg->row)
        + "\n\tof file: "
        + std::string(msg->section)
        + " . . . \n\t"
        + std::string(msg->message)
        + "\n";

    print(s);

}

void DefaultPlugin::print(std::string msg)
{
    std::cout << msg << "\n";

}
