/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "RobotContainer.h"

#include <frc2/command/Command.h>
#include <frc2/command/ConditionalCommand.h>
#include <frc2/command/InstantCommand.h>
#include <frc2/command/ParallelRaceGroup.h>
#include <frc2/command/RunCommand.h>
#include <frc2/command/StartEndCommand.h>
#include <frc2/command/SequentialCommandGroup.h>
#include <frc2/command/WaitCommand.h>
#include <frc2/command/WaitUntilCommand.h>
#include <frc/XboxController.h>
#include <frc2/command/button/JoystickButton.h>
#include <frc2/command/button/Trigger.h>


#include <units/units.h>
#include <utility>
#include <cmath>

RobotContainer::RobotContainer() : m_autonomousCommand(&m_subsystem), controller(0), 
    armSubsystem(0,1,2), 
    bucketSubsystem(std::pair<int,int>(0,1), std::pair<int,int>(2,3), 5),
    driveSubsystem(2,5,3,6), 
    liftSubsystem(0, 6), 
    loadSubsystem(7,8, std::pair<int,int>(4,5)) {
  // Initialize all of your commands and subsystems here

  // Configure the button bindings
  this->endgameEnabled = false;

  ConfigureButtonBindings();

  // Drives straight with right stick
  // turns at a constant rate with the bumpers: CW with right, CCW with left.
  // Also, does a little bit of black magic.
  driveSubsystem.SetDefaultCommand(frc2::RunCommand([this] {
    this->driveSubsystem.arcadeDrive(this->controller.GetY(frc::GenericHID::kRightHand),
                                     (this->controller.GetBumperPressed(frc::GenericHID::JoystickHand::kLeftHand) ? -1 : 
                                        (this->controller.GetBumperPressed(frc::GenericHID::JoystickHand::kRightHand) ? 1 : 0)) * 0.8);
  }, {&(this->driveSubsystem)}));

  armSubsystem.SetDefaultCommand(frc2::RunCommand([this] {
    if(this->endgameEnabled) {
      this->armSubsystem.raise(this->controller.GetY(frc::GenericHID::kLeftHand));
    }
  }, {&(this->armSubsystem)}));

}

void RobotContainer::ConfigureButtonBindings() {
  // Configure your button bindings here

  // toggles endgame state
  frc2::JoystickButton(&(this->controller), static_cast<int>(frc::XboxController::Button::kBack))
  .ToggleWhenPressed(frc2::StartEndCommand([this] {this->endgameEnabled = true;},                         
                                           [this] {this->endgameEnabled = false;}));

  // Enables winch on DPAD-Up and toggled state
  frc2::Trigger([this] {return this->controller.GetPOV() == 0 && this->endgameEnabled;})
  .ToggleWhenActive(frc2::RunCommand([this] {this->liftSubsystem.raise(1.0 * (this->liftSubsystem.getHeight() < 1.0));}, 
                               {&(this->liftSubsystem)}));

  // Enables loader on RT press, disables on LT press
  frc2::Trigger([this] {return abs(this->controller.GetTriggerAxis(frc::GenericHID::JoystickHand::kRightHand)) > 0.2;})
  .WhenActive(frc2::RunCommand([this] {this->loadSubsystem.fullEnable(true);}, {&(this->loadSubsystem)})
              .WithInterrupt([this] {return abs(this->controller.GetTriggerAxis(frc::GenericHID::JoystickHand::kLeftHand)) > 0.2;})
              .AndThen([this] {this->loadSubsystem.fullEnable(false);}));

  // Hold B -> Raise/Lower bucket
  frc2::JoystickButton(&(this->controller), static_cast<int>(frc::XboxController::Button::kB))
  .WhenActive([this] {this->bucketSubsystem.raiseBucket(true);}, {&(this->bucketSubsystem)})
  .WhenInactive([this] {this->bucketSubsystem.raiseBucket(false);}, {&(this->bucketSubsystem)});

  // Photo sensor -> activate/deactivate popper automatically
  frc2::Trigger([this] {return this->bucketSubsystem.hasBall();})
  .WhenActive(frc2::SequentialCommandGroup(
                frc2::WaitCommand(1.0_s),
                frc2::InstantCommand([this] {this->bucketSubsystem.popArm(true);}, {&(this->bucketSubsystem)}),
                frc2::WaitCommand(0.2_s)
  ).AndThen([this] {this->bucketSubsystem.popArm(false);}));

  // press A -> 180 spin
  frc2::JoystickButton(&(this->controller), static_cast<int>(frc::XboxController::Button::kA))
  .WhenActive(frc2::SequentialCommandGroup(
                frc2::InstantCommand([this] {this->driveSubsystem.resetGyro();}),
                frc2::RunCommand([this] {this->driveSubsystem.turnToAngle(180);})
  ).WithInterrupt([this] {return this->driveSubsystem.atAngle(180);}), {&(this->driveSubsystem)});

}

frc2::Command* RobotContainer::GetAutonomousCommand() {
  // An example command will be run in autonomous
  return &m_autonomousCommand;
}
