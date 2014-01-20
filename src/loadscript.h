#ifndef LOADSCRIPT_H_
#define LOADSCRIPT_H_

#include <string>
#include <angelscript.h>

//returns the script file as a string
//processes include directives using the FileMatcher
std::string loadScript(std::string path);

#endif /* LOADSCRIPT_H_ */
