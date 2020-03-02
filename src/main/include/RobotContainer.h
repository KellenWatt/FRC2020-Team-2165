/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <cmath>

#include <frc2/command/Command.h>
#include <frc/XboxController.h>

#include "commands/ExampleCommand.h"
#include "subsystems/ExampleSubsystem.h"

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
 public:
  RobotContainer();

  frc2::Command* GetAutonomousCommand();

 private:


  // The robot's subsystems and commands are defined here...
  ExampleSubsystem m_subsystem;
  ExampleCommand m_autonomousCommand;

  frc::XboxController controller;

  ArmSubsystem armSubsystem;
  BucketSubsystem bucketSubsystem;
  DriveSubsystem driveSubsystem;
  LiftSubsystem liftSubsystem;
  LoadSubsystem loadSubsystem;

  bool endgameEnabled;

  void ConfigureButtonBindings();
};
