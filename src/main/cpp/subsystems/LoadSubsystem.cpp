#include "subsystems/LoadSubsystem.h"

#include <frc/DoubleSolenoid.h>

#include <utility>

LoadSubsystem::LoadSubsystem(int beltChannel, int captureRollerChannel, std::pair<int,int> captureArmChannels) :
        loaderBelt(beltChannel), captureRoller(captureRollerChannel), 
        captureArm(captureArmChannels.first, captureArmChannels.second) {
    this->beltSpeed = 1.0;
    this->captureRollerSpeed = 1.0;
}

void LoadSubsystem::lowerCaptureArm(bool lowered) {
    this->captureArm.Set(lowered ? frc::DoubleSolenoid::Value::kForward : frc::DoubleSolenoid::Value::kReverse);
}

void LoadSubsystem::enableBelt(bool enabled) {
    this->loaderBelt.Set(enabled * this->beltSpeed);
}

void LoadSubsystem::enableCaptureRoller(bool enabled) {
    this->captureRoller.Set(enabled * this->captureRollerSpeed);
}

void LoadSubsystem::fullEnable(bool enabled) {
    this->lowerCaptureArm(enabled);
    this->enableBelt(enabled);
    this->enableCaptureRoller(enabled);
}

void LoadSubsystem::setBeltSpeed(double speed) {
    this->beltSpeed = speed;
}

void LoadSubsystem::setCaptureRollerSpeed(double speed) {
    this->captureRollerSpeed = speed;
}

bool LoadSubsystem::armLowered(){
    return this->captureArm.Get() == frc::DoubleSolenoid::Value::kForward;
}