#pragma once

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
  static constexpr double speedOfSoundInchesPerSec  = 13660.0;
  static constexpr double speedOfSoundMMPerSec = speedOfSoundInchesPerSec * 25.4;

  frc::Counter counter;
  frc::DigitalOutput echo;

  UnitType measureUnit;

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
