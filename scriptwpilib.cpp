#include "scriptwpilib.h"
#include "ScriptRobot.h"

#ifdef FAKEWPILIB
#include "../FakeWPILib/FakeWPILib.h"
#else
#include <WPILib.h>
#endif

void registerWPILib(asIScriptEngine* engine)
{
    registerJoystick(engine);
    registerControllers(engine);
    registerSensors(engine);
    registerTimer(engine);
    registerRobotDrive(engine);

    engine->RegisterObjectType("Robot", sizeof(ScriptRobot), asOBJ_REF | asOBJ_NOCOUNT);

}

void registerJoystick(asIScriptEngine* engine)
{
    ///// Joystick /////
    engine->RegisterObjectType("Joystick", sizeof(Joystick), asOBJ_REF | asOBJ_NOCOUNT);

    engine->RegisterObjectMethod("Joystick", "float getX()", asMETHOD(Joystick, GetX), asCALL_THISCALL);
    engine->RegisterObjectMethod("Joystick", "float getY()", asMETHOD(Joystick, GetY), asCALL_THISCALL);
    engine->RegisterObjectMethod("Joystick", "float getZ()", asMETHOD(Joystick, GetZ), asCALL_THISCALL);

    engine->RegisterObjectMethod("Joystick", "float getTwist()", asMETHOD(Joystick, GetTwist), asCALL_THISCALL);
    engine->RegisterObjectMethod("Joystick", "float getThrottle()", asMETHOD(Joystick, GetThrottle), asCALL_THISCALL);

    engine->RegisterEnum("Joystick_Axis");
    engine->RegisterEnumValue("Joystick_Axis", "x", Joystick::kXAxis);
    engine->RegisterEnumValue("Joystick_Axis", "y", Joystick::kYAxis);
    engine->RegisterEnumValue("Joystick_Axis", "z", Joystick::kZAxis);
    engine->RegisterEnumValue("Joystick_Axis", "twist", Joystick::kTwistAxis);
    engine->RegisterEnumValue("Joystick_Axis", "throttle", Joystick::kThrottleAxis);

    engine->RegisterObjectMethod("Joystick", "float getAxis(Joystick_Axis)", asMETHOD(Joystick, GetRawAxis), asCALL_THISCALL);

    engine->RegisterObjectMethod("Joystick", "bool getTrigger()", asMETHOD(Joystick, GetTrigger), asCALL_THISCALL);
    engine->RegisterObjectMethod("Joystick", "bool getButton(uint)", asMETHOD(Joystick, GetRawButton), asCALL_THISCALL);

    engine->RegisterObjectMethod("Joystick", "float getMagnitude()", asMETHOD(Joystick, GetMagnitude), asCALL_THISCALL);
    engine->RegisterObjectMethod("Joystick", "float getDirectionRadians()", asMETHOD(Joystick, GetDirectionRadians), asCALL_THISCALL);
    engine->RegisterObjectMethod("Joystick", "float getDirectionDegrees()", asMETHOD(Joystick, GetDirectionDegrees), asCALL_THISCALL);

}

