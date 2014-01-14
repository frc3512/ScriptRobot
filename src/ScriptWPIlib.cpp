#include "ScriptWPILib.h"
#include "Convert/convertstring.h"
#include "ScriptPackage.h"
#include <iostream>

asDriverStation::asDriverStation()
{
    m_lcd = DriverStationLCD::GetInstance();
    m_ds = DriverStation::GetInstance();

}

//LCD
void asDriverStation::print(int line, int column, std::string msg)
{
    m_lcd->Printf(line, column, "%s", msg.c_str());
    m_lcd->UpdateLCD();

}

void asDriverStation::print(int line, std::string msg)
{
    m_lcd->PrintfLine(line, "%s", msg.c_str());
    m_lcd->UpdateLCD();

}

void asDriverStation::clear()
{
    m_lcd->Clear();

}

float asDriverStation::getStickAxis(unsigned int stick, unsigned int axis)
{
    return m_ds->GetStickAxis(stick, axis);

}

int asDriverStation::getStickButtons(unsigned int stick)
{
    return m_ds->GetStickButtons(stick);

}

float asDriverStation::getAnalogIn(unsigned int chan)
{
    return m_ds->GetAnalogIn(chan);

}

bool asDriverStation::getDigitalIn(unsigned int chan)
{
    return m_ds->GetDigitalIn(chan);

}

void asDriverStation::setDigitalOut(unsigned int chan, bool value)
{
    m_ds->SetDigitalOut(chan, value);

}

bool asDriverStation::getDigitalOut(unsigned int chan)
{
    return m_ds->GetDigitalOut(chan);

}

bool asDriverStation::isEnabled()
{
    return m_ds->IsEnabled();

}

bool asDriverStation::isDisabled()
{
    return m_ds->IsDisabled();

}

bool asDriverStation::isAutonomous()
{
    return m_ds->IsAutonomous();

}

bool asDriverStation::isTest()
{
    return m_ds->IsTest();

}

bool asDriverStation::isNewControlData()
{
    return m_ds->IsNewControlData();

}

bool asDriverStation::isFMSAttached()
{
    return m_ds->IsFMSAttached();

}

int asDriverStation::getAlliance()
{
    return m_ds->GetAlliance();

}

unsigned int asDriverStation::getLocation()
{
    return m_ds->GetLocation();

}

float asDriverStation::getMatchTime()
{
    return m_ds->GetMatchTime();

}

float asDriverStation::getBatteryVoltage()
{
    return m_ds->GetBatteryVoltage();

}

unsigned int asDriverStation::getTeamNumber()
{
    return m_ds->GetTeamNumber();

}

ScriptWPILib::ScriptWPILib()
{
    m_driverStation = NULL;

}

void ScriptWPILib::onInitFactories()
{
    m_factory->add(new FactoryInfo("Joystick", joystickFactory, joystickRecycler, false, 1, false));
    m_factory->add(new FactoryInfo("Servo", servoFactory, servoRecycler, false, 1, false));
    m_factory->add(new FactoryInfo("Relay", relayFactory, relayRecycler, false, 1, false));
    m_factory->add(new FactoryInfo("Solenoid", solenoidFactory, solenoidRecycler, false, 1, false));
    m_factory->add(new FactoryInfo("DoubleSolenoid", doubleSolenoidFactory, doubleSolenoidRecycler, false, 2, false));
    m_factory->add(new FactoryInfo("Victor", victorFactory, victorRecycler, false, 1, false));
    m_factory->add(new FactoryInfo("Jaguar", jaguarFactory, jaguarRecycler, false, 1, false));
    m_factory->add(new FactoryInfo("Talon", talonFactory, talonRecycler, false, 1, false));
    m_factory->add(new FactoryInfo("Counter", counterFactory, counterRecycler, false, 1, false));
    m_factory->add(new FactoryInfo("Encoder", encoderFactory, encoderRecycler, false, 2, false));
    m_factory->add(new FactoryInfo("AnalogChannel", analogChannelFactory, analogChannelRecycler, false, 1, false));
    m_factory->add(new FactoryInfo("DigitalInput", digitalInputFactory, digitalInputRecycler, false, 1, false));
    m_factory->add(new FactoryInfo("Timer", timerFactory, timerRecycler, true, 0, false));
    m_factory->add(new FactoryInfo("Compressor", compressorFactory, compressorRecycler, false, 2, false));
    m_factory->add(new FactoryInfo("RobotDrive", robotDriveFactory, robotDriveRecycler, true, 0, true));

    if(m_driverStation == NULL)
    {
        m_driverStation = new GlobalProperty;
        m_driverStation->setup("DriverStation driverStation;", "DriverStation", "driverStation", new asDriverStation);

    }

}

