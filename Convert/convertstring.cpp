#include "convertstring.h"
#include <sstream>
#include "../add_on/vxw_assert.h"

void registerConvertString(asIScriptEngine* engine)
{
	assert(engine->RegisterGlobalFunction("int toInt(const string& in)", asFUNCTION(toInt), asCALL_CDECL) >= 0);
	assert(engine->RegisterGlobalFunction("uint toUInt(const string& in)", asFUNCTION(toUInt), asCALL_CDECL) >= 0);
	assert(engine->RegisterGlobalFunction("float toFloat(const string& in)", asFUNCTION(toFloat), asCALL_CDECL) >= 0);
	assert(engine->RegisterGlobalFunction("bool toBool(const string& in)", asFUNCTION(toBool), asCALL_CDECL) >= 0);
	assert(engine->RegisterGlobalFunction("string toString(int)", asFUNCTIONPR(toString, (int), std::string), asCALL_CDECL) >= 0);
	assert(engine->RegisterGlobalFunction("string toString(uint)", asFUNCTIONPR(toString, (unsigned int), std::string), asCALL_CDECL) >= 0);
	assert(engine->RegisterGlobalFunction("string toString(float)", asFUNCTIONPR(toString, (float), std::string), asCALL_CDECL) >= 0);
	assert(engine->RegisterGlobalFunction("string toString(bool)", asFUNCTIONPR(toString, (bool), std::string), asCALL_CDECL) >= 0);

}

int toInt(std::string s)
{
	std::stringstream ss(s);
	int i;
	ss >> i;
	return i;

}

unsigned int toUInt(std::string s)
{
	std::stringstream ss(s);
	unsigned int i;
	ss >> i;
	return i;

}

float toFloat(std::string s)
{
	std::stringstream ss(s);
	float i;
	ss >> i;
	return i;

}

bool toBool(std::string s)
{
	return s == "true";

}

std::string toString(int i)
{
	std::stringstream ss;
	ss << i;
	std::string s;
	ss >> s;
	return s;

}

std::string toString(unsigned int i)
{
	std::stringstream ss;
	ss << i;
	std::string s;
	ss >> s;
	return s;

}

std::string toString(float i)
{
	std::stringstream ss;
	ss << i;
	std::string s;
	ss >> s;
	return s;

}

std::string toString(bool i)
{
	return i ? "true" : "false";

}
