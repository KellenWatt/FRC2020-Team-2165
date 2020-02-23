#ifndef _2165_LOADSUBSYSTEM_H_
#define _2165_LOADSUBSYSTEM_H_

#include <frc2/command/SubsystemBase.h>
#include <frc/DoubleSolenoid.h>
#include <ctre/Phoenix.h>


#include <utility>

class LoadSubsystem : public frc2::SubsystemBase {
private:
    WPI_TalonSRX loadBelt;
    WPI_TalonSRX loaderArm;
    frc::DoubleSolenoid armMover;
public:

    void lowerCaptureArm(bool lowered);
    void enableBelt(bool enabled);
    void enableCaptureArm(bool enabled);

    bool armLowered();
    bool beltEnabled();
    bool armEnabled();
};

#endif