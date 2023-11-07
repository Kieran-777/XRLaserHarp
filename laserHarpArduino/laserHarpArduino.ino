#include <Stepper.h>

int LaserState = LOW;                    // The variable that stores the state of the laser beam.
int LaserPin =  7;                       // Tell the arduino that the laser is on pin 7
int sensor = 8 ;                         // Change this value to calibrate your harp's sensor
int delayLaser = 5;                     // If you increase this, the laser will be brighter, but the harp will be less fluid
int delayMotor = 3;                     // This variable affects the speed, and fluidity of the harp.

int beamSteps = 1;                      // Number of steps to take
int stepSpeed = 200;                     // Stepper motor speed

const int stepsPerRevolution = 2038;    // Defines the number of steps per rotation

Stepper myStepper = Stepper(stepsPerRevolution, 8, 10, 9, 11); // Assign pins to stepper

int note9 = 0x61;
int note8 = 0x63;
int note7 = 0x64;
int note6 = 0x66;
int note5 = 0x68;
int note4 = 0x70;
int note3 = 0x71;
int note2 = 0x40;
int note1 = 0x47;

int a, b, c, d, e, f, g, h, i = 0;       // Iniating the note status markers.

const int beamCount = 8;
int* noteStatuses[beamCount] = {&a, &b, &c, &d, &e, &f, &g, &h};
int notes[beamCount] = {note1, note2, note3, note4, note5, note6, note7, note8};


void setup() {
	pinMode(LaserPin, OUTPUT);            // Setup for laser.
	pinMode(13, OUTPUT);                 // Setup for status led.
	Serial.begin(31250);                 //  Start a serial communication channel for MIDI

}

void noteOn(int cmd, int pitch, int velocity)     // Function to play the notes
{
	Serial.write(cmd);
	Serial.write(pitch);
	Serial.write(velocity);
}

void beamBlock(int note, int &noteStatus, int pol)
{
	digitalWrite(LaserPin, HIGH);               // Turn on the laser for the 1st beam.
	delay(delayLaser);

	if ( (analogRead(0) > sensor ) && (noteStatus == 0) ) // If the sensor gets a signal, and the note is not playing:
	{
		digitalWrite(13, HIGH);                   // Switch on status led.
		noteOn(0x90, note, 0x7F);                 // Play note 1
		noteStatus++;                             // Change the status variable to one.
	}
	else if(analogRead(0) < sensor )            // If the sensor does not get a signal:
	{
		digitalWrite(13, LOW);                    // Switch off the status led.
		noteOn(0x90, note, 0x00);                 // Stop playing note 1.
		noteStatus = 0;                                    // Change the status variable to zero.
	}
	digitalWrite(LaserPin, LOW);                // Turn off the Laser.

  myStepper.setSpeed(stepSpeed);              // Set stepper speed
  myStepper.step(beamSteps * pol);                  // Move stepper
  delay(delayMotor);   
}



void loop() {

  // Accesding
  for (int i = 0; i < beamCount; i++) {
    beamBlock(notes[i], *noteStatuses[i], 1);
  }
  // Decending
  for (int i = beamCount - 1; i >=0; i--) {
    beamBlock(notes[i], *noteStatuses[i], -1);
  }

}
