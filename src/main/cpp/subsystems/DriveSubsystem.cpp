#include "subsystems/DriveSubsystem.h"

#include <frc/SPI.h>
#include <AHRS.h>

#include <utility>
#include <algorithm>
#include <cmath>

DriveSubsystem::DriveSubsystem() : DriveSubsystem::DriveSubsystem(0,0,0,0) {}

DriveSubsystem::DriveSubsystem(int rf, int rr, int lf, int lr) :
        rightFront(rf), rightRear(rr), leftFront(lf), leftRear(lr), rightMotors(rightFront, rightRear), 
        leftMotors(leftFront, leftRear), drivetrain(leftMotors, rightMotors), gyro(frc::SPI::Port::kMXP)  {
// Do nothing
}

void DriveSubsystem::arcadeDrive(double speed, double rotation) {
    this->drivetrain.ArcadeDrive(speed, rotation);
}

void DriveSubsystem::turnToAngle(double angle) {
   this->turnToAngleAtSpeed(angle, 0.5); 
}

void DriveSubsystem::turnToAngleAtSpeed(double angle, double speed) {
    double diff = angle - gyro.GetAngle();

    if(abs(diff) > 0.5) {
        this->drivetrain.ArcadeDrive(0, (angle < 0 ? -1 : 1) * speed);
    }
}

double DriveSubsystem::getAngle() {
    return this->gyro.GetAngle();
}

void DriveSubsystem::setMaxSpeed(double max) {
    this->drivetrain.SetMaxOutput(max);
}

void DriveSubsystem::resetGyro() {
    this->gyro.Reset();
}
