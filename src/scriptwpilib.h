#ifndef SCRIPTWPILIB_H_
#define SCRIPTWPILIB_H_

#include <angelscript.h>

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

void registerWPILib(asIScriptEngine* engine);

void registerJoystick(asIScriptEngine* engine);
void registerControllers(asIScriptEngine* engine);
void registerSensors(asIScriptEngine* engine);
void registerTimer(asIScriptEngine* engine);
void registerDriverStation(asIScriptEngine* engine);
void registerRobotDrive(asIScriptEngine* engine);

#endif /* SCRIPTWPILIB_H_ */
