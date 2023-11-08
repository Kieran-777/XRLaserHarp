#include <Stepper.h>

int LaserState = LOW;                    // The variable that stores the state of the laser beam.
int LaserPin =  7;                       // Tell the arduino that the laser is on pin 7
int sensor = 8 ;                         // Change this value to calibrate your harp's sensor
int delayLaser = 5;                     // If you increase this, the laser will be brighter, but the harp will be less fluid
int delayMotor = 200;                     // This variable affects the speed, and fluidity of the harp.

int beamSteps = 20;                      // Number of steps to take
int stepSpeed = 10;                     // Stepper motor speed

const int stepsPerRevolution = 2038;    // Defines the number of steps per rotation

Stepper myStepper = Stepper(stepsPerRevolution, 8, 10, 9, 11); // Assign pins to stepper

int note10 = 0x60;
int note9 = 0x61;
int note8 = 0x63;
int note7 = 0x64;
int note6 = 0x66;
int note5 = 0x68;
int note4 = 0x70;
int note3 = 0x71;
int note2 = 0x40;
int note1 = 0x47;

int a, b, c, d, e, f, g, h, i, j = 0;       // Iniating the note status markers.

const int beamCount = 10;
int* noteStatuses[beamCount] = {&a, &b, &c, &d, &e, &f, &g, &h, &i, &j};
int notes[beamCount] = {note1, note2, note3, note4, note5, note6, note7, note8, note9, note10};


void setup() {
	pinMode(LaserPin, OUTPUT);            // Setup for laser.
	pinMode(13, OUTPUT);                 // Setup for status led.
	Serial.begin(9600);                 //  Start a serial communication channel for MIDI

}


void noteOn(int cmd, int pitch, int velocity)     // Function to play the notes
{
	Serial.write(cmd);
	Serial.write(pitch);
	Serial.write(velocity);
}


void beamBlock(int note, int &noteStatus, int pol)
{
	digitalWrite(LaserPin, HIGH);               // Turn on the laser
	delay(delayLaser);

	if ( (analogRead(0) > sensor ) && (noteStatus == 0) ) // If the sensor gets a signal, and the note is not playing:
	{
		digitalWrite(13, HIGH);                   // Switch on status led.
		//  noteOn(0x90, note, 0x7F);                 // Play note 1
		//  noteStatus++;                             // Change the status variable to one.

	}
	else if(analogRead(0) < sensor )            // If the sensor does not get a signal:
	{
		digitalWrite(13, LOW);                    // Switch off the status led.
		//  noteOn(0x90, note, 0x00);                 // Stop playing note 1.
		//  noteStatus = 0;                                    // Change the status variable to zero.
	}
	digitalWrite(LaserPin, LOW);                // Turn off the Laser.

  myStepper.setSpeed(stepSpeed);              // Set stepper speed
  myStepper.step(beamSteps * pol);                  // Move stepper
  delay(delayMotor);   
}

  int dynBeam = 9;

void loop() {
  
  // Check if there's any Serial input
  if (Serial.available() > 0) {
    char touchBeam = Serial.read();
    // Assuming the input is a single digit followed by newline
    if (touchBeam >= '0' && touchBeam <= '9') {
      dynBeam = touchBeam - '0';
    }
  }

 

  // Accesding
  for (int beam = 0; beam < dynBeam; beam++) {
  Serial.print("beam ");
  Serial.print(beam);
  Serial.println(" active");
  beamBlock(notes[beam], *noteStatuses[beam], 1);

    char touchBeam = Serial.read();
    // Assuming the input is a single digit followed by newline
    if (touchBeam >= '0' && touchBeam <= '9') {
      dynBeam = touchBeam - '0';
    }

  }

  // Decending
  for (int beam = dynBeam - 1; beam >= 0; beam--) {
  Serial.print("beam ");
  Serial.print(beam);
  Serial.println(" active");
  beamBlock(notes[beam], *noteStatuses[beam], -1);

        char touchBeam = Serial.read();
    // Assuming the input is a single digit followed by newline
    if (touchBeam >= '0' && touchBeam <= '9') {
      dynBeam = touchBeam - '0';
    }

  }

}
