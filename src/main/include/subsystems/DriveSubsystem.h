#ifndef _2165_DRIVESUBSYSTEM_H_
#define _2165_DRIVESUBSYSTEM_H_

#include <utility>

#include <frc2/command/SubsystemBase.h>
#include <ctre/Phoenix.h>
#include <frc/SpeedControllerGroup.h>
#include <frc/drive/DifferentialDrive.h>
#include <frc/Ultrasonic.h>

#include <AHRS.h>


class DriveSubsystem : public frc2::SubsystemBase {
private:
    WPI_TalonSRX rightFront;
    WPI_TalonSRX rightRear;
    WPI_TalonSRX leftFront;
    WPI_TalonSRX leftRear;

    frc::SpeedControllerGroup rightMotors;
    frc::SpeedControllerGroup leftMotors;

    frc::DifferentialDrive drivetrain;

    AHRS gyro;

public: 
    DriveSubsystem();
    DriveSubsystem(int rf, int rr, int lf, int lr);

    void arcadeDrive(double speed, double rotation);
    void turnToAngle(double angle);
    void turnToAngleAtSpeed(double angle, double speed);

    double getAngle();
    bool atAngle(double angle);

    void setMaxSpeed(double max);
    void resetGyro();
};



#endif