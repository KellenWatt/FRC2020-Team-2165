#include "subsystems/BucketSubsystem.h"

#include <frc/DoubleSolenoid.h>

#include <utility>

BucketSubsystem::BucketSubsystem(std::pair<int,int> bucketArmChannels, std::pair<int,int> popperChannels, int ballCheckChannel) :
        bucketArm(bucketArmChannels.first, bucketArmChannels.second),
        popper(popperChannels.first, popperChannels.second), ballCheck(ballCheckChannel) {
}

void BucketSubsystem::raiseBucket(bool raised) {
    this->bucketArm.Set(raised ? frc::DoubleSolenoid::Value::kForward : frc::DoubleSolenoid::Value::kReverse);
}

void BucketSubsystem::popArm(bool popped) {
    this->popper.Set(popped ? frc::DoubleSolenoid::Value::kForward : frc::DoubleSolenoid::Value::kReverse);
}

bool BucketSubsystem::isRaised() {
    return this->bucketArm.Get() == frc::DoubleSolenoid::Value::kForward;
}

bool BucketSubsystem::isPopped() {
    return this->popper.Get();
} 

bool BucketSubsystem::hasBall() {
    return this->ballCheck.Get();
}