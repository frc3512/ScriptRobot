#ifndef FAKEWPILIB_H_
#define FAKEWPILIB_H_

//For compiling on systems that
//don't support wpilib/vxworks stuff
//like the windows/linux for making
//deployer, packaging software
//CUATION CLOSE YOUR EYES THIS IS A HACK

class GenericHID
{
public:
    GenericHID(int){}

};

class Joystick : public GenericHID
{
public:
    Joystick(int){}
    float GetX(){return 0;}
    float GetY(){return 0;}
    float GetZ(){return 0;}
    float GetTwist(){return 0;}
    float GetThrottle(){return 0;}

    typedef enum
    {
        kXAxis, kYAxis, kZAxis, kTwistAxis, kThrottleAxis, kNumAxisTypes
    } AxisType;

    float GetRawAxis(int){return 0;}
    bool GetTrigger(){return false;}
    bool GetRawButton(unsigned int){return false;}
    float GetMagnitude(){return 0;}
    float GetDirectionRadians(){return 0;}
    float GetDirectionDegrees(){return 0;}

};

class SetGet
{
public:
    SetGet(){}

    void Set(float){}
    float Get(){return 0;}

};

class Servo : public SetGet
{
public:
    Servo(int){}

    void SetAngle(float){}
    float GetAngle(){return 0;}

};

class Relay
{
public:
    Relay(int){}

    typedef enum {kOff, kOn, kForward, kReverse} Value;

    void Set(int){}
    int Get(){return 0;}

};

class Solenoid
{
public:
    Solenoid(int){}

    void Set(bool){}
    bool Get(){return false;}

};

class DoubleSolenoid
{
public:
    DoubleSolenoid(int, int){}

    typedef enum {kOff, kForward, kReverse} Value;

    void Set(int){}
    int Get(){return 0;}

};

class Victor : public SetGet
{
public:
    Victor(int){}

};

class Jaguar : public SetGet
{
public:
    Jaguar(int){}

};

class Talon : public SetGet
{
public:
    Talon(int){}

};

class Counter
{
public:
    Counter(int){}

    void Start(){}
    int Get(){return 0;}
    void Reset(){}
    void Stop(){}
    float GetPeriod(){return 0;}
    void SetMaxPeriod(float){}
    void SetUpdateWhenEmpty(bool){}
    bool GetStopped(){return 0;}
    bool GetDirection(){return 0;}
    unsigned int GetIndex(){return 0;}

};

class Encoder : public Counter
{
public:
    Encoder(int, int){}

    int GetRaw(){return 0;}
    float GetDistance(){return 0;}
    float GetRate(){return 0;}
    void SetMinRate(float){}
    void SetDistancePerPulse(float){}
    void SetReverseDirection(bool){}
    float PIDGet(){return 0;}

};

class Timer
{
public:
    Timer(){}
    float Get(){return 0;}
    void Reset(){}
    void Start(){}
    void Stop(){}
    bool HasPeriodPassed(float){return false;}

};

class RobotDrive
{
public:
    RobotDrive(){}
    RobotDrive(int, int){}
    RobotDrive(int, int, int, int){}

    typedef enum
    {
        kFrontLeftMotor = 0,
        kFrontRightMotor = 1,
        kRearLeftMotor = 2,
        kRearRightMotor = 3
    } MotorType;

    void SetInvertedMotor(int, bool){}
    void SetSensitivity(float){}
    void SetMaxOutput(float){}
    void TankDrive(GenericHID*, GenericHID*, bool squareInputs = true){}
    void TankDrive(GenericHID*, unsigned int, GenericHID*, unsigned int, bool squareInputs = true){}
    void ArcadeDrive(GenericHID*, bool squareInputs = true){}
    void ArcadeDrive(GenericHID*, unsigned int, GenericHID*, unsigned int, bool squareInputs = true){}
    void MecanumDrive_Cartesian(float, float, float, float){}
    void MecanumDrive_Polar(float, float, float){}
    void HolonomicDrive(float, float, float){}

};

#endif /* FAKEWPILIB_H_ */
