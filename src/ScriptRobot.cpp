#include "ScriptRobot.h"
#include "ScriptWPILib.h"
#include "DefaultPlugin.h"
#include <iostream>
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
        instance->load("FRC_UserProgram.scpkg");
        return;

    }

    instance->load(instance->getPackage()->getPath());

}

//TODO: std::flush everywhere
ScriptRobot::ScriptRobot()
{
    m_watchdog.SetEnabled(false);

    m_engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);

    m_routine = NULL;
    m_package = NULL;

    m_ctx = m_engine->CreateContext();
    load("FRC_UserProgram.scpkg");

}

ScriptRobot::~ScriptRobot()
{
    unload();
    m_ctx->Release();
    m_engine->Release();

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

    ScriptPackage* temp = new ScriptPackage;
    setup(temp);
    temp->read(path);
    if(temp->getLastError() != ScriptPackage::NotLoaded)
    {
        std::cout << temp->getLastErrorMessage() << "\n" << std::flush;
        delete temp;
        reloading = false;
        return;

    }

    temp->load();
    if(temp->getLastError() != ScriptPackage::NotBuilt)
    {
        std::cout << temp->getLastErrorMessage() << "\n" << std::flush;
        delete temp;
        reloading = false;
        return;

    }

    if(m_package != NULL)
    {
        m_package->unload();

    }

    temp->build(m_engine);
    if(!temp->isValid())
    {
        std::cout << temp->getLastErrorMessage() << "\n" << std::flush;
        delete temp;
        if(m_package != NULL)
        {
            m_package->load();
            m_package->build(m_engine);

        }
        reloading = false;
        return;

    }

    if(m_package != NULL)
    {
        delete m_package;
        m_package = NULL;

    }
    m_package = temp;

    std::cout << m_package->getLastErrorMessage() << "\n" << std::flush;

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

void ScriptRobot::setup(ScriptPackage* package)
{
    package->addPlugin(new DefaultPlugin);
    package->addPlugin(new ScriptWPILib);

    onSetup(package);

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
            lw->SetEnabled(true);
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
            lw->SetEnabled(false);

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
