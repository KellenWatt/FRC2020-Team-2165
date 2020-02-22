/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once
//#include <frc/WPILib.h>
#include <math.h>
#include <frc/Counter.h>
#include <frc/DigitalInput.h>
#include <frc/DigitalOutput.h>


class Ultrasonic {
public: 
  enum class UnitType {
    INCH,
    MM
  };
private:
  // It's desirable that everything possible under private except
  // for methods that implement subsystem capabilities

  static constexpr double speedOfSoundInchesPerSec  = 13660.0;
  static constexpr double speedOfSoundMMPerSec = speedOfSoundInchesPerSec * 25.4;

  UnitType measureUnit;

  frc::Counter counter;
  frc::DigitalOutput echo;

  bool isValidRange();

public:
  Ultrasonic();
  Ultrasonic(int counterPort, int echoPort, UnitType unit = UnitType::INCH);

  double getRange();
  double ping();
  void resetCounter();
  void enable(bool enabled);
  bool isEnabled();

  void setUnit(UnitType unit);
};
