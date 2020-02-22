#include "susbystems/BucketSubsystem"

#include <frc/DoubleSolenoid.h>

#include <utility>

BucketSubsystem::BucketSubsystem(std::pair<int,int> bucketArmChannels, std::pair<int,int> popperChannels, int ballCheckChannel) :
        bucketArm(bucketArmChannels.first, bucketArmChannels.second),
        popper(popperChannels.first, popperChannels.second), ballCheck(ballCheckChannels) {
    bucketArm.Set(DoubleSolenoid::Value::kReverse);
    popper.Set(DoubleSolenoid::Value::kReverse);
}

void BucketSubsystem::raiseBucket(bool raised) {
    bucketArm.Set(raised ? DoubleSolenoid::Value::kForward : DoubleSolenoid::Value::kReverse));
}

void BucketSubsystem::popArm(bool popped) {
    popper.Set(popped ? DoubleSolenoid::Value::kForward : DoubleSolenoid::Value::kReverse));
}

bool BucketSubsystem::isRaised() {
    return bucketArm.Get() == DoubleSolenoid::Value::kForward;
}

bool BucketSubsystem::isPopped() {
    return popper.Get()
}

bool BucketSubsystem::hasBall() {
    return ballCheck.Get();
}