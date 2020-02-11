#ifndef _2165_DRIVESUBSYSTEM_H_
#define _2165_DRIVESUBSYSTEM_H_


#include <frc2/command/SubsystemBase.h>
#include <ctre/Phoenix.h>
#include <frc/SpeedControllerGroup.h>
#include <frc/drive/DifferentialDrive.h>

class DriveSubsystem : public frc2::SubsystemBase {
private:
    WPI_TalonSRX rightFront;
    WPI_TalonSRX rightRear;
    WPI_TalonSRX leftFront;
    WPI_TalonSRX leftRear;

    frc::SpeedControllerGroup rightMotors;
    frc::SpeedControllerGroup leftMotors;

    frc::DifferentialDrive drivetrain;

};



#endif