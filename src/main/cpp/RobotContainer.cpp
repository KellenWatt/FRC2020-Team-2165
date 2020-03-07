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
#include <frc2/command/SelectCommand.h>
#include <frc2/command/WaitCommand.h>
#include <frc2/command/WaitUntilCommand.h>
#include <frc2/command/FunctionalCommand.h>
#include <frc/XboxController.h>
#include <frc2/command/button/JoystickButton.h>
#include <frc2/command/button/Trigger.h>

#include <frc/smartDashboard/SmartDashboard.h>


#include <units/units.h>
#include <utility>
#include <cmath>

RobotContainer::RobotContainer() : controller(0), 
    armSubsystem(4), 
    bucketSubsystem(std::pair<int,int>(2,3), std::pair<int,int>(4,5), 2),
    driveSubsystem(3,6,2,5, std::pair<int,int>(0,1)), 
    liftSubsystem(8,0), 
    loadSubsystem(1,7, std::pair<int,int>(6,7)) {
  // Initialize all of your commands and subsystems here

  // Configure the button bindings
  ConfigureButtonBindings();

  driveSubsystem.SetDefaultCommand(frc2::RunCommand([this] {
    this->driveSubsystem.enableBrakes(false);
    this->driveSubsystem.adjustedArcadeDrive(-this->controller.GetY(frc::GenericHID::JoystickHand::kLeftHand),
                                             this->controller.GetX(frc::GenericHID::JoystickHand::kRightHand));
  }, {&(this->driveSubsystem)}));

}

RobotContainer::~RobotContainer() {
  delete this->m_autonomousCommand;
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

  // Start + Back -> Run winch program, lift automatically
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
  .WhenActive(frc2::SequentialCommandGroup( // Possibly switch to a WhenInactive, just to see if we can get rid of that first delay
                frc2::WaitCommand(0.5_s),
                frc2::InstantCommand([this] {this->bucketSubsystem.popArm(true);}, {&(this->bucketSubsystem)}),
                frc2::WaitCommand(0.5_s),
                frc2::InstantCommand([this] {this->bucketSubsystem.popArm(false);}, {&(this->bucketSubsystem)})));

  // Right stick -> 180 spin
  frc2::JoystickButton(&(this->controller), static_cast<int>(frc::XboxController::Button::kStickRight))
  .ToggleWhenActive(frc2::SequentialCommandGroup(
                frc2::InstantCommand([this] {this->driveSubsystem.resetGyro();
                                             this->driveSubsystem.enableBrakes(true);}, {&(this->driveSubsystem)}),
                frc2::RunCommand([this] {this->driveSubsystem.turnToAngleAtSpeed(180, 0.6);}, {&(this->driveSubsystem)})
                .WithInterrupt([this] {return this->driveSubsystem.atAngle(180);})));


  // Granular Controls for loader
  // Requirement restrictions removed, so be careful
  // Run belt on D-pad up or up-right
  frc2::Trigger([this] {return this->controller.GetPOV() == 0 || this->controller.GetPOV() == 45;})
  .WhenActive([this] {this->loadSubsystem.enableBelt(true);})
  .WhenInactive([this] {this->loadSubsystem.enableBelt(false);});
  // Run roller on D-pad right or up-right
  frc2::Trigger([this] {return this->controller.GetPOV() == 90 || this->controller.GetPOV() == 45;})
  .WhenActive([this] {this->loadSubsystem.enableCaptureRoller(true);})
  .WhenInactive([this] {this->loadSubsystem.enableCaptureRoller(false);});
  // toggle capture arm on D-pad down
  frc2::Trigger([this] {return this->controller.GetPOV() == 180;})
  .ToggleWhenActive(frc2::StartEndCommand([this] {this->loadSubsystem.lowerCaptureArm(true);},
                                          [this] {this->loadSubsystem.lowerCaptureArm(false);}));

}

frc2::Command* RobotContainer::GetAutonomousCommand() {
  // An example command will be run in autonomous
  

  //button name: DB/Button 0
  //                       1/2/3
  //
  // commands 0: move forward 100 inches at half speed with timout of 10s 
  //          1: move forward at half speed for 3 seconds
  //          2: get balls
  //          3: dump balls
  //    default: do nothing

  return new frc2::SelectCommand<int>(
    [this] {return 0;},
    // Do nothing
    std::pair<int, frc2::SequentialCommandGroup>(0, frc2::SequentialCommandGroup(frc2::InstantCommand())),
    // Drive off line
    std::pair<int, frc2::SequentialCommandGroup>(1, frc2::SequentialCommandGroup(frc2::InstantCommand([this] {this->driveSubsystem.resetEncoder();}),
                                              frc2::RunCommand([this] {this->driveSubsystem.adjustedArcadeDrive(0.5, 0);})
                                              .WithInterrupt([this] {return this->driveSubsystem.getDistance() >= 100;})
                                              .WithTimeout(10.0_s))),
    // Drive forward and pick up balls
    std::pair<int, frc2::SequentialCommandGroup>(2, frc2::SequentialCommandGroup(frc2::InstantCommand([this] {this->loadSubsystem.fullEnable(true);}),
                                              frc2::WaitCommand(0.5_s),
                                              frc2::RunCommand([this] {this->driveSubsystem.adjustedArcadeDrive(0.5, 0);})
                                              .WithTimeout(14.0_s)
                                              .WithInterrupt([this] {return this->driveSubsystem.getDistance() >= 180;}))
                                              .AndThen([this] {this->loadSubsystem.fullEnable(false);})),
    // Drive backward to goal and dump, then drive back
    std::pair<int, frc2::SequentialCommandGroup>(3, frc2::SequentialCommandGroup(frc2::InstantCommand([this] {this->driveSubsystem.resetEncoder();}),
                                              frc2::RunCommand([this] {this->driveSubsystem.adjustedArcadeDrive(-0.5, 0);})
                                              .WithInterrupt([this] {return this->driveSubsystem.getDistance() <= -84;}),
                                              frc2::WaitCommand(1.0_s),
                                              frc2::InstantCommand([this] {this->bucketSubsystem.raiseBucket(true);}),
                                              frc2::WaitCommand(2.0_s),
                                              frc2::InstantCommand([this] {this->bucketSubsystem.raiseBucket(false);}),
                                              frc2::RunCommand([this] {this->driveSubsystem.adjustedArcadeDrive(0.5, 0);})
                                              .WithInterrupt([this] {return this->driveSubsystem.getDistance() >= 36;})))
  );
}
