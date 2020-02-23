#ifndef _2165_LOADSUBSYSTEM_H_
#define _2165_LOADSUBSYSTEM_H_

#include <frc2/command/SubsystemBase.h>
#include <frc/DoubleSolenoid.h>
#include <ctre/Phoenix.h>


#include <utility>

class LoadSubsystem : public frc2::SubsystemBase {
private:
    WPI_TalonSRX loaderBelt;
    WPI_TalonSRX captureRoller;
    frc::DoubleSolenoid captureArm;

    double beltSpeed;
    double captureRollerSpeed;
public:
    LoadSubsystem(int beltChannel, int loaderArmChannel, std::pair<int,int> captureArmChannels);

    void lowerCaptureArm(bool lowered);
    void enableBelt(bool enabled);
    void enableCaptureRoller(bool enabled);

    void setBeltSpeed(double speed);
    void setCaptureRollerSpeed(double speed);

    bool armLowered();
};

#endif