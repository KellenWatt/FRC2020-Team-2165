#ifndef _2165_ARMSUBSYSTEM_H_
#define _2165_ARMSUBSYSTEM_H_


#include <frc2/command/SubsystemBase.h>
#include <ctre/Phoenix.h>
#include <frc/DigitalInput.h>
#include <frc/Servo.h>


class ArmSubsystem : public frc2::SubsystemBase {
private:

    WPI_TalonSRX armMotor;

    frc::DigitalInput armsUp;
    frc::DigitalInput armsDown;

    frc::Servo hookReleaseLeft;
    frc::Servo hookReleaseRight;

public:

    ArmSubsystem();
    ArmSubsystem(int motor, int up, int down);
    ArmSubsystem(int motor, int up, int down, int hookLeft, int hookRight);

    void raise(double speed);
    void lower(double speed);
    void fullStop();

    void releaseHook(bool release);

    bool fullRaised();
    bool fullLowered();
    bool middling();
};

#endif