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
#include <frc2/command/FunctionalCommand.h>
#include <frc/XboxController.h>
#include <frc2/command/button/JoystickButton.h>
#include <frc2/command/button/Trigger.h>


#include <units/units.h>
#include <utility>
#include <cmath>

RobotContainer::RobotContainer() : m_autonomousCommand(&m_subsystem), controller(0), 
    armSubsystem(4), 
    bucketSubsystem(std::pair<int,int>(2,3), std::pair<int,int>(4,5), 2),
    driveSubsystem(3,6,2,5, std::pair<int,int>(0,0)), 
    liftSubsystem(8,0), 
    loadSubsystem(1,7, std::pair<int,int>(6,7)) {
  // Initialize all of your commands and subsystems here

  // Configure the button bindings
  ConfigureButtonBindings();

  driveSubsystem.SetDefaultCommand(frc2::RunCommand([this] {
    this->driveSubsystem.adjustedArcadeDrive(-this->controller.GetY(frc::GenericHID::JoystickHand::kLeftHand),
                                             this->controller.GetX(frc::GenericHID::JoystickHand::kRightHand));
  }, {&(this->driveSubsystem)}));

}

void RobotContainer::ConfigureButtonBindings() {
  // Configure your button bindings here

  // LB + Y -> raise arms manually
  (frc2::JoystickButton(&(this->controller), static_cast<int>(frc::XboxController::Button::kBumperLeft)) &&
   frc2::JoystickButton(&(this->controller), static_cast<int>(frc::XboxController::Button::kY)))
  .WhileActiveContinous([this] {this->armSubsystem.raise(1.0);}, {&(this->armSubsystem)})
  .WhenInactive([this] {this->armSubsystem.fullStop();}, {&(this->armSubsystem)});

  
  // LB + X -> lower arms manually
  (frc2::JoystickButton(&(this->controller), static_cast<int>(frc::XboxController::Button::kBumperLeft)) &&
   frc2::JoystickButton(&(this->controller), static_cast<int>(frc::XboxController::Button::kX)))
  .WhileActiveContinous([this] {this->armSubsystem.lower(1.0);}, {&(this->armSubsystem)})
  .WhenInactive([this] {this->armSubsystem.fullStop();}, {&(this->armSubsystem)});
  
  // LB + Right-Y -> move arms manually, variable speed
  (frc2::JoystickButton(&(this->controller), static_cast<int>(frc::XboxController::Button::kBumperLeft)) && 
   frc2::Trigger([this] {return abs(this->controller.GetY(frc::GenericHID::JoystickHand::kRightHand)) > 0.1;}))
  .WhileActiveContinous([this] {this->armSubsystem.raise(-this->controller.GetY(frc::GenericHID::JoystickHand::kRightHand));},
                        {&(this->armSubsystem)})
  .WhenInactive([this] {this->armSubsystem.fullStop();}, {&(this->armSubsystem)});
  
  // RB + Y -> raise winch manually
  (frc2::JoystickButton(&(this->controller), static_cast<int>(frc::XboxController::Button::kBumperRight)) &&
   frc2::JoystickButton(&(this->controller), static_cast<int>(frc::XboxController::Button::kY)))
  .WhileActiveContinous([this] {this->liftSubsystem.raise(0.5);}, {&(this->liftSubsystem)})
  .WhenInactive([this] {this->liftSubsystem.stop();}, {&(this->liftSubsystem)});
  
  // RB + Right-Y -> move arms manually, variable speed (only upward)
  (frc2::JoystickButton(&(this->controller), static_cast<int>(frc::XboxController::Button::kBumperRight)) &&
   frc2::Trigger([this] {return abs(this->controller.GetY(frc::GenericHID::JoystickHand::kRightHand)) > 0.1;}))
  .WhileActiveContinous([this] {this->liftSubsystem.raise(abs(this->controller.GetY(frc::GenericHID::JoystickHand::kRightHand)));},
                        {&(this->liftSubsystem)})
  .WhenInactive([this] {this->liftSubsystem.stop();}, {&(this->liftSubsystem)});

  // RB + X -> lower winch manually - DO NOT USE
#ifdef LOWER_WINCH
  (frc2::JoystickButton(&(this->controller), static_cast<int>(frc::XboxController::Button::kBumperRight)) &&
   frc2::JoystickButton(&(this->controller), static_cast<int>(frc::XboxController::Button::kX)))
  .WhileActiveContinous([this] {this->liftSubsystem.lower(0.5);},
                        {&(this->liftSubsystem)});
#endif
  // Start + Select -> Run winch program, lift automatically
  (frc2::JoystickButton(&(this->controller), static_cast<int>(frc::XboxController::Button::kBack)) &&
   frc2::JoystickButton(&(this->controller), static_cast<int>(frc::XboxController::Button::kStart)))
  .ToggleWhenActive(frc2::FunctionalCommand([this] {},
                                            [this] {this->liftSubsystem.raise(0.5 * (this->liftSubsystem.getHeight() < 1.0));},
                                            [this] (bool) {this->liftSubsystem.stop();},
                                            [this] {return false;}, 
                                            {&(this->liftSubsystem)}));

  // Toggle A -> enable/disable loader
  frc2::JoystickButton(&(this->controller), static_cast<int>(frc::XboxController::Button::kA))
  .ToggleWhenActive(frc2::StartEndCommand([this] {this->loadSubsystem.fullEnable(true);},
                                          [this] {this->loadSubsystem.fullEnable(false);},
                                          {&(this->loadSubsystem)}));
    
  // Hold B -> Raise/Lower bucket
  frc2::JoystickButton(&(this->controller), static_cast<int>(frc::XboxController::Button::kB))
  .WhenActive([this] {this->bucketSubsystem.raiseBucket(true);}, {&(this->bucketSubsystem)})
  .WhenInactive([this] {this->bucketSubsystem.raiseBucket(false);}, {&(this->bucketSubsystem)});

  // Left stick -> activate/deactivate popper manually
  frc2::JoystickButton(&(this->controller), static_cast<int>(frc::XboxController::Button::kStickLeft))
  .WhenActive([this] {this->bucketSubsystem.popArm(true);}, {&(this->bucketSubsystem)})
  .WhenInactive([this] {this->bucketSubsystem.popArm(false);}, {&(this->bucketSubsystem)});

  // Photo sensor -> activate/deactivate popper automatically
  frc2::Trigger([this] {return this->bucketSubsystem.hasBall();})
  .WhenActive(frc2::SequentialCommandGroup(
                frc2::WaitCommand(0.8_s),
                frc2::InstantCommand([this] {this->bucketSubsystem.popArm(true);}, {&(this->bucketSubsystem)}),
                frc2::WaitCommand(0.5_s),
                frc2::InstantCommand([this] {this->bucketSubsystem.popArm(false);}, {&(this->bucketSubsystem)})));

  // Right stick -> 180 spin
  frc2::JoystickButton(&(this->controller), static_cast<int>(frc::XboxController::Button::kStickRight))
  .WhenActive(frc2::SequentialCommandGroup(
                frc2::InstantCommand([this] {this->driveSubsystem.resetGyro();}, {&(this->driveSubsystem)}),
                frc2::RunCommand([this] {this->driveSubsystem.turnToAngleAtSpeed(180, 0.6);}, {&(this->driveSubsystem)}))
  .WithInterrupt([this] {return this->driveSubsystem.atAngle(180);}), {&(this->driveSubsystem)});

}

frc2::Command* RobotContainer::GetAutonomousCommand() {
  // An example command will be run in autonomous
  return &m_autonomousCommand;
}
