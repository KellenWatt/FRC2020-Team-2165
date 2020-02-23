#include "subsystems/LiftSubsystem.h"

#include <utility>

LiftSubsystem::LiftSubsystem(int winchChannel, std::pair<int,int> sonarChannels) :
        winch(winchChannel), heightSonar(sonarChannels.first, sonarChannels.second) {
    this->heightSonar.enable(true);
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
    return this->heightSonar.getRange();
}