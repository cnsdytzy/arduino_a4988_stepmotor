// https://bitbucket.org/ehsmaes/cmdcallback/wiki/Home
#include <CallBack.h>
// https://www.pjrc.com/teensy/td_libs_AccelStepper.html
#include <AccelStepper.h>

// Compile and upload to arduino. Run the serial monitor and type command
// :help;
// Values for initiation of cmd/response interface. 
// After initial boot, id, gid and del are stored in eeprom.
// Change values by command. Make sure each device has a unique id.
String descr="Command/response test program v0.1";
String id="a1";
String gid="a";
int    del=0; //delayed response
byte   echo=1; // command back to host

// List of commands defined by keyword, funtion pointer, number of arguments 
// and description used in "help" command.
CallBackDef f[] = {
  {(String)"add",   (FunctionPointer)&add,  (int)2, (String)":num1:num2"}
};

// initiate command handler: function array, number of functions and intial values
CallBack cmd(f, sizeof(f) / sizeof(*f), id, gid, descr, del, echo);

//   --------- command initiated callback functions below ---------
// callback functions all need to be defined void and with String argv
// argument list. The command parser will validate the number of input
// parameters but any additional validation has to be perfomed by each
// callback function. As the argument list is passed as strings, type
// casting to other types is the responsibility of the function.

void add(String argv[]) {
  int a = cmd.stoi(argv[0]);
  int b = cmd.stoi(argv[1]);
  cmd.respond(String(a + b));
}


AccelStepper stepper(1, 3, 6); // pin 3 = step, pin 6 = direction

void setup() {
  stepper.setMaxSpeed(6000);
  stepper.setAcceleration(3000);
  //stepper.setSpeed(2000);
  
  Serial.begin(9600);
  cmd.ok(); // say hello
}

void loop() {  
/*
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
*/

  cmd.cmdCheck();
}
