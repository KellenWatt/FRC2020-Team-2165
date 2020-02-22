/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "components/UltraSonic.h"


Ultrasonic::Ultrasonic() : Ultrasonic::Ultrasonic(0,0, Ultrasonic::UnitType::INCH) {}

Ultrasonic::Ultrasonic(int counterPort, int echoPort, Ultrasonic::UnitType unit) : counter(counterPort), echo(echoPort), measureUnit(unit) {
    counter.SetMaxPeriod(0.2);       //max period for determining pulse duration(seconds)
    counter.SetSemiPeriodMode(true); //pulse width of a high pulse (defined by rising edge to falling edge)
    counter.Reset();

    echo.Set(false);
}

bool Ultrasonic::isValidRange() {
    return counter.Get() > 1; // counter has started
}

double Ultrasonic::getRange() {
    if(this->isValidRange()) { // if count is invalid, then range has not been measured yet.
        return counter.GetPeriod() * (this->measureUnit == Ultrasonic::UnitType::INCH ? speedOfSoundInchesPerSec : speedOfSoundMMPerSec) / 2.0;
    } else {
        return 0;    // returns 0 to be compatibel with WPIlib definitions.
    }
}

double Ultrasonic::ping() {
    this->enable(true);
    this->resetCounter();
    double range = this->getRange();
    this->enable(false);
    return range;
}

void Ultrasonic::resetCounter() {
    counter.Reset();
}

void Ultrasonic::enable(bool enabled) {
    echo.Set(enabled);
}

bool Ultrasonic::isEnabled() {
    return echo.Get();
}

void Ultrasonic::setUnit(Ultrasonic::UnitType unit) {
    this->measureUnit = unit;
}