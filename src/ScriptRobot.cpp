#include "ScriptRobot.h"
#include "scriptwpilib.h"
#include "add_on/scriptarray/scriptarray.h"
#include "add_on/scriptstdstring/scriptstdstring.h"
#include "add_on/scriptdictionary/scriptdictionary.h"
#include "add_on/scriptmath/scriptmath.h"
#include "add_on/scriptany/scriptany.h"
#include "Convert/convertstring.h"
#include "Convert/units.h"
#include <iostream>
#include <WPILib.h>

ScriptRobot::ScriptRobot()
{
    m_watchdog.SetEnabled(false);

    m_engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
    RegisterScriptArray(m_engine, true);
    RegisterStdString(m_engine);
    RegisterScriptDictionary_Generic(m_engine);
    RegisterScriptMath(m_engine);
    RegisterScriptAny_Generic(m_engine);
    registerWPILib(m_engine);
    registerConvertString(m_engine);
    registerConvertUnits(m_engine);
    m_engine->SetMessageCallback(asFUNCTION(ScriptRobot::messageCallback), 0, asCALL_CDECL);
    m_engine->RegisterGlobalFunction("void print(string)", asFUNCTION(ScriptRobot::print), asCALL_CDECL);

    setup();

    m_package = NULL;
    m_routine = NULL;

    m_ctx = m_engine->CreateContext();
    load("FRC_UserProgram.scpkg");

}

ScriptRobot::~ScriptRobot()
{
    unload();
    m_ctx->Release();
    m_engine->Release();

}

void ScriptRobot::setup()
{
    //OVERRIDEME

}

void ScriptRobot::onInit()
{
    //OVERRIDEME

}

void ScriptRobot::onDisabled()
{
    //OVERRIDEME

}

void ScriptRobot::onAutonomous()
{
    //OVERRIDEME

}

void ScriptRobot::onOperatorControl()
{
    //OVERRIDEME

}

void ScriptRobot::onTest()
{
    //OVERRIDEME

}

void ScriptRobot::load(std::string path)
{
    reloading = true;

    ScriptPackage* temp = new ScriptPackage;
    temp->load(path);
    if(temp->getLastError() != ScriptPackage::Error::NotBuilt)
    {
        std::cout << temp->getLastErrorMessage() << "\n";
        delete temp;
        return;

    }

    temp->build(m_engine);
    if(!temp->isValid())
    {
        std::cout << temp->getLastErrorMessage() << "\n";
        delete temp;
        return;

    }

    if(m_package != NULL)
    {
        delete m_package;
        m_package = NULL;

    }
    m_package = temp;

    std::cout << m_package->getLastErrorMessage() << "\n";

    m_disabledRoutine = m_package->getDefaultDisabledRoutine();
    m_autonomousRoutine = m_package->getDefaultAutonomousRoutine();
    m_operatorControlRoutine = m_package->getDefaultOperatorControlRoutine();
    m_testRoutine = m_package->getDefaultTestRoutine();

    m_routine = m_package->getInitRoutine();
    executeRoutine();
    m_routine = NULL;

    reloading = false;

}

void ScriptRobot::unload()
{
    if(m_package != NULL)
    {
        m_package->release();
        delete m_package;
        m_package = NULL;

    }

    m_routine = NULL;

}

bool ScriptRobot::isReloading()
{
    return reloading;

}

asIScriptEngine* ScriptRobot::getEngine()
{
    return m_engine;

}

ScriptPackage* ScriptRobot::getPackage()
{
    return m_package;

}

void ScriptRobot::setDisabledRoutine(std::string name)
{
    m_disabledRoutine = name;

}

std::string ScriptRobot::getDisabledRoutine()
{
    return m_disabledRoutine;

}

void ScriptRobot::setAutonomousRoutine(std::string name)
{
    m_autonomousRoutine = name;

}

std::string ScriptRobot::getAutonomousRoutine()
{
    return m_autonomousRoutine;

}

