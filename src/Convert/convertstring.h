#ifndef CONVERTSTRING_H_
#define CONVERTSTRING_H_

#include <string>
#include <angelscript.h>

void registerConvertString(asIScriptEngine* engine);

int toInt(std::string);
unsigned int toUInt(std::string);
float toFloat(std::string);
bool toBool(std::string);

std::string toString(int);
std::string toString(unsigned int);
std::string toString(float);
std::string toString(bool);

#endif /* CONVERTSTRING_H_ */
