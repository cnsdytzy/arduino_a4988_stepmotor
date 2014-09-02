#include <AccelStepper.h>

AccelStepper stepper(1, 3, 6); // pin 3 = step, pin 6 = direction

void setup() {
  stepper.setMaxSpeed(6000);
  stepper.setAcceleration(3000);
  //stepper.setSpeed(2000);
}

void loop() {  
  //stepper.runSpeed();
  int destpos = 50000;
   
  stepper.moveTo(destpos);
  while (stepper.currentPosition() != destpos) // Full speed up to 1200
    stepper.run();
  stepper.stop(); // Stop as fast as possible: sets new target
  stepper.runToPosition(); 
  // Now stopped after quickstop
  // Now go backwards
  stepper.moveTo(-destpos);
  while (stepper.currentPosition() != 0) // Full speed basck to 0
    stepper.run();
  stepper.stop(); // Stop as fast as possible: sets new target
  stepper.runToPosition(); 
  // Now stopped after quickstop   
}


