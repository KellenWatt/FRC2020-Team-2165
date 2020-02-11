#include "subsystems/DriveSubsystem.h"

#include <utility>

DriveSubsystem::DriveSubsystem() : DriveSubsystem::DriveSubsystem(0,0,0,0, std::pair<int,int>(0,0)) {}

DriveSubsystem::DriveSubsystem(int rf, int rr, int lf, int lr, std::pair<int,int> us) :
        rightFront(rf), rightRear(rr), leftFront(lf), leftRear(lr), rightMotors(rightFront, rightRear), 
        leftMotors(leftFront, leftRear), drivetrain(leftMotors, rightMotors), ultrasonic(us.first, us.second) {
// Do nothing
}

void DriveSubsystem::arcadeDrive(double speed, double rotation) {
    this->drivetrain.ArcadeDrive(speed, rotation);
}

void DriveSubsystem::setMaxSpeed(double max) {
    this->drivetrain.SetMaxOutput(max);
}

double DriveSubsystem::getUltrasonicDistance() {
    return this->ultrasonic.GetRangeInches();
}