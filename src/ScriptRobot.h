#if !(defined __WIN32 || __linux__)

#ifndef SCRIPTROBOT_H_
#define SCRIPTROBOT_H_

#include <RobotBase.h>
#include <angelscript.h>
#include "ScriptEngine.h"

#include "ScriptPackage.h"

class ScriptRobot : public RobotBase
{
public:
    ScriptRobot();

    virtual void onSetup(ScriptPackage* package);
    virtual void onInit();
    virtual void onDisabled();
    virtual void onAutonomous();
    virtual void onOperatorControl();
    virtual void onTest();

    void load(std::string path);
    void unload();
    bool isReloading();

    ScriptEngine* getEngine();
    ScriptPackage* getPackage();

    void setDisabledRoutine(std::string name);
    std::string getDisabledRoutine();

    void setAutonomousRoutine(std::string name);
    std::string getAutonomousRoutine();

    void setOperatorControlRoutine(std::string name);
    std::string getOperatorControlRoutine();

    void setTestRoutine(std::string name);
    std::string getTestRoutine();

    static void print(std::string msg);

protected:
    bool reloading; ///< reloading
    ScriptEngine* m_engine; ///< Angelscript engine.
    asIScriptContext* m_ctx; ///< Script context.

    ScriptPackage* m_package; ///< Current package loaded.
    ScriptRoutine* m_routine; ///< Current routine being executed.

    std::string m_disabledRoutine;
    std::string m_autonomousRoutine;
    std::string m_operatorControlRoutine;
    std::string m_testRoutine;

private:
    ~ScriptRobot();

    static void messageCallback(const asSMessageInfo* msg);

    void StartCompetition();
    void executeRoutine();

};

#endif /* SCRIPTROBOT_H_ */

#endif

