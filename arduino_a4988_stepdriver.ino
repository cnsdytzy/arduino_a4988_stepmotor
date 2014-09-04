// https://bitbucket.org/ehsmaes/cmdcallback/wiki/Home
#include <CallBack.h>
// https://www.pjrc.com/teensy/td_libs_AccelStepper.html
#include <AccelStepper.h>

AccelStepper stepper(1, 3, 6); // pin 3 = step, pin 6 = direction
int direction = 1; // 1 for ClockWise, -1 for CounterClockWise

// Compile and upload to arduino. Run the serial monitor and type command
// :help;
// Values for initiation of cmd/response interface. 
// After initial boot, id, gid and del are stored in eeprom.
// Change values by command. Make sure each device has a unique id.
String descr="Command/response test program v0.1";
String id="a1";
String gid="a";
int    del=0; //delayed response
byte   echo=0; // command back to host

// List of commands defined by keyword, funtion pointer, number of arguments 
// and description used in "help" command.
CallBackDef f[] = {
  {(String)"setdir", (FunctionPointer)&setdir, (int)1, (String)":<1|-1>"},
  {(String)"setspeedacl", (FunctionPointer)&setspeedacl, (int)2, (String)":<speed>:<acceleration>"},
  {(String)"moveto", (FunctionPointer)&moveto, (float)1, (String)":<position>"},
  {(String)"movetofeedback", (FunctionPointer)&movetofeedback, (float)2, (String)":<position>:<feedback>"}
};

// initiate command handler: function array, number of functions and intial values
CallBack cmd(f, sizeof(f) / sizeof(*f), id, gid, descr, del, echo);

//   --------- command initiated callback functions below ---------
// callback functions all need to be defined void and with String argv
// argument list. The command parser will validate the number of input
// parameters but any additional validation has to be perfomed by each
// callback function. As the argument list is passed as strings, type
// casting to other types is the responsibility of the function.

// id=a1:setdir:1;
// id=a1:setdir:-1;
void setdir(String argv[]) {

  int idir = cmd.stoi(argv[0]); // 1 for ClockWise, -1 for CounterClockWise
  if(idir == 1 || idir == -1)
  {
    direction = idir;
    cmd.respond("direction="+String(idir)+":ok");
  }
  else
  {
    cmd.respond("direction=NAN");
  }
}

// id=a1:setspeedacl:6000:3000;
void setspeedacl(String argv[]) {

  int spd = cmd.stoi(argv[0]);
  int accl = cmd.stoi(argv[1]);
  
  stepper.setMaxSpeed(spd);
  stepper.setAcceleration(accl);
  
  cmd.respond("speed="+String(spd)+":acceleration="+String(accl)+":ok");
}

// id=a1:moveto:15000;
// id=a1:moveto:500000;
void moveto(String argv[]) {

  long int destpos = (long int)cmd.stof(argv[0]);  // use float for higher number
  destpos = direction * destpos; // set direction
  stepper.moveTo(destpos);
  while (stepper.currentPosition() != destpos)
    stepper.run();
  stepper.stop(); // Stop as fast as possible: sets new target
  stepper.runToPosition(); 
  // Now stopped after quickstop
  
  //cmd.respond(String("position="+destpos));
  cmd.respond("position="+String(destpos)+":ok");
}

// id=a1:movetofeedback:15000:1000;
// id=a1:movetofeedback:500000:1000;
void movetofeedback(String argv[]) {

  long int destpos = (long int)cmd.stof(argv[0]);  // use float for higher number
  long int feedback = (long int)cmd.stof(argv[1]);  // use float for higher number
  
  destpos = direction * destpos; // set direction
  stepper.moveTo(destpos);
  long int curpos = 0;
  while ((curpos = stepper.currentPosition()) != destpos)
  {
    if((curpos % feedback) == 0)
      cmd.respond("curpos="+String(curpos)+":beacon");
    stepper.run();
  }
  cmd.respond("curpos="+String(curpos)+":beacon");
  stepper.stop(); // Stop as fast as possible: sets new target
  stepper.runToPosition(); 
  // Now stopped after quickstop
  
  //cmd.respond(String("position="+destpos));
  cmd.respond("position="+String(destpos)+":ok");
}

void setup() {
  stepper.setMaxSpeed(6000);
  stepper.setAcceleration(3000);
  //stepper.setSpeed(2000);
  
  Serial.begin(115200);
  cmd.ok(); // say hello
}

void loop() {  
  cmd.cmdCheck();
}
