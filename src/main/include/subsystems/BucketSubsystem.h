#ifndef _2165_BUCKETSUBSYSTEM_H_
#define _2165_BUCKETSUBSYSTEM_H_

#include <frc2/command/SubsystemBase.h>
#include <frc/DoubleSolenoid.h>
#include <frc/DigitalInput.h>

#include <utility>

class BucketSubsystem : public frc2::SubsystemBase {
private:
    frc::DoubleSolenoid bucketArm;
    frc::DoubleSolenoid popper;
    frc::DigitalInput ballCheck;

public:
    BucketSubsystem(std::pair<int,int> bucketArmChannels, std::pair<int,int> popperChannels, int ballCheckChannel);

    void raiseBucket(bool raised);
    void popArm(bool popped);
    bool isRaised();
    bool isPopped();
    bool hasBall();

};

#endif