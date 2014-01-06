#include "scriptwpilib.h"
#include "add_on/vxw_assert.h"

#ifdef FAKEWPILIB
#include "FakeWPILib/FakeWPILib.h"
#else
#include "ScriptRobot.h"
#include <WPILib.h>
#endif

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

void registerWPILib(asIScriptEngine* engine)
{
    registerJoystick(engine);
    registerControllers(engine);
    registerSensors(engine);
    registerTimer(engine);
    registerDriverStation(engine);
    registerCompressor(engine);
    registerRobotDrive(engine);

    engine->RegisterObjectType("Robot", sizeof(ScriptRobot), asOBJ_REF | asOBJ_NOCOUNT);

}

void registerJoystick(asIScriptEngine* engine)
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

void registerControllers(asIScriptEngine* engine)
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

void registerSensors(asIScriptEngine* engine)
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

void registerTimer(asIScriptEngine* engine) //TODO: fix / test this
{
    ///// Timer /////
    /*assert(engine->RegisterObjectType("Timer", sizeof(Timer), asOBJ_REF | asOBJ_NOCOUNT) >= 0);

    assert(engine->RegisterObjectMethod("Timer", "float get()", asMETHOD(Timer, Get), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("Timer", "void reset()", asMETHOD(Timer, Reset), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("Timer", "void start()", asMETHOD(Timer, Start), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("Timer", "void stop()", asMETHOD(Timer, Stop), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("Timer", "bool hasTimePassed(float)", asMETHOD(Timer, HasPeriodPassed), asCALL_THISCALL) >= 0);
*/

}

void registerDriverStation(asIScriptEngine* engine)
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

void registerCompressor(asIScriptEngine* engine)
{
    assert(engine->RegisterObjectType("Compressor", sizeof(Compressor), asOBJ_REF | asOBJ_NOCOUNT) >= 0);

    assert(engine->RegisterObjectMethod("Compressor", "void start()", asMETHOD(Compressor, Start), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("Compressor", "void stop()", asMETHOD(Compressor, Stop), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("Compressor", "bool enabled()", asMETHOD(Compressor, Enabled), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("Compressor", "uint getPressureSwitchValue()", asMETHOD(Compressor, GetPressureSwitchValue), asCALL_THISCALL) >= 0);

}

void registerRobotDrive(asIScriptEngine* engine)
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
    assert(engine->RegisterObjectMethod("RobotDrive", "void tankDrive(Joystick@, uint, Joystick@, uint, bool squareInputs = true)", asMETHODPR(RobotDrive, TankDrive, (GenericHID*, unsigned int, GenericHID*, unsigned int, bool), void), asCALL_THISCALL) >= 0);

    ///// ArcadeDrive /////
    assert(engine->RegisterObjectMethod("RobotDrive", "void arcadeDrive(Joystick@, bool squareInputs = true)", asMETHODPR(RobotDrive, ArcadeDrive, (GenericHID*, bool), void), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("RobotDrive", "void arcadeDrive(Joystick@, uint, Joystick@, uint, bool squareInputs = true)", asMETHODPR(RobotDrive, ArcadeDrive, (GenericHID*, unsigned int, GenericHID*, unsigned int, bool), void), asCALL_THISCALL) >= 0);

    ///// MecanumDrive /////
    assert(engine->RegisterObjectMethod("RobotDrive", "void mecanumDrive(float, float, float, float)", asMETHOD(RobotDrive, MecanumDrive_Cartesian), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("RobotDrive", "void mecanumDrive(float, float, float)", asMETHOD(RobotDrive, MecanumDrive_Polar), asCALL_THISCALL) >= 0);

    ///// HolonomicDrive /////
    assert(engine->RegisterObjectMethod("RobotDrive", "void holonomicDrive(float, float, float)", asMETHOD(RobotDrive, HolonomicDrive), asCALL_THISCALL) >= 0);

}
