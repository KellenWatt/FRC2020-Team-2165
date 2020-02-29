#ifndef _2165_LIFTSUBSYSTEM_H_
#define _2165_LIFTSUBSYSTEM_H_

#include <frc2/command/SubsystemBase.h>
#include <frc/AnalogInput.h>
#include <ctre/Phoenix.h>

#include "components/Ultrasonic.h"

#include <utility>

class LiftSubsystem : public frc2::SubsystemBase {
private:
    WPI_TalonSRX winch;
    //Ultrasonic heightSonar;
    frc::AnalogInput heightIR;
public:

    LiftSubsystem(int winchChannel, int sonarChannel);

    void raise(double speed);
    void lower(double speed);
    void stop();
    double getHeight();

};

#endif