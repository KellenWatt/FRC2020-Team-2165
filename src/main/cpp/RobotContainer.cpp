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
#include <frc2/command/SequentialCommandGroup.h>
#include <frc2/command/WaitCommand.h>
#include <frc2/command/WaitUntilCommand.h>
#include <frc/XboxController.h>

#include <utility>

RobotContainer::RobotContainer() : m_autonomousCommand(&m_subsystem), controller(0), 
    armSubsystem(0,1,2), 
    bucketSubsystem(std::pair<int,int>(0,1), std::pair<int,int>(2,3), 5),
    driveSubsystem(2,5,3,6, std::pair<int,int>(4,8)), 
    liftSubsystem(0, 6), 
    loadSubsystem(7,8, std::pair<int,int>(4,5)) {
  // Initialize all of your commands and subsystems here

  // Configure the button bindings
  ConfigureButtonBindings();

  driveSubsystem.SetDefaultCommand(frc2::RunCommand([this] {
    this->driveSubsystem.arcadeDrive(this->controller.GetY(frc::GenericHID::kLeftHand),
                                 this->controller.GetX(frc::GenericHID::kRightHand));
  }, {&driveSubsystem}));
}

void RobotContainer::ConfigureButtonBindings() {
  // Configure your button bindings here


  
}

frc2::Command* RobotContainer::GetAutonomousCommand() {
  // An example command will be run in autonomous
  return &m_autonomousCommand;
}
