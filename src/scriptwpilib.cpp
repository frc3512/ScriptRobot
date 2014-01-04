#include "scriptwpilib.h"
#include "add_on/vxw_assert.h"

#ifdef FAKEWPILIB
#include "FakeWPILib/FakeWPILib.h"
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

}

void registerTimer(asIScriptEngine* engine)
{
    ///// Timer /////
    assert(engine->RegisterObjectType("Timer", sizeof(Timer), asOBJ_VALUE | asOBJ_POD) >= 0);

    assert(engine->RegisterObjectMethod("Timer", "float get()", asMETHOD(Timer, Get), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("Timer", "void reset()", asMETHOD(Timer, Reset), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("Timer", "void start()", asMETHOD(Timer, Start), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("Timer", "void stop()", asMETHOD(Timer, Stop), asCALL_THISCALL) >= 0);
    assert(engine->RegisterObjectMethod("Timer", "bool hasTimePassed(float)", asMETHOD(Timer, HasPeriodPassed), asCALL_THISCALL) >= 0);

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
