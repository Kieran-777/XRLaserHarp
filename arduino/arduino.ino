#include <Stepper.h>

int LaserState = LOW;                      // The variable that stores the state of the laser beam.
int LaserPin =  7;                        //  Tell the arduino that the laser is on pin 7
int sensor = 8 ;                         //   Change this value to calibrate your harp's sensor
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


void readSerial()
{
  char touchSerial = Serial.read();
  if (touchSerial >= '0' && touchSerial <= '9') { // convert serial string to int
    touchCommand = touchSerial - '0';
  }
  return touchCommand;
}


void beamBlock(int beam, int pol)
{
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
  myStepper.setSpeed(stepSpeed);                 // Set stepper to standard speed
  myStepper.step(50);                           //  Move stepper to center (1.8deg per step, so 50 brings to 90deg)
  while (digitalRead(modeButtonPin)  == LOW){} //   Wait for the user to move the stepper into position
  selectMode();                               //    With the calibration complete we move onto select mode
}







void selectMode()
{
  const int modeBeams = 8;    // number of beams in mode
  int selectedMode = 0;       // var for mode select

  while (selectedMode == 0){          // until they select a mode

    // Ascending
    for (int beam = 0; beam < modeBeams; beam++) {
      beamBlock(beam, 1);
      char touchSerial = Serial.read();
      if (touchSerial >= '0' && touchSerial <= '9') { // convert serial string to int
        touchCommand = touchSerial - '0';
      }
      if (touchCommand == 3){
        pianoMode();
      }
    }

    // Descending
        for (int beam = modeBeams - 1; beam >= 0; beam--) {
      beamBlock(beam, 1);
      char touchSerial = Serial.read();
      if (touchSerial >= '0' && touchSerial <= '9') { // convert serial string to int
        touchCommand = touchSerial - '0';
      }
      if (touchCommand == 3){
        pianoMode();
      }
    }

  }

}








void pianoMode()
{
  const int modeBeams = 8;
  while (digitalRead(modeButtonPin) == LOW){          // until the mode button is pressed

  // Ascending
  for (int beam = 0; beam < modeBeams; beam++) {
    beamBlock(beam, 1)
  }

  

  }

}







void loop() 
{

  // Check if there's any Serial input
  if (Serial.available() > 0) {
    char touchSerial = Serial.read();
    if (touchSerial >= '0' && touchSerial <= '9') { // convert serial string to int
      touchCommand = touchSerial - '0';
    }
  }

  if (touchCommand == 0) {
    calibrationMode();
  }
  else if (touchCommand == 1){
    selectMode();
  }

}