void ScriptWPILib::onInitBindings()
{
    ScriptWPILib::registerWPILib(m_engine);

    if(m_driverStation != NULL)
    {
        m_driverStation->registerProperty(m_engine, false);

    }

}

void ScriptWPILib::onDeinitFactories()
{
    if(m_driverStation != NULL)
    {
        delete (asDriverStation*)m_driverStation->getPtr();
        delete m_driverStation;

    }

    m_driverStation = NULL;

}

void ScriptWPILib::onDeinitBindings()
{
    if(m_driverStation != NULL)
    {
        m_driverStation->release();

    }

}

std::string ScriptWPILib::getName()
{
    return "wpilib";

}

void ScriptWPILib::registerWPILib(asIScriptEngine* engine)
{
    ScriptWPILib::registerJoystick(engine);
    ScriptWPILib::registerControllers(engine);
    ScriptWPILib::registerSensors(engine);
    ScriptWPILib::registerTimer(engine);
    ScriptWPILib::registerDriverStation(engine);
    ScriptWPILib::registerCompressor(engine);
    ScriptWPILib::registerRobotDrive(engine);

    engine->RegisterObjectType("Robot", sizeof(ScriptRobot), asOBJ_REF | asOBJ_NOCOUNT);

}

void ScriptWPILib::registerJoystick(asIScriptEngine* engine)
{
    ///// Joystick /////
    assert(engine->RegisterObjectType("Joystick", sizeof(Joystick), asOBJ_REF | asOBJ_NOCOUNT) >= 0);

    assert(engine->RegisterObjectMethod("Joystick", "float getX()", asMETHOD(Joystick, GetX), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("Joystick", "float getY()", asMETHOD(Joystick, GetY), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("Joystick", "float getZ()", asMETHOD(Joystick, GetZ), asCALL_THISCALL) >= 0);

    assert(engine->RegisterObjectMethod("Joystick", "float getTwist()", asMETHOD(Joystick, GetTwist), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("Joystick", "float getThrottle()", asMETHOD(Joystick, GetThrottle), asCALL_THISCALL) >= 0);

    assert(engine->RegisterEnum("Joystick_Axis") >= 0);
    assert(engine->RegisterEnumValue("Joystick_Axis", "x", Joystick::kXAxis) >= 0);
    assert(engine->RegisterEnumValue("Joystick_Axis", "y", Joystick::kYAxis) >= 0);
    assert(engine->RegisterEnumValue("Joystick_Axis", "z", Joystick::kZAxis) >= 0);
    assert(engine->RegisterEnumValue("Joystick_Axis", "twist", Joystick::kTwistAxis) >= 0);
    assert(engine->RegisterEnumValue("Joystick_Axis", "throttle", Joystick::kThrottleAxis) >= 0);

    assert(engine->RegisterObjectMethod("Joystick", "float getAxis(Joystick_Axis)", asMETHOD(Joystick, GetRawAxis), asCALL_THISCALL) >= 0);

    assert(engine->RegisterObjectMethod("Joystick", "bool getTrigger()", asMETHOD(Joystick, GetTrigger), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("Joystick", "bool getButton(uint)", asMETHOD(Joystick, GetRawButton), asCALL_THISCALL) >= 0);

    assert(engine->RegisterObjectMethod("Joystick", "float getMagnitude()", asMETHOD(Joystick, GetMagnitude), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("Joystick", "float getDirectionRadians()", asMETHOD(Joystick, GetDirectionRadians), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("Joystick", "float getDirectionDegrees()", asMETHOD(Joystick, GetDirectionDegrees), asCALL_THISCALL) >= 0);

}

void ScriptWPILib::registerControllers(asIScriptEngine* engine)
{
    ///// Servo /////
    assert(engine->RegisterObjectType("Servo", sizeof(Servo), asOBJ_REF | asOBJ_NOCOUNT) >= 0);

    assert(engine->RegisterObjectMethod("Servo", "void set(float)", asMETHOD(Servo, Set), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("Servo", "float get()", asMETHOD(Servo, Get), asCALL_THISCALL) >= 0);

    assert(engine->RegisterObjectMethod("Servo", "void setAngle(float)", asMETHOD(Servo, SetAngle), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("Servo", "float getAngle()", asMETHOD(Servo, GetAngle), asCALL_THISCALL) >= 0);

    ///// Relay /////
    assert(engine->RegisterObjectType("Relay", sizeof(Relay), asOBJ_REF | asOBJ_NOCOUNT) >= 0);

    assert(engine->RegisterEnum("Relay_Value") >= 0);
    assert(engine->RegisterEnumValue("Relay_Value", "off", Relay::kOff) >= 0);
    assert(engine->RegisterEnumValue("Relay_Value", "on", Relay::kOn) >= 0);
    assert(engine->RegisterEnumValue("Relay_Value", "forward", Relay::kForward) >= 0);
    assert(engine->RegisterEnumValue("Relay_Value", "reverse", Relay::kReverse) >= 0);

    assert(engine->RegisterObjectMethod("Relay", "void set(Relay_Value)", asMETHOD(Relay, Set), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("Relay", "Relay_Value get()", asMETHOD(Relay, Get), asCALL_THISCALL) >= 0);

    ///// Solenoid /////
    assert(engine->RegisterObjectType("Solenoid", sizeof(Solenoid), asOBJ_REF | asOBJ_NOCOUNT) >= 0);

    assert(engine->RegisterObjectMethod("Solenoid", "void set(bool)", asMETHOD(Solenoid, Set), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("Solenoid", "bool get()", asMETHOD(Solenoid, Get), asCALL_THISCALL) >= 0);

    ///// Double Solenoid /////
    assert(engine->RegisterObjectType("DoubleSolenoid", sizeof(DoubleSolenoid), asOBJ_REF | asOBJ_NOCOUNT) >= 0);

    assert(engine->RegisterEnum("DoubleSolenoid_Value") >= 0);
    assert(engine->RegisterEnumValue("DoubleSolenoid_Value", "off", DoubleSolenoid::kOff) >= 0);
    assert(engine->RegisterEnumValue("DoubleSolenoid_Value", "forward", DoubleSolenoid::kForward) >= 0);
    assert(engine->RegisterEnumValue("DoubleSolenoid_Value", "reverse", DoubleSolenoid::kReverse) >= 0);

    assert(engine->RegisterObjectMethod("DoubleSolenoid", "void set(DoubleSolenoid_Value)", asMETHOD(DoubleSolenoid, Set), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("DoubleSolenoid", "DoubleSolenoid_Value get()", asMETHOD(DoubleSolenoid, Get), asCALL_THISCALL) >= 0);

    ///// Victor /////
    assert(engine->RegisterObjectType("Victor", sizeof(Victor), asOBJ_REF | asOBJ_NOCOUNT) >= 0);

    assert(engine->RegisterObjectMethod("Victor", "void set(float)", asMETHOD(Victor, Set), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("Victor", "float get()", asMETHOD(Victor, Get), asCALL_THISCALL) >= 0);

    ///// Jaguar /////
    assert(engine->RegisterObjectType("Jaguar", sizeof(Jaguar), asOBJ_REF | asOBJ_NOCOUNT) >= 0);

    assert(engine->RegisterObjectMethod("Jaguar", "void set(float)", asMETHOD(Jaguar, Set), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("Jaguar", "float get()", asMETHOD(Jaguar, Get), asCALL_THISCALL) >= 0);

    ///// Talon /////
    assert(engine->RegisterObjectType("Talon", sizeof(Talon), asOBJ_REF | asOBJ_NOCOUNT) >= 0);

    assert(engine->RegisterObjectMethod("Talon", "void set(float)", asMETHOD(Talon, Set), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("Talon", "float get()", asMETHOD(Talon, Get), asCALL_THISCALL) >= 0);

}

void ScriptWPILib::registerSensors(asIScriptEngine* engine)
{
    ///// Counter /////
    assert(engine->RegisterObjectType("Counter", sizeof(Counter), asOBJ_REF | asOBJ_NOCOUNT) >= 0);

    assert(engine->RegisterObjectMethod("Counter", "void start()", asMETHOD(Counter, Start), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("Counter", "int get()", asMETHOD(Counter, Get), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("Counter", "void reset()", asMETHOD(Counter, Reset), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("Counter", "void stop()", asMETHOD(Counter, Stop), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("Counter", "float getPeriod()", asMETHOD(Counter, GetPeriod), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("Counter", "void setMaxPeriod(float)", asMETHOD(Counter, SetMaxPeriod), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("Counter", "void setUpdateWhenEmpty(bool)", asMETHOD(Counter, SetUpdateWhenEmpty), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("Counter", "bool getStopped()", asMETHOD(Counter, GetStopped), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("Counter", "bool getDirection()", asMETHOD(Counter, GetDirection), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("Counter", "uint getIndex()", asMETHOD(Counter, GetIndex), asCALL_THISCALL) >= 0);

    ///// Encoder /////
    assert(engine->RegisterObjectType("Encoder", sizeof(Encoder), asOBJ_REF | asOBJ_NOCOUNT) >= 0);

    assert(engine->RegisterObjectMethod("Encoder", "void start()", asMETHOD(Encoder, Start), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("Encoder", "int get()", asMETHOD(Encoder, Get), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("Encoder", "int getRaw()", asMETHOD(Encoder, GetRaw), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("Encoder", "void reset()", asMETHOD(Encoder, Reset), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("Encoder", "void stop()", asMETHOD(Encoder, Stop), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("Encoder", "float getPeriod()", asMETHOD(Encoder, GetPeriod), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("Encoder", "void setMaxPeriod(float)", asMETHOD(Encoder, SetMaxPeriod), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("Encoder", "bool getStopped()", asMETHOD(Encoder, GetStopped), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("Encoder", "bool getDirection()", asMETHOD(Encoder, GetDirection), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("Encoder", "float getDistance()", asMETHOD(Encoder, GetDistance), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("Encoder", "float getRate()", asMETHOD(Encoder, GetRate), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("Encoder", "void setMinRate(float)", asMETHOD(Encoder, SetMinRate), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("Encoder", "void setDistancePerPulse(float)", asMETHOD(Encoder, SetDistancePerPulse), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("Encoder", "void setReverseDirection(bool)", asMETHOD(Encoder, SetReverseDirection), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("Encoder", "float PIDGet()", asMETHOD(Encoder, PIDGet), asCALL_THISCALL) >= 0);

    //AnalogChannel
    assert(engine->RegisterObjectType("AnalogChannel", sizeof(AnalogChannel), asOBJ_REF | asOBJ_NOCOUNT) >= 0);

    assert(engine->RegisterObjectMethod("AnalogChannel", "int getValue()", asMETHOD(AnalogChannel, GetValue), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("AnalogChannel", "int getAverageValue()", asMETHOD(AnalogChannel, GetAverageValue), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("AnalogChannel", "float getVoltage()", asMETHOD(AnalogChannel, GetVoltage), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("AnalogChannel", "float getAverageVoltage()", asMETHOD(AnalogChannel, GetAverageVoltage), asCALL_THISCALL) >= 0);

    //DigitalInput
    assert(engine->RegisterObjectType("DigitalInput", sizeof(DigitalInput), asOBJ_REF | asOBJ_NOCOUNT) >= 0);

    assert(engine->RegisterObjectMethod("DigitalInput", "uint get()", asMETHOD(DigitalInput, Get), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("DigitalInput", "void setupSourceEdge(bool, bool)", asMETHOD(DigitalInput, SetUpSourceEdge), asCALL_THISCALL) >= 0);

}

void ScriptWPILib::registerTimer(asIScriptEngine* engine) //TODO: fix / test this
{
    ///// Timer /////
    engine->RegisterObjectType("Timer", sizeof(Timer), asOBJ_VALUE);
    engine->RegisterObjectBehaviour("Timer", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(timerConstructor), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectBehaviour("Timer", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(timerDestructor), asCALL_CDECL_OBJLAST);

    engine->RegisterObjectMethod("Timer", "float get()", asMETHOD(Timer, Get), asCALL_THISCALL);
    engine->RegisterObjectMethod("Timer", "void reset()", asMETHOD(Timer, Reset), asCALL_THISCALL);
    engine->RegisterObjectMethod("Timer", "void start()", asMETHOD(Timer, Start), asCALL_THISCALL);
    engine->RegisterObjectMethod("Timer", "void stop()", asMETHOD(Timer, Stop), asCALL_THISCALL);
    engine->RegisterObjectMethod("Timer", "bool hasTimePassed(float)", asMETHOD(Timer, HasPeriodPassed), asCALL_THISCALL);

}

void ScriptWPILib::timerConstructor(void* memory)
{
	new (memory)Timer();

}

void ScriptWPILib::timerDestructor(void* memory)
{
	((Timer*)memory)->~Timer();

}

void ScriptWPILib::registerDriverStation(asIScriptEngine* engine)
{
    assert(engine->RegisterObjectType("DriverStation", sizeof(asDriverStation), asOBJ_REF | asOBJ_NOCOUNT) >= 0);

    //lcd
    assert(engine->RegisterObjectMethod("DriverStation", "void print(int, int, string)", asMETHODPR(asDriverStation, print, (int, int, std::string), void), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("DriverStation", "void print(int, string)", asMETHODPR(asDriverStation, print, (int, std::string), void), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("DriverStation", "void clear()", asMETHOD(asDriverStation, clear), asCALL_THISCALL) >= 0);

    //fms/driverstation
    assert(engine->RegisterObjectMethod("DriverStation", "float getStickAxis(uint, uint)", asMETHOD(asDriverStation, getStickAxis), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("DriverStation", "int getStickButtons(uint)", asMETHOD(asDriverStation, getStickButtons), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("DriverStation", "float getAnalogIn(uint)", asMETHOD(asDriverStation, getAnalogIn), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("DriverStation", "bool getDigitalIn(uint)", asMETHOD(asDriverStation, getDigitalIn), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("DriverStation", "void setDigitalOut(uint, bool)", asMETHOD(asDriverStation, setDigitalOut), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("DriverStation", "bool getDigitalOut(uint)", asMETHOD(asDriverStation, getDigitalOut), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("DriverStation", "bool isEnabled()", asMETHOD(asDriverStation, isEnabled), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("DriverStation", "bool isDisabled()", asMETHOD(asDriverStation, isDisabled), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("DriverStation", "bool isTest()", asMETHOD(asDriverStation, isTest), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("DriverStation", "bool isNewControlData()", asMETHOD(asDriverStation, isNewControlData), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("DriverStation", "bool isFMSAttached()", asMETHOD(asDriverStation, isFMSAttached), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("DriverStation", "int getAlliance()", asMETHOD(asDriverStation, getAlliance), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("DriverStation", "uint getLocation()", asMETHOD(asDriverStation, getLocation), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("DriverStation", "float getMatchTime()", asMETHOD(asDriverStation, getMatchTime), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("DriverStation", "float getBatteryVoltage()", asMETHOD(asDriverStation, getBatteryVoltage), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("DriverStation", "uint getTeamNumber()", asMETHOD(asDriverStation, getTeamNumber), asCALL_THISCALL) >= 0);

}

void ScriptWPILib::registerCompressor(asIScriptEngine* engine)
{
    assert(engine->RegisterObjectType("Compressor", sizeof(Compressor), asOBJ_REF | asOBJ_NOCOUNT) >= 0);

    assert(engine->RegisterObjectMethod("Compressor", "void start()", asMETHOD(Compressor, Start), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("Compressor", "void stop()", asMETHOD(Compressor, Stop), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("Compressor", "bool enabled()", asMETHOD(Compressor, Enabled), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("Compressor", "uint getPressureSwitchValue()", asMETHOD(Compressor, GetPressureSwitchValue), asCALL_THISCALL) >= 0);

}

void ScriptWPILib::registerRobotDrive(asIScriptEngine* engine)
{
    ///// Robot Drive /////
    assert(engine->RegisterObjectType("RobotDrive", sizeof(RobotDrive), asOBJ_REF | asOBJ_NOCOUNT) >= 0);

    assert(engine->RegisterEnum("RobotDrive_Motor") >= 0);
    assert(engine->RegisterEnumValue("RobotDrive_Motor", "frontLeft", RobotDrive::kFrontLeftMotor) >= 0);
    assert(engine->RegisterEnumValue("RobotDrive_Motor", "frontRight", RobotDrive::kFrontRightMotor) >= 0);
    assert(engine->RegisterEnumValue("RobotDrive_Motor", "rearLeft", RobotDrive::kRearLeftMotor) >= 0);
    assert(engine->RegisterEnumValue("RobotDrive_Motor", "rearRight", RobotDrive::kRearRightMotor) >= 0);

    assert(engine->RegisterObjectMethod("RobotDrive", "void setInvertedMotor(RobotDrive_Motor, bool)", asMETHOD(RobotDrive, SetInvertedMotor), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("RobotDrive", "void setSensitivity(float)", asMETHOD(RobotDrive, SetSensitivity), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("RobotDrive", "void setMaxOutput(float)", asMETHOD(RobotDrive, SetMaxOutput), asCALL_THISCALL) >= 0);

    ///// TankDrive /////
    assert(engine->RegisterObjectMethod("RobotDrive", "void tankDrive(Joystick@, Joystick@, bool squareInputs = true)", asMETHODPR(RobotDrive, TankDrive, (GenericHID*, GenericHID*, bool), void), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("RobotDrive", "void tankDrive(Joystick@, uint, Joystick@, uint, bool squareInputs = true)", asMETHODPR(RobotDrive, TankDrive, (GenericHID*, uint32_t, GenericHID*, uint32_t, bool), void), asCALL_THISCALL) >= 0);

    ///// ArcadeDrive /////
    assert(engine->RegisterObjectMethod("RobotDrive", "void arcadeDrive(Joystick@, bool squareInputs = true)", asMETHODPR(RobotDrive, ArcadeDrive, (GenericHID*, bool), void), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("RobotDrive", "void arcadeDrive(Joystick@, uint, Joystick@, uint, bool squareInputs = true)", asMETHODPR(RobotDrive, ArcadeDrive, (GenericHID*, uint32_t, GenericHID*, uint32_t, bool), void), asCALL_THISCALL) >= 0);

    ///// MecanumDrive /////
    assert(engine->RegisterObjectMethod("RobotDrive", "void mecanumDrive(float, float, float, float)", asMETHOD(RobotDrive, MecanumDrive_Cartesian), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("RobotDrive", "void mecanumDrive(float, float, float)", asMETHOD(RobotDrive, MecanumDrive_Polar), asCALL_THISCALL) >= 0);

    ///// HolonomicDrive /////
    assert(engine->RegisterObjectMethod("RobotDrive", "void holonomicDrive(float, float, float)", asMETHOD(RobotDrive, HolonomicDrive), asCALL_THISCALL) >= 0);

}

void* ScriptWPILib::joystickFactory(std::vector<std::string> params, void* data)
{
    uint32_t port = toUInt(params[0]);
    Joystick* temp = new Joystick(port);
    return temp;

}

void ScriptWPILib::joystickRecycler(void* ptr)
{
    delete (Joystick*)ptr;

}

void* ScriptWPILib::servoFactory(std::vector<std::string> params, void* data)
{
    uint32_t port = toUInt(params[0]);
    Servo* temp = new Servo(port);
    return temp;

}

void ScriptWPILib::servoRecycler(void* ptr)
{
    delete (Servo*)ptr;

}

void* ScriptWPILib::relayFactory(std::vector<std::string> params, void* data)
{
    uint32_t port = toUInt(params[0]);
    Relay* temp = new Relay(port);
    return temp;

}

void ScriptWPILib::relayRecycler(void* ptr)
{
    delete (Relay*)ptr;

}

void* ScriptWPILib::solenoidFactory(std::vector<std::string> params, void* data)
{
    uint32_t port = toUInt(params[0]);
    Solenoid* temp = new Solenoid(port);
    return temp;

}

void ScriptWPILib::solenoidRecycler(void* ptr)
{
    delete (Solenoid*)ptr;

}

void* ScriptWPILib::doubleSolenoidFactory(std::vector<std::string> params, void* data)
{
    uint32_t port1 = toUInt(params[0]);
    uint32_t port2 = toUInt(params[1]);
    DoubleSolenoid* temp = new DoubleSolenoid(port1, port2);
    return temp;

}

void ScriptWPILib::doubleSolenoidRecycler(void* ptr)
{
    delete (DoubleSolenoid*)ptr;

}

void* ScriptWPILib::victorFactory(std::vector<std::string> params, void* data)
{
    uint32_t port = toUInt(params[0]);
    Victor* temp = new Victor(port);
    return temp;

}

void ScriptWPILib::victorRecycler(void* ptr)
{
    delete (Victor*)ptr;

}

void* ScriptWPILib::jaguarFactory(std::vector<std::string> params, void* data)
{
    uint32_t port = toUInt(params[0]);
    Jaguar* temp = new Jaguar(port);
    return temp;

}

void ScriptWPILib::jaguarRecycler(void* ptr)
{
    delete (Jaguar*)ptr;

}

void* ScriptWPILib::talonFactory(std::vector<std::string> params, void* data)
{
    uint32_t port = toUInt(params[0]);
    Talon* temp = new Talon(port);
    return temp;

}

void ScriptWPILib::talonRecycler(void* ptr)
{
    delete (Talon*)ptr;

}

void* ScriptWPILib::counterFactory(std::vector<std::string> params, void* data)
{
    uint32_t port = toUInt(params[0]);
    Counter* temp = new Counter(port);
    return temp;

}

void ScriptWPILib::counterRecycler(void* ptr)
{
    delete (Counter*)ptr;

}

void* ScriptWPILib::encoderFactory(std::vector<std::string> params, void* data)
{
    uint32_t port1 = toUInt(params[0]);
    uint32_t port2 = toUInt(params[1]);
    Encoder* temp = new Encoder(port1, port2);
    return temp;

}

void ScriptWPILib::encoderRecycler(void* ptr)
{
    delete (Encoder*)ptr;

}

void* ScriptWPILib::analogChannelFactory(std::vector<std::string> params, void* data)
{
    uint32_t port = toUInt(params[0]);
    AnalogChannel* temp = new AnalogChannel(port);
    return temp;

}

void ScriptWPILib::analogChannelRecycler(void* ptr)
{
    delete (AnalogChannel*)ptr;

}

void* ScriptWPILib::digitalInputFactory(std::vector<std::string> params, void* data)
{
    uint32_t port = toUInt(params[0]);
    DigitalInput* temp = new DigitalInput(port);
    return temp;

}

void ScriptWPILib::digitalInputRecycler(void* ptr)
{
    delete (DigitalInput*)ptr;

}

void* ScriptWPILib::timerFactory(std::vector<std::string> params, void* data)
{
    Timer* temp = new Timer;
    return temp;

}

void ScriptWPILib::timerRecycler(void* ptr)
{
    delete (Timer*)ptr;

}

void* ScriptWPILib::compressorFactory(std::vector<std::string> params, void* data)
{
    uint32_t port1 = toUInt(params[0]);
    uint32_t port2 = toUInt(params[1]);
    Compressor* temp = new Compressor(port1, port2);
    return temp;

}

void ScriptWPILib::compressorRecycler(void* ptr)
{
    delete (Compressor*)ptr;

}

void* ScriptWPILib::robotDriveFactory(std::vector<std::string> params, void* data)
{
    std::cout << "creating a robot drive\n";
    ScriptPackage* package = (ScriptPackage*)data;

    std::vector<SpeedController*> controllers;

    for(unsigned int i = 0; i < params.size(); i++)
    {
        GlobalProperty* property = package->getProperty(params[i]);
        if(property == NULL)
        {
            std::cout << "property is null\n";
            return NULL;

        }
        else if(property->getType() == "Victor")
        {
            Victor* c = (Victor*)property->getPtr();
            controllers.push_back((SpeedController*)c);

        }
        else if(property->getType() == "Jaguar")
        {
            Jaguar* c = (Jaguar*)property->getPtr();
            controllers.push_back((SpeedController*)c);

        }
        else if(property->getType() == "Talon")
        {
            Talon* c = (Talon*)property->getPtr();
            controllers.push_back((SpeedController*)c);

        }

    }

    RobotDrive* temp = NULL;
    if(controllers.size() == 2)
    {
        std::cout << "two motors\n";
        temp = new RobotDrive(controllers[0], controllers[1]);
       	temp->SetExpiration(1);
	
	return temp;

    }
    else if(controllers.size() == 4)
    {
        std::cout << "four motors\n";
        temp = new RobotDrive(controllers[0], controllers[1], controllers[2], controllers[3]);
	temp->SetExpiration(1);

        return temp;

    }

    std::cout << "returning null\n";
    return NULL;

}

void ScriptWPILib::robotDriveRecycler(void* ptr)
{
    delete (RobotDrive*)ptr;

}
