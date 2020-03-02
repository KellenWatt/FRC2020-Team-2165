#ifndef _2165_ARMSUBSYSTEM_H_
#define _2165_ARMSUBSYSTEM_H_


#include <frc2/command/SubsystemBase.h>
#include <ctre/Phoenix.h>
#include <frc/DigitalInput.h>
#include <frc/Servo.h>


class ArmSubsystem : public frc2::SubsystemBase {
private:

    WPI_TalonSRX armMotor;

    // May not be necessary, as switches might be directly wired to Talon. 
    // Keeping because it literally can't hurt, as long as they're `false` by default
/*
    frc::Servo hookReleaseLeft;
    frc::Servo hookReleaseRight;
*/
public:

    ArmSubsystem(int motor);

    void raise(double speed);
    void lower(double speed);
    void fullStop();

//    void releaseHook(bool release);

};

#endif