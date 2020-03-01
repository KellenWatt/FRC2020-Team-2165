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
  ConfigureButtonBindings();

  driveSubsystem.SetDefaultCommand(frc2::RunCommand([this] {
    this->driveSubsystem.arcadeDrive(this->controller.GetY(frc::GenericHID::kLeftHand),
                                 this->controller.GetX(frc::GenericHID::kRightHand));
  }, {&(this->driveSubsystem)}));

}

void RobotContainer::ConfigureButtonBindings() {
  // Configure your button bindings here

  // LB + Y -> raise arms manually
  (frc2::JoystickButton(&(this->controller), static_cast<int>(frc::XboxController::Button::kBumperLeft)) &&
   frc2::JoystickButton(&(this->controller), static_cast<int>(frc::XboxController::Button::kY)))
  .WhileActiveContinous([this] {this->armSubsystem.raise(1.0);},
                        {&(this->armSubsystem)});
  
  // LB + X -> lower arms manually
  (frc2::JoystickButton(&(this->controller), static_cast<int>(frc::XboxController::Button::kBumperLeft)) &&
   frc2::JoystickButton(&(this->controller), static_cast<int>(frc::XboxController::Button::kX)))
  .WhileActiveContinous([this] {this->armSubsystem.lower(1.0);},
                        {&(this->armSubsystem)});
  
  // LB + Right-Y -> move arms manually
  (frc2::JoystickButton(&(this->controller), static_cast<int>(frc::XboxController::Button::kBumperLeft)) && 
   frc2::Trigger([this] {return abs(this->controller.GetY(frc::GenericHID::JoystickHand::kRightHand) > 0.1);}))
  .WhileActiveContinous([this] {this->armSubsystem.raise(this->controller.GetY(frc::GenericHID::JoystickHand::kRightHand));},
                        {&(this->armSubsystem)});
  
  // RB + Y -> raise winch manually
  (frc2::JoystickButton(&(this->controller), static_cast<int>(frc::XboxController::Button::kBumperRight)) &&
   frc2::JoystickButton(&(this->controller), static_cast<int>(frc::XboxController::Button::kY)))
  .WhileActiveContinous([this] {this->liftSubsystem.raise(1.0);},
                        {&(this->liftSubsystem)});

  // RB + X -> lower winch manually
  (frc2::JoystickButton(&(this->controller), static_cast<int>(frc::XboxController::Button::kBumperRight)) &&
   frc2::JoystickButton(&(this->controller), static_cast<int>(frc::XboxController::Button::kX)))
  .WhileActiveContinous([this] {this->liftSubsystem.lower(1.0);},
                        {&(this->liftSubsystem)});

  // Start + Select -> Run winch program, lift automatically
  (frc2::JoystickButton(&(this->controller), static_cast<int>(frc::XboxController::Button::kBack)) &&
   frc2::JoystickButton(&(this->controller), static_cast<int>(frc::XboxController::Button::kStart)))
  .ToggleWhenActive(frc2::RunCommand([this] {this->liftSubsystem.raise(1.0 * (this->liftSubsystem.getHeight() < 1.0));}, 
                               {&(this->liftSubsystem)}));

  // Toggle A -> full enable/disable loader
  frc2::JoystickButton(&(this->controller), static_cast<int>(frc::XboxController::Button::kA))
  .ToggleWhenPressed(frc2::RunCommand([this] {this->loadSubsystem.fullEnable(true);
                                      }, {&(this->loadSubsystem)})
                     .AndThen([this] {this->loadSubsystem.fullEnable(false);}));

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
                frc2::WaitCommand(1.0_s),
                frc2::InstantCommand([this] {this->bucketSubsystem.popArm(true);}, {&(this->bucketSubsystem)})
  ).AndThen([this] {this->bucketSubsystem.popArm(false);}));

  // Right stick -> 180 spin
  frc2::JoystickButton(&(this->controller), static_cast<int>(frc::XboxController::Button::kStickRight))
  .WhenActive(frc2::SequentialCommandGroup(
                frc2::InstantCommand([this] {this->driveSubsystem.resetGyro();}),
                frc2::RunCommand([this] {this->driveSubsystem.turnToAngle(180);})
  ).WithInterrupt([this] {return abs(180 - this->driveSubsystem.getAngle()) < 0.5;}), {&(this->driveSubsystem)});

}

frc2::Command* RobotContainer::GetAutonomousCommand() {
  // An example command will be run in autonomous
  return &m_autonomousCommand;
}