void ScriptRobot::setOperatorControlRoutine(std::string name)
{
    m_operatorControlRoutine = name;

}

std::string ScriptRobot::getOperatorControlRoutine()
{
    return m_operatorControlRoutine;

}

void ScriptRobot::setTestRoutine(std::string name)
{
    m_testRoutine = name;

}

std::string ScriptRobot::getTestRoutine()
{
    return m_testRoutine;

}

void ScriptRobot::StartCompetition()
{
    LiveWindow* lw = LiveWindow::GetInstance();

    SmartDashboard::init();
    NetworkTable::GetTable("LiveWindow")->GetSubTable("~STATUS~")->PutBoolean("LW Enabled", false);

    onInit();

    lw->SetEnabled(false);

    while(true)
    {
        if(isReloading())
        {
            m_routine = NULL;

            while(isReloading())
            {
                Wait(0.01);

            }

        }
        else if(IsDisabled())
        {
            if(!(m_engine == NULL || m_ctx == NULL || m_package == NULL))
            {
                m_routine = m_package->getRoutine(m_disabledRoutine);

            }
            else
            {
                m_routine = NULL;

            }

            m_ds->InDisabled(true);
            while(IsDisabled())
            {
                if(isReloading())
                {
                    break;

                }

                executeRoutine();

                onDisabled();
                Wait(0.01);
            }
            m_ds->InDisabled(false);

        }
        else if(IsAutonomous())
        {
            if(!(m_engine == NULL || m_ctx == NULL || m_package == NULL))
            {
                m_routine = m_package->getRoutine(m_autonomousRoutine);

            }
            else
            {
                m_routine = NULL;

            }

            m_ds->InAutonomous(true);
            while(IsAutonomous() && IsEnabled())
            {
                if(isReloading())
                {
                    break;

                }

                executeRoutine();

                onAutonomous();
                Wait(0.01);
            }
            m_ds->InAutonomous(false);

        }
        else if(IsOperatorControl())
        {
            if(!(m_engine == NULL || m_ctx == NULL || m_package == NULL))
            {
                m_routine = m_package->getRoutine(m_operatorControlRoutine);

            }
            else
            {
                m_routine = NULL;

            }

            lw->SetEnabled(true);
            m_ds->InOperatorControl(true);
            while(IsOperatorControl() && IsEnabled())
            {
                if(isReloading())
                {
                    break;

                }

                executeRoutine();

                onOperatorControl();
                Wait(0.01);
            }
            m_ds->InOperatorControl(false);
            lw->SetEnabled(false);

        }
        else if(IsTest())
        {
            if(!(m_engine == NULL || m_ctx == NULL || m_package == NULL))
            {
                m_routine = m_package->getRoutine(m_testRoutine);

            }
            else
            {
                m_routine = NULL;

            }

            m_ds->InTest(true);
            while(IsTest() && IsEnabled())
            {
                if(isReloading())
                {
                    break;

                }

                executeRoutine();

                onTest();
                Wait(0.01);
            }
            m_ds->InTest(false);

        }

        m_ds->WaitForData();

    }

}

void ScriptRobot::executeRoutine()
{
    if(!(m_engine == NULL || m_ctx == NULL || m_package == NULL || m_routine == NULL))
    {
        for(unsigned int i = 0; i < m_routine->getNumOfHooks(); i++)
        {
            asIScriptFunction* func = m_routine->getHook(i);
            if(m_routine != NULL)
            {
               if(m_ctx->Prepare(func) < 0)
               {
                   m_ctx->Unprepare();
                   return;

               }

               m_ctx->SetArgAddress(0, this);
               m_ctx->Execute();

            }

        }

    }

}

void ScriptRobot::print(std::string msg)
{
    std::cout << msg << "\n";

}

void ScriptRobot::messageCallback(const asSMessageInfo* msg)
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

START_ROBOT_CLASS(ScriptRobot)
