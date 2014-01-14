#ifndef SCRIPTWPILIB_H_
#define SCRIPTWPILIB_H_

#include "ScriptPlugin.h"
#include "GlobalProperty.h"
#include <vector>

#if defined _WIN32 || UNIX
//TODO: change the name of this include
#include "FakeWPILib/FakeWPILib.h"
#include <assert.h>
#include <stdint.h>
#else
#include "ScriptRobot.h"
#include <WPILib.h>
#include "add_on/vxw_assert.h"
#include <types/vxTypes.h>
#endif

class asDriverStation
{
public:
    asDriverStation();

    //LCD
    void print(int line, int column, std::string msg);
    void print(int line, std::string msg);
    void clear();

    //DriverStation FMS TODO: this
    float getStickAxis(unsigned int stick, unsigned int axis);
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

    DriverStationLCD* m_lcd;
    DriverStation* m_ds;

};

class ScriptWPILib : public ScriptPlugin
{
public:
    ScriptWPILib();

    std::string getName();

    static void registerWPILib(asIScriptEngine* engine);
    static void registerJoystick(asIScriptEngine* engine);
    static void registerControllers(asIScriptEngine* engine);
    static void registerSensors(asIScriptEngine* engine);

    static void registerTimer(asIScriptEngine* engine);
    static void timerConstructor(void* memory);
    static void timerDestructor(void* memory);

    static void registerDriverStation(asIScriptEngine* engine);
    static void registerCompressor(asIScriptEngine* engine);
    static void registerRobotDrive(asIScriptEngine* engine);

    static void* joystickFactory(std::vector<std::string> params, void* data);
    static void joystickRecycler(void* ptr);

    static void* servoFactory(std::vector<std::string> params, void* data);
    static void servoRecycler(void* ptr);

    static void* relayFactory(std::vector<std::string> params, void* data);
    static void relayRecycler(void* ptr);

    static void* solenoidFactory(std::vector<std::string> params, void* data);
    static void solenoidRecycler(void* ptr);

    static void* doubleSolenoidFactory(std::vector<std::string> params, void* data);
    static void doubleSolenoidRecycler(void* ptr);

    static void* victorFactory(std::vector<std::string> params, void* data);
    static void victorRecycler(void* ptr);

    static void* jaguarFactory(std::vector<std::string> params, void* data);
    static void jaguarRecycler(void* ptr);

    static void* talonFactory(std::vector<std::string> params, void* data);
    static void talonRecycler(void* ptr);

    static void* counterFactory(std::vector<std::string> params, void* data);
    static void counterRecycler(void* ptr);

    static void* encoderFactory(std::vector<std::string> params, void* data);
    static void encoderRecycler(void* ptr);

    static void* analogChannelFactory(std::vector<std::string> params, void* data);
    static void analogChannelRecycler(void* ptr);

    static void* digitalInputFactory(std::vector<std::string> params, void* data);
    static void digitalInputRecycler(void* ptr);

    static void* timerFactory(std::vector<std::string> params, void* data);
    static void timerRecycler(void* ptr);

    static void* compressorFactory(std::vector<std::string> params, void* data);
    static void compressorRecycler(void* ptr);

    static void* robotDriveFactory(std::vector<std::string> params, void* data);
    static void robotDriveRecycler(void* ptr);

private:
    void onInitFactories();
    void onInitBindings();

    void onDeinitFactories();
    void onDeinitBindings();

    GlobalProperty* m_driverStation;

};

#endif /* SCRIPTWPILIB_H_ */
