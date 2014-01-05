#ifndef SCRIPTWPILIB_H_
#define SCRIPTWPILIB_H_

#include <angelscript.h>
#include <string>

#ifdef FAKEWPILIB
#include "FakeWPILib/FakeWPILib.h"
#else
#include "ScriptRobot.h"
#include <WPILib.h>
#endif

/*
Angelscript WPILib bindings for:

handles are c++ object types that can not be created in the script program
but must be instead passed in through a call to a angelscript c++ bound function
or hook

objects are c++ object types that can be initialized in the script program

Joystick handle
Servo handle
Relay handle
Solenoid handle
DoubleSolenoid handle
Victor handle
Jaguar handle
Talon handle
Robot handle

Timer object

*/

//wraps all the wpilib drivers stations into one useable class
class asDriverStation
{
public:
    asDriverStation();

    //LCD
    void print(int line, int column, std::string msg);
    void print(int line, std::string msg);
    void clear();

    //DriverStation FMS TODO: this
    /*float getStickAxis(unsigned int stick, unsigned int axis);
    int getStickButtons(unsigned int stick);

    float getAnalogIn(unsigned int chan);
    bool getDigitalIn(unsigned int chan);
    void setDigitalOut(unsigned int chan, bool value);
    bool getDigitalOut(unsigned int chan);

    bool isEnabled();
    bool isDisabled();
    bool isAutonomous();
    bool isTest();
    bool isNewControlData();
    bool isFMSAttached();

    int getAlliance();
    unsigned int getLocation();
    float getMatchTime();
    float getBatteryVoltage();
    unsigned int getTeamNumber();
    */

    //Enhanced IO TODO: this
    /*
    float getAcceleration(unsigned int chan);
    float getAnalogIn(unsigned int chan);
    float getAnalogInRation(unsigned int chan);
    float getAnalogOut(unsigned int chan);
    void setAnalogOut(unsigned int chan, float value);
    bool getButton(unsigned int chan);
    unsigned int getButtons();
    void setLED(unsigned int chan, bool value);
    void setLEDs(unsigned int value);
    bool getDigital(unsigned int chan);
    */

    DriverStationLCD* lcd;

};

void registerWPILib(asIScriptEngine* engine);

void registerJoystick(asIScriptEngine* engine);
void registerControllers(asIScriptEngine* engine);
void registerSensors(asIScriptEngine* engine);
void registerTimer(asIScriptEngine* engine);
void registerDriverStation(asIScriptEngine* engine);
void registerRobotDrive(asIScriptEngine* engine);

#endif /* SCRIPTWPILIB_H_ */
