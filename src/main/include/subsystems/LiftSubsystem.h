#ifndef _2165_LIFTSUBSYSTEM_H_
#define _2165_LIFTSUBSYSTEM_H_

#include <frc2/command/SubsystemBase.h>
#include <ctre/Phoenix.h>
#include "components/Ultrasonic.h"

#include <utility>

class LiftSubsystem : public frc2::SubsystemBase {
private:
    WPI_TalonSRX winch;
    Ultrasonic heightSonar;
public:

    LiftSubsystem(int winchChannel, std::pair<int,int> sonarChannels);

    void raise(double speed);
    void lower(double speed);
    void stop();
    double getHeight();

}

#endif