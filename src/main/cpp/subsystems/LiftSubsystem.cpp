#include "subsystems/LiftSubsystem.h"

#include <utility>

LiftSubsystem::LiftSubsystem(int winchChannel, std::pair<int,int> sonarChannels) :
        winch(winchChannel), heightSonar(sonarChannels.first, sonarChannels.second) {
    heightSonar.enable(true);
}

void LiftSubsystem::raise(double speed) {
    winch.Set(speed);
}

void LiftSubsystem::lower(double speed) {
    winch.Set(-speed);
}

void LiftSubsystem::stop() {
    winch.Set(0);
}

double LiftSubsystem::getHeight() {
    heightSonar.getRange();
}