#include <Stepper.h>

int LaserState = LOW;                      // The variable that stores the state of the laser beam.
int LaserPin =  7;                        //  Tell the arduino that the laser is on pin 7
int sensor = 4 ;                         //   Change this value to calibrate your harp's sensor
int delayLaser = 5;                     //    If you increase this, the laser will be brighter, but the harp will be less fluid

int delayMotor = 200;                     // This variable affects the speed, and fluidity of the harp.
int beamSteps = 20;                      //  Number of steps to take
int stepSpeed = 10;                     //   Stepper motor speed
const int stepsPerRevolution = 2038;   //    Defines the number of steps per rotation
Stepper myStepper = Stepper(stepsPerRevolution, 8, 10, 9, 11); // Assign pins to stepper

const int modeButtonPin = 2;        // Mode button pin
int modeState = 0;               //  Mode state

int touchCommand = 0;           // default to calibration mode

void setup() {
	pinMode(LaserPin, OUTPUT);            // Setup for laser.
	pinMode(13, OUTPUT);                 //  Setup for status led.
  pinMode(modeButtonPin, INPUT);       // Calibartion button as input
  int touchCommand = 0;               //   Start on Callibration mode
	Serial.begin(9600);                //   Start a serial communication channel

}


char readSerial()
{
  char touchSerial = Serial.read();
  if (touchSerial >= '0' && touchSerial <= '9') { // convert serial string to int
    touchCommand = touchSerial - '0';
  }
  return touchCommand;
}


void beamBlock(int beam, int pol)
{
  Serial.print("beam - ");
  Serial.println(beam);

	digitalWrite(LaserPin, HIGH);               // Turn on the laser
	delay(delayLaser);

	if (analogRead(0) > sensor ) // && (noteStatus == 0) ) // If the sensor gets a signal, and the note is not playing:
	{
		digitalWrite(13, HIGH);                   // Switch on status led.
	}
	else if (analogRead(0) < sensor )            // If the sensor does not get a signal:
	{
		digitalWrite(13, LOW);                    // Switch off the status led.
	}

	digitalWrite(LaserPin, LOW);                // Turn off the Laser.

  myStepper.setSpeed(stepSpeed);              // Set stepper speed
  myStepper.step(beamSteps * pol);                  // Move stepper
  delay(delayMotor);   
}








void calibrationMode()
{
  Serial.println("CALIBRATION mode ACTIVE");
  myStepper.setSpeed(stepSpeed);                 // Set stepper to standard speed
  Serial.println("starting calibration");
  myStepper.step(500);                           //  Move stepper to center (1.8deg per step, so 50 brings to 90deg)
  Serial.println("x steps taken");
  while (digitalRead(modeButtonPin)  == HIGH){} //   Wait for the user to move the stepper into position
  Serial.println("Calibration Complete!");
  selectMode();                               //    With the calibration complete we move onto select mode
}







void selectMode()
{
Serial.println("SELECT mode ACTIVE");

  const int modeBeams = 8;    // number of beams in mode
  int selectedMode = 1;       // var for mode select

  while (selectedMode == 1){          // until they select a mode

    // Ascending
    for (int beam = 0; beam < modeBeams; beam++) {
      beamBlock(beam, 1);
      char input = readSerial();
      if (input == 2){
        Serial.println("piano mode selected");
        selectedMode = 2;      // Update the correct selectedMode variable
        break;
      }
    }

    // Descending
        for (int beam = modeBeams - 1; beam >= 0; beam--) {
      beamBlock(beam, -1);
      char selectedMode = readSerial();
      if (selectedMode == 2){
        Serial.println("piano mode selected");
        selectedMode = 2;      // Update the correct selectedMode variable
        break;
      }
    }


  }
          if (selectedMode == 2){
        pianoMode();
          }

}








void pianoMode()
{
Serial.println("PIANO mode ACTIVE");

  const int modeBeams = 8;

  bool buttonPressed = false;

  while (!buttonPressed){          // until the mode button is pressed

    // Ascending
    for (int beam = 0; beam < modeBeams; beam++) {
      beamBlock(beam, 1);
      if (digitalRead(modeButtonPin) == LOW) { // Assuming LOW means button pressed
        buttonPressed = true;
        break; // Break out of the for loop
      }
    }
  }
  selectMode();

}







void loop() 
{
  modeState = digitalRead(modeButtonPin);

    if (modeState == LOW) {
    // turn LED on:
    digitalWrite(13, HIGH);
  } else {
    // turn LED off:
    digitalWrite(13, LOW);
  }

  // // Check if there's any Serial input
  // if (Serial.available() > 0) {
  //   char touchSerial = Serial.read();
  //   if (touchSerial >= '0' && touchSerial <= '9') { // convert serial string to int
  //     touchCommand = touchSerial - '0';
  //   }
  // }

  Serial.println("###################");
  Serial.println("#  XR-LASER-HARP  #");
  Serial.println("###################");

  if (touchCommand == 0) {
    calibrationMode();
  }
  // else if (touchCommand == 1){
  //   selectMode();
  // }

}
