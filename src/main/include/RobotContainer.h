/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <cmath>

#include <frc2/command/Command.h>
#include <frc2/command/SelectCommand.h>
#include <frc/XboxController.h>

#include "subsystems/ArmSubsystem.h"
#include "subsystems/BucketSubsystem.h"
#include "subsystems/DriveSubsystem.h"
#include "subsystems/LiftSubsystem.h"
#include "subsystems/LoadSubsystem.h"

/**
 * This class is where the bulk of the robot should be declared.  Since
 * Command-based is a "declarative" paradigm, very little robot logic should
 * actually be handled in the {@link Robot} periodic methods (other than the
 * scheduler calls).  Instead, the structure of the robot (including subsystems,
 * commands, and button mappings) should be declared here.
 */
class RobotContainer {
private:
  // The robot's subsystems and commands are defined here...
  frc2::SelectCommand<int> m_autonomousCommand{
    [this] {return 0;},
    // Do nothing
    std::pair(0, frc2::InstantCommand()),
    // Drive off line
    std::pair(1, frc2::SequentialCommandGroup(frc2::InstantCommand([this] {this->driveSubsystem.resetEncoder();}),
                                              frc2::RunCommand([this] {this->driveSubsystem.adjustedArcadeDrive(0.5, 0);})
                                              .WithInterrupt([this] {return this->driveSubsystem.getDistance() >= 60;}))),
    // Drive backward to goal and dump, then drive back
    std::pair(2, frc2::SequentialCommandGroup()),
    // Drive forward and pick up balls
    std::pair(4, frc2::SequentialCommandGroup())
  };


  frc::XboxController controller;

  ArmSubsystem armSubsystem;
  BucketSubsystem bucketSubsystem;
  DriveSubsystem driveSubsystem;
  LiftSubsystem liftSubsystem;
  LoadSubsystem loadSubsystem;

  void ConfigureButtonBindings();

public:
  RobotContainer();

  frc2::Command* GetAutonomousCommand();
};