void registerControllers(asIScriptEngine* engine)
{
    ///// Servo /////
    engine->RegisterObjectType("Servo", sizeof(Servo), asOBJ_REF | asOBJ_NOCOUNT);

    engine->RegisterObjectMethod("Servo", "void set(float)", asMETHOD(Servo, Set), asCALL_THISCALL);
    engine->RegisterObjectMethod("Servo", "float get()", asMETHOD(Servo, Get), asCALL_THISCALL);

    engine->RegisterObjectMethod("Servo", "void setAngle(float)", asMETHOD(Servo, SetAngle), asCALL_THISCALL);
    engine->RegisterObjectMethod("Servo", "float getAngle()", asMETHOD(Servo, GetAngle), asCALL_THISCALL);

    ///// Relay /////
    engine->RegisterObjectType("Relay", sizeof(Relay), asOBJ_REF | asOBJ_NOCOUNT);

    engine->RegisterEnum("Relay_Value");
    engine->RegisterEnumValue("Relay_Value", "off", Relay::kOff);
    engine->RegisterEnumValue("Relay_Value", "on", Relay::kOn);
    engine->RegisterEnumValue("Relay_Value", "forward", Relay::kForward);
    engine->RegisterEnumValue("Relay_Value", "reverse", Relay::kReverse);

    engine->RegisterObjectMethod("Relay", "void set(Relay_Value)", asMETHOD(Relay, Set), asCALL_THISCALL);
    engine->RegisterObjectMethod("Relay", "Relay_Value get()", asMETHOD(Relay, Get), asCALL_THISCALL);

    ///// Solenoid /////
    engine->RegisterObjectType("Solenoid", sizeof(Solenoid), asOBJ_REF | asOBJ_NOCOUNT);

    engine->RegisterObjectMethod("Solenoid", "void set(bool)", asMETHOD(Solenoid, Set), asCALL_THISCALL);
    engine->RegisterObjectMethod("Solenoid", "bool get()", asMETHOD(Solenoid, Get), asCALL_THISCALL);

    ///// Double Solenoid /////
    engine->RegisterObjectType("DoubleSolenoid", sizeof(DoubleSolenoid), asOBJ_REF | asOBJ_NOCOUNT);

    engine->RegisterEnum("DoubleSolenoid_Value");
    engine->RegisterEnumValue("DoubleSolenoid_Value", "off", DoubleSolenoid::kOff);
    engine->RegisterEnumValue("DoubleSolenoid_Value", "forward", DoubleSolenoid::kForward);
    engine->RegisterEnumValue("DoubleSolenoid_Value", "reverse", DoubleSolenoid::kReverse);

    engine->RegisterObjectMethod("DoubleSolenoid", "void set(DoubleSolenoid_Value)", asMETHOD(DoubleSolenoid, Set), asCALL_THISCALL);
    engine->RegisterObjectMethod("DoubleSolenoid", "DoubleSolenoid_Value get()", asMETHOD(DoubleSolenoid, Get), asCALL_THISCALL);

    ///// Victor /////
    engine->RegisterObjectType("Victor", sizeof(Victor), asOBJ_REF | asOBJ_NOCOUNT);

    engine->RegisterObjectMethod("Victor", "void set(float)", asMETHOD(Victor, Set), asCALL_THISCALL);
    engine->RegisterObjectMethod("Victor", "float get()", asMETHOD(Victor, Get), asCALL_THISCALL);

    ///// Jaguar /////
    engine->RegisterObjectType("Jaguar", sizeof(Jaguar), asOBJ_REF | asOBJ_NOCOUNT);

    engine->RegisterObjectMethod("Jaguar", "void set(float)", asMETHOD(Jaguar, Set), asCALL_THISCALL);
    engine->RegisterObjectMethod("Jaguar", "float get()", asMETHOD(Jaguar, Get), asCALL_THISCALL);

    ///// Talon /////
    engine->RegisterObjectType("Talon", sizeof(Talon), asOBJ_REF | asOBJ_NOCOUNT);

   engine->RegisterObjectMethod("Talon", "void set(float)", asMETHOD(Talon, Set), asCALL_THISCALL);
   engine->RegisterObjectMethod("Talon", "float get()", asMETHOD(Talon, Get), asCALL_THISCALL);

}

