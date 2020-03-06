#ifndef _2165_DRIVESUBSYSTEM_H_
#define _2165_DRIVESUBSYSTEM_H_

#include <utility>

#include <frc2/command/SubsystemBase.h>
#include <ctre/Phoenix.h>
#include <frc/SpeedControllerGroup.h>
#include <frc/drive/DifferentialDrive.h>
#include <frc/Encoder.h>

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
    frc::Encoder encoder;

public: 
    DriveSubsystem(int rf, int rr, int lf, int lr, std::pair<int,int> encoderChannels);

    void arcadeDrive(double speed, double rotation);
    void adjustedArcadeDrive(double speed, double rotation);
    void turnToAngle(double angle);
    void turnToAngleAtSpeed(double angle, double speed);

    double getAngle();
    bool atAngle(double angle);

    double getDistance();

    void setMaxSpeed(double max);
    void resetGyro();
    void resetEncoder();
};



#endif