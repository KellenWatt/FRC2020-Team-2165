#include "subsystems/BucketSubsystem.h"

#include <frc/DoubleSolenoid.h>

#include <utility>

BucketSubsystem::BucketSubsystem(std::pair<int,int> bucketArmChannels, std::pair<int,int> popperChannels, int ballCheckChannel) :
        bucketArm(bucketArmChannels.first, bucketArmChannels.second),
        popper(popperChannels.first, popperChannels.second), ballCheck(ballCheckChannel) {
    bucketArm.Set(frc::DoubleSolenoid::Value::kReverse);
    popper.Set(frc::DoubleSolenoid::Value::kReverse);
}

void BucketSubsystem::raiseBucket(bool raised) {
    bucketArm.Set(raised ? frc::DoubleSolenoid::Value::kForward : frc::DoubleSolenoid::Value::kReverse);
}

void BucketSubsystem::popArm(bool popped) {
    popper.Set(popped ? frc::DoubleSolenoid::Value::kForward : frc::DoubleSolenoid::Value::kReverse);
}

bool BucketSubsystem::isRaised() {
    return bucketArm.Get() == frc::DoubleSolenoid::Value::kForward;
}

bool BucketSubsystem::isPopped() {
    return popper.Get();
} 

bool BucketSubsystem::hasBall() {
    return ballCheck.Get();
}