#include "subsystems/ArmSubsystem.h"

ArmSubsystem::ArmSubsystem() : ArmSubsystem::ArmSubsystem(0,0,0) {}

ArmSubsystem::ArmSubsystem(int motor, int up, int down) : armMotor(motor), armsUp(up), armsDown(down) {}

/*
ArmSubsystem::ArmSubsystem(int motor, int up, int down, int hookLeft, int hookRight) :
        armMotor(motor), armsUp(up), armsDown(down), hookReleaseLeft(hookLeft), hookReleaseRight(hookRight) {
// set servos to closed
}*/

void ArmSubsystem::raise(double speed) {
    this->armMotor.Set(speed * !this->fullRaised());
}

void ArmSubsystem::lower(double speed) {
    this->armMotor.Set(-speed * !this->fullLowered());
}

void ArmSubsystem::fullStop() {
    this->armMotor.Set(0);
}
/* // originally required when the release hook was powered 
void ArmSubsystem::releaseHook(bool release) {
    double range = 0.25;
    double base = 0.0;

    this->hookReleaseLeft.Set(base + (range*release));
    this->hookReleaseRight.Set((1.0 - base) - (range*release));
}
*/
bool ArmSubsystem::fullRaised() {
    return this->armsUp.Get();
}

bool ArmSubsystem::fullLowered() {
    return this->armsDown.Get();
}

bool ArmSubsystem::middling() {
    return !(this->fullRaised() || this->fullLowered());
}