void registerSensors(asIScriptEngine* engine)
{
    ///// Counter /////
    engine->RegisterObjectType("Counter", sizeof(Counter), asOBJ_REF | asOBJ_NOCOUNT);

    engine->RegisterObjectMethod("Counter", "void start()", asMETHOD(Counter, Start), asCALL_THISCALL);
    engine->RegisterObjectMethod("Counter", "int get()", asMETHOD(Counter, Get), asCALL_THISCALL);
    engine->RegisterObjectMethod("Counter", "void reset()", asMETHOD(Counter, Reset), asCALL_THISCALL);
    engine->RegisterObjectMethod("Counter", "void stop()", asMETHOD(Counter, Stop), asCALL_THISCALL);
    engine->RegisterObjectMethod("Counter", "float getPeriod()", asMETHOD(Counter, GetPeriod), asCALL_THISCALL);
    engine->RegisterObjectMethod("Counter", "void setMaxPeriod(float)", asMETHOD(Counter, SetMaxPeriod), asCALL_THISCALL);
    engine->RegisterObjectMethod("Counter", "void setUpdateWhenEmpty(bool)", asMETHOD(Counter, SetUpdateWhenEmpty), asCALL_THISCALL);
    engine->RegisterObjectMethod("Counter", "bool getStopped()", asMETHOD(Counter, GetStopped), asCALL_THISCALL);
    engine->RegisterObjectMethod("Counter", "bool getDirection()", asMETHOD(Counter, GetDirection), asCALL_THISCALL);
    engine->RegisterObjectMethod("Counter", "uint getIndex()", asMETHOD(Counter, GetIndex), asCALL_THISCALL);

    ///// Encoder /////
    engine->RegisterObjectType("Encoder", sizeof(Encoder), asOBJ_REF | asOBJ_NOCOUNT);

    engine->RegisterObjectMethod("Encoder", "void start()", asMETHOD(Encoder, Start), asCALL_THISCALL);
    engine->RegisterObjectMethod("Encoder", "int get()", asMETHOD(Encoder, Get), asCALL_THISCALL);
    engine->RegisterObjectMethod("Encoder", "int getRaw()", asMETHOD(Encoder, GetRaw), asCALL_THISCALL);
    engine->RegisterObjectMethod("Encoder", "void reset()", asMETHOD(Encoder, Reset), asCALL_THISCALL);
    engine->RegisterObjectMethod("Encoder", "void stop()", asMETHOD(Encoder, Stop), asCALL_THISCALL);
    engine->RegisterObjectMethod("Encoder", "float getPeriod()", asMETHOD(Encoder, GetPeriod), asCALL_THISCALL);
    engine->RegisterObjectMethod("Encoder", "void setMaxPeriod(float)", asMETHOD(Encoder, SetMaxPeriod), asCALL_THISCALL);
    engine->RegisterObjectMethod("Encoder", "bool getStopped()", asMETHOD(Encoder, GetStopped), asCALL_THISCALL);
    engine->RegisterObjectMethod("Encoder", "bool getDirection()", asMETHOD(Encoder, GetDirection), asCALL_THISCALL);
    engine->RegisterObjectMethod("Encoder", "float getDistance()", asMETHOD(Encoder, GetDistance), asCALL_THISCALL);
    engine->RegisterObjectMethod("Encoder", "float getRate()", asMETHOD(Encoder, GetRate), asCALL_THISCALL);
    engine->RegisterObjectMethod("Encoder", "void setMinRate(float)", asMETHOD(Encoder, SetMinRate), asCALL_THISCALL);
    engine->RegisterObjectMethod("Encoder", "void setDistancePerPulse(float)", asMETHOD(Encoder, SetDistancePerPulse), asCALL_THISCALL);
    engine->RegisterObjectMethod("Encoder", "void setReverseDirection(bool)", asMETHOD(Encoder, SetReverseDirection), asCALL_THISCALL);
    engine->RegisterObjectMethod("Encoder", "float PIDGet()", asMETHOD(Encoder, PIDGet), asCALL_THISCALL);

}

