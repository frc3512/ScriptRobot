#include "ScriptRobot.h"
#include "ScriptWPILib.h"
#include "DefaultPlugin.h"
#include <WPILib.h>

void loadSCPKG(char* path)
{
    ((ScriptRobot*)&RobotBase::getInstance())->load(path);
    delete path;

}

void reloadSCPKG()
{
    ScriptRobot* instance = (ScriptRobot*)&RobotBase::getInstance();
    if(instance->getPackage() == NULL)
    {
        instance->load("/FRC_UserProgram.scpkg");
        return;

    }

    instance->load(instance->getPackage()->getPath());

}

//TODO: std::flush everywhere
ScriptRobot::ScriptRobot()
{
    m_watchdog.SetEnabled(false);

    m_engine = new ScriptEngine;
    m_engine->addPlugin(new DefaultPlugin);
    m_engine->addPlugin(new ScriptWPILib);

    m_routine = NULL;
    m_package = NULL;

    m_ctx = m_engine->get()->CreateContext();
    load("/FRC_UserProgram.scpkg");

}

ScriptRobot::~ScriptRobot()
{
    unload();
    m_ctx->Release();
    m_engine->remRef();

}

void ScriptRobot::onSetup(ScriptPackage* package)
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

    if(m_engine == NULL)
    {
        reloading = false;
        return;

    }

    std::cout << "loading from path: " << path << "\n" << std::flush;
    ScriptPackage* temp = new ScriptPackage;
    temp->load(path);
    if(temp->getError() != ScriptPackage::NotBuilt)
    {
        std::cout << "could not load error: " << temp->getError() << "\n" << std::flush;
        delete temp;
        reloading = false;
        std::cout << "ADDRESS: " << m_package << "\n";
        return;

    }

    m_ctx->Release();
    std::cout << "releasing old package\n" << std::flush;
    if(m_package != NULL)
    {
        m_package->unload();

    }
    else
    {
    	std::cout << "THE PACKAGE DIDN'T EXIST\n" << std::flush;

    }
    m_ctx = m_engine->get()->CreateContext();

    std::cout << "building engine\n" << std::flush;
    temp->build(m_engine);
    if(temp->getError() != ScriptPackage::NoError)
    {
        std::cout << "error while building: " << temp->getError() << "\n" << std::flush;
        temp->unload();
        if(m_package != NULL)
        {
            m_package->build(m_engine);

        }
        delete temp;
        reloading = false;
        std::cout << "ADDRESS: " << m_package << "\n";
        return;

    }

    std::cout << "deleting old package\n" << std::flush;

    if(m_package != NULL)
    {
        delete m_package;
        m_package = NULL;

    }
    m_package = temp;

    std::cout << "succesfully built: " << m_package->getError() << "\n" << std::flush;

    std::cout << "ADDRESS: " << m_package << "\n";

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
        delete m_package;
        m_package = NULL;

    }

    m_routine = NULL;

}

bool ScriptRobot::isReloading()
{
    return reloading;

}

ScriptEngine* ScriptRobot::getEngine()
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
    SmartDashboard::init();
    NetworkTable::GetTable("LiveWindow")->GetSubTable("~STATUS~")->PutBoolean("LW Enabled", false);

    onInit();

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
            m_ds->InDisabled(true);
            while(IsDisabled())
            {
                if(isReloading())
                {
                    break;

                }

                if(!(m_engine == NULL || m_ctx == NULL || m_package == NULL))
                {
                    m_routine = m_package->getRoutine(m_disabledRoutine);

                }
                else
                {
                    m_routine = NULL;

                }

                executeRoutine();

                onDisabled();
                Wait(0.01);
            }
            m_ds->InDisabled(false);

        }
        else if(IsAutonomous())
        {
            m_ds->InAutonomous(true);
            while(IsAutonomous() && IsEnabled())
            {
                if(isReloading())
                {
                    break;

                }

                if(!(m_engine == NULL || m_ctx == NULL || m_package == NULL))
                {
                    m_routine = m_package->getRoutine(m_autonomousRoutine);

                }
                else
                {
                    m_routine = NULL;

                }

                executeRoutine();

                onAutonomous();
                Wait(0.01);
            }
            m_ds->InAutonomous(false);

        }
        else if(IsOperatorControl())
        {
            m_ds->InOperatorControl(true);
            while(IsOperatorControl() && IsEnabled())
            {
                if(isReloading())
                {
                    break;

                }

                if(!(m_engine == NULL || m_ctx == NULL || m_package == NULL))
                {
                    m_routine = m_package->getRoutine(m_operatorControlRoutine);

                }
                else
                {
                    m_routine = NULL;

                }

                executeRoutine();

                onOperatorControl();
                Wait(0.01);
            }

            m_ds->InOperatorControl(false);

        }
        else if(IsTest())
        {
            m_ds->InTest(true);
            while(IsTest() && IsEnabled())
            {
                if(isReloading())
                {
                    break;

                }

                if(!(m_engine == NULL || m_ctx == NULL || m_package == NULL))
                {
                    m_routine = m_package->getRoutine(m_testRoutine);

                }
                else
                {
                    m_routine = NULL;

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

START_ROBOT_CLASS(ScriptRobot)
