#include "subsystems/DriveSubsystem.h"

#include <frc/SPI.h>
#include <AHRS.h>

#include <utility>
#include <algorithm>
#include <cmath>


DriveSubsystem::DriveSubsystem(int rf, int rr, int lf, int lr, std::pair<int,int> encoderChannels) :
        rightFront(rf), rightRear(rr), leftFront(lf), leftRear(lr), rightMotors(rightFront, rightRear), 
        leftMotors(leftFront, leftRear), drivetrain(leftMotors, rightMotors), gyro(frc::SPI::Port::kMXP), 
        encoder(encoderChannels.first, encoderChannels.second) {
// set encoder constants

    this->encoder.SetDistancePerPulse(0.01252); // This is correct. Don't worry about it.
    this->encoder.SetReverseDirection(true);

}

void DriveSubsystem::arcadeDrive(double speed, double rotation) {
    this->drivetrain.ArcadeDrive(speed, rotation);
}

void DriveSubsystem::adjustedArcadeDrive(double speed, double rotation) {
    double rate = -this->gyro.GetRate(); 

    this->drivetrain.ArcadeDrive(speed, (abs(rotation) < 0.02 ? rate : rotation)); 
    // magic number comes from DifferentialDrive deadband.
    // Basically, if rotation is supposed to be 0, stabilize forward motion
    // if rate is less than 0.02 degrees per second, we'll be fine, otherwise adjust the rate by some factor
}

void DriveSubsystem::turnToAngle(double angle) {
   this->turnToAngleAtSpeed(angle, 0.5); 
}

void DriveSubsystem::turnToAngleAtSpeed(double angle, double speed) {
    if(!this->atAngle(angle)) {
        this->adjustedArcadeDrive(0, (angle < 0 ? -1 : 1) * speed);
    }
}

double DriveSubsystem::getAngle() {
    return this->gyro.GetAngle();
}

bool DriveSubsystem::atAngle(double angle) {
    return abs(angle - this->gyro.GetAngle()) < 0.5;
}

double DriveSubsystem::getDistance() {
    return this->encoder.GetDistance();
}

void DriveSubsystem::enableBrakes(bool enable) {
    NeutralMode mode = (enable ? NeutralMode::Brake : NeutralMode::Coast);
    
    this->rightFront.SetNeutralMode(mode);
    this->rightRear.SetNeutralMode(mode);
    this->leftFront.SetNeutralMode(mode);
    this->leftRear.SetNeutralMode(mode);
}


void DriveSubsystem::setMaxSpeed(double max) {
    this->drivetrain.SetMaxOutput(max);
}

void DriveSubsystem::resetGyro() {
    this->gyro.Reset();
}

void DriveSubsystem::resetEncoder() {
    this->encoder.Reset();
}