void registerTimer(asIScriptEngine* engine)
{
    ///// Timer /////
    engine->RegisterObjectType("Timer", sizeof(Timer), asOBJ_VALUE | asOBJ_POD);

    engine->RegisterObjectMethod("Timer", "float get()", asMETHOD(Timer, Get), asCALL_THISCALL);
    engine->RegisterObjectMethod("Timer", "void reset()", asMETHOD(Timer, Reset), asCALL_THISCALL);
    engine->RegisterObjectMethod("Timer", "void start()", asMETHOD(Timer, Start), asCALL_THISCALL);
    engine->RegisterObjectMethod("Timer", "void stop()", asMETHOD(Timer, Stop), asCALL_THISCALL);
    engine->RegisterObjectMethod("Timer", "bool hasTimePassed(float)", asMETHOD(Timer, HasPeriodPassed), asCALL_THISCALL);

}

void registerRobotDrive(asIScriptEngine* engine)
{
    ///// Robot Drive /////
    engine->RegisterObjectType("RobotDrive", sizeof(RobotDrive), asOBJ_REF | asOBJ_NOCOUNT);

    engine->RegisterEnum("RobotDrive_Motor");
    engine->RegisterEnumValue("RobotDrive_Motor", "frontLeft", RobotDrive::kFrontLeftMotor);
    engine->RegisterEnumValue("RobotDrive_Motor", "frontRight", RobotDrive::kFrontRightMotor);
    engine->RegisterEnumValue("RobotDrive_Motor", "rearLeft", RobotDrive::kRearLeftMotor);
    engine->RegisterEnumValue("RobotDrive_Motor", "rearRight", RobotDrive::kRearRightMotor);

    engine->RegisterObjectMethod("RobotDrive", "void setInvertedMotor(RobotDrive_Motor, bool)", asMETHOD(RobotDrive, SetInvertedMotor), asCALL_THISCALL);
    engine->RegisterObjectMethod("RobotDrive", "void setSensitivity(float)", asMETHOD(RobotDrive, SetSensitivity), asCALL_THISCALL);
    engine->RegisterObjectMethod("RobotDrive", "void setMaxOutput(float)", asMETHOD(RobotDrive, SetMaxOutput), asCALL_THISCALL);

    ///// TankDrive /////
    engine->RegisterObjectMethod("RobotDrive", "void tankDrive(Joystick@, Joystick@, bool squareInputs = true)", asMETHODPR(RobotDrive, TankDrive, (GenericHID*, GenericHID*, bool), void), asCALL_THISCALL);
    engine->RegisterObjectMethod("RobotDrive", "void tankDrive(Joystick@, uint, Joystick@, uint, bool squareInputs = true)", asMETHODPR(RobotDrive, TankDrive, (GenericHID*, unsigned int, GenericHID*, unsigned int, bool), void), asCALL_THISCALL);

    ///// ArcadeDrive /////
    engine->RegisterObjectMethod("RobotDrive", "void arcadeDrive(Joystick@, bool squareInputs = true)", asMETHODPR(RobotDrive, ArcadeDrive, (GenericHID*, bool), void), asCALL_THISCALL);
    engine->RegisterObjectMethod("RobotDrive", "void arcadeDrive(Joystick@, uint, Joystick@, uint, bool squareInputs = true)", asMETHODPR(RobotDrive, ArcadeDrive, (GenericHID*, unsigned int, GenericHID*, unsigned int, bool), void), asCALL_THISCALL);

    ///// MecanumDrive /////
    engine->RegisterObjectMethod("RobotDrive", "void mecanumDrive(float, float, float, float)", asMETHOD(RobotDrive, MecanumDrive_Cartesian), asCALL_THISCALL);
    engine->RegisterObjectMethod("RobotDrive", "void mecanumDrive(float, float, float)", asMETHOD(RobotDrive, MecanumDrive_Polar), asCALL_THISCALL);

    ///// HolonomicDrive /////
    engine->RegisterObjectMethod("RobotDrive", "void holonomicDrive(float, float, float)", asMETHOD(RobotDrive, HolonomicDrive), asCALL_THISCALL);

}
