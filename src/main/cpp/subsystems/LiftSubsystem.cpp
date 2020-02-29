#include "subsystems/LiftSubsystem.h"

#include <utility>

LiftSubsystem::LiftSubsystem(int winchChannel, int sonarChannel) :
        winch(winchChannel), heightIR(sonarChannel) {
    //this->heightIR.enable(true);
    // configures the pins on the Talons
    this->winch.ConfigForwardLimitSwitchSource(LimitSwitchSource::LimitSwitchSource_FeedbackConnector, LimitSwitchNormal::LimitSwitchNormal_NormallyOpen, 0);
    this->winch.ConfigReverseLimitSwitchSource(LimitSwitchSource::LimitSwitchSource_FeedbackConnector, LimitSwitchNormal::LimitSwitchNormal_NormallyOpen, 0);
}

void LiftSubsystem::raise(double speed) {
    this->winch.Set(speed);
}

void LiftSubsystem::lower(double speed) {
    this->winch.Set(-speed);
}

void LiftSubsystem::stop() {
    this->winch.Set(0);
}

double LiftSubsystem::getHeight() {
    return this->heightIR.GetValue() / 1000; // No idea why this is, but it is.
}