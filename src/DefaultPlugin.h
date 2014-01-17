#ifndef DEFAULTPLUGIN_H_
#define DEFAULTPLUGIN_H_

#include "ScriptPlugin.h"
#include <vector>

#if defined _WIN32 || __linux__
#include <stdint.h>
#elif defined __VXWORKS__
#include <types/vxTypes.h>
#endif

class DefaultPlugin : public ScriptPlugin
{
public:
    DefaultPlugin();

    ~DefaultPlugin();

    std::string getName();

    static void* intFactory(std::vector<std::string> params, void* data);
    static void intRecycler(void* ptr);

    static void* uintFactory(std::vector<std::string> params, void* data);
    static void uintRecycler(void* ptr);

    static void* floatFactory(std::vector<std::string> params, void* data);
    static void floatRecycler(void* ptr);

    static void* stringFactory(std::vector<std::string> params, void* data);
    static void stringRecycler(void* ptr);

    static void* boolFactory(std::vector<std::string> params, void* data);
    static void boolRecycler(void* ptr);

    static void messageCallback(const asSMessageInfo* msg);
    static void print(std::string msg);

protected:
    void onInit();
};

#endif /* DEFAULTPLUGIN_H_ */
