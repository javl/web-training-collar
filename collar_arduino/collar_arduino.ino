// The Arduino code for the Web User Training Collar
// http://github.com/javl/webUserTrainingCollar

#include <SoftwareSerial.h>

// SoftwareSerial is used with the inverted flag set to true.
// This way an inverted serial signal is used (idle = low, active = high), which is required
// by the receiver of the particular collar I'm using. This could be different for other types.
SoftwareSerial mySerialOut(3, 4, true);

// All of the available commands for my receiver, sniffed using a logic analyzer
const uint8_t commands[49][22] = {
	//0 beep
	{192, 232, 239, 15, 239, 239, 8, 15, 232, 232, 239, 8, 15, 15, 232, 15, 239, 8, 15, 8, 232, 255},
	//1 light
	{192, 232, 239, 232, 239, 239, 8, 15, 232, 232, 239, 8, 15, 15, 232, 15, 239, 8, 232, 8, 232, 255},
	//2 vibrate 1
	{192, 232, 239, 239, 232, 239, 8, 15, 232, 232, 239, 8, 15, 239, 239, 239, 15, 232, 8, 8, 232, 255},
	//3 vibrate 5
	{192, 232, 239, 239, 232, 239, 8, 15, 232, 232, 239, 8, 15, 239, 239, 15, 15, 232, 8, 8, 232, 255},
	//4 vibrate 10
	{192, 232, 239, 239, 232, 239, 8, 15, 232, 232, 239, 8, 15, 239, 239, 232, 232, 232, 8, 8, 232, 255},
	//5 vibrate 50
	{192, 232, 239, 239, 232, 239, 8, 15, 232, 232, 239, 8, 15, 239, 8, 239, 232, 232, 8, 8, 232, 255},
	//6 vibrate 100
	{192, 232, 239, 239, 232, 239, 8, 15, 232, 232, 239, 8, 15, 15, 232, 15, 239, 232, 8, 8, 232, 255},
	//7 shock 1
	{192, 232, 239, 239, 15, 239, 8, 15, 232, 232, 239, 8, 15, 239, 239, 239, 15, 15, 8, 8, 232, 255},
	//8 shock 2
	{192, 232, 239, 239, 15, 239, 8, 15, 232, 232, 239, 8, 15, 239, 239, 239, 232, 15, 8, 8, 232, 255},
	//9 shock 3
	{192, 232, 239, 239, 15, 239, 8, 15, 232, 232, 239, 8, 15, 239, 239, 239, 8, 15, 8, 8, 232, 255},
	//10 shock 4
	{192, 232, 239, 239, 15, 239, 8, 15, 232, 232, 239, 8, 15, 239, 239, 15, 239, 15, 8, 8, 232, 255},
	//11 shock 5
	{192, 232, 239, 239, 15, 239, 8, 15, 232, 232, 239, 8, 15, 239, 239, 15, 15, 15, 8, 8, 232, 255},
	//12 shock 6
	{192, 232, 239, 239, 15, 239, 8, 15, 232, 232, 239, 8, 15, 239, 239, 15, 232, 15, 8, 8, 232, 255},
	//13 shock 7
	{192, 232, 239, 239, 15, 239, 8, 15, 232, 232, 239, 8, 15, 239, 239, 15, 8, 15, 8, 8, 232, 255},
	//14 shock 8
	{192, 232, 239, 239, 15, 239, 8, 15, 232, 232, 239, 8, 15, 239, 239, 232, 239, 15, 8, 8, 232, 255},
	//15 shock 9
	{192, 232, 239, 239, 15, 239, 8, 15, 232, 232, 239, 8, 15, 239, 239, 232, 15, 15, 8, 8, 232, 255},
	//16 shock 10
	{192, 232, 239, 239, 15, 239, 8, 15, 232, 232, 239, 8, 15, 239, 239, 232, 232, 15, 8, 8, 232, 255},
	//17 shock 20
	{192, 232, 239, 239, 15, 239, 8, 15, 232, 232, 239, 8, 15, 239, 15, 15, 239, 15, 8, 8, 232, 255},
	//18 shock 30
	{192, 232, 239, 239, 15, 239, 8, 15, 232, 232, 239, 8, 15, 239, 15, 8, 232, 15, 8, 8, 232, 255},
	//19 shock 40
	{192, 232, 239, 239, 15, 239, 8, 15, 232, 232, 239, 8, 15, 239, 232, 232, 239, 15, 8, 8, 232, 255},
	//20 shock 50
	{192, 232, 239, 239, 15, 239, 8, 15, 232, 232, 239, 8, 15, 239, 8, 239, 232, 15, 8, 8, 232, 255},
	//21 shock 60
	{192, 232, 239, 239, 15, 239, 8, 15, 232, 232, 239, 8, 15, 239, 8, 8, 239, 15, 8, 8, 232, 255},
	//22 shock 70
	{192, 232, 239, 239, 15, 239, 8, 15, 232, 232, 239, 8, 15, 15, 239, 15, 232, 15, 8, 8, 232, 255},
	//23 shock 80
	{192, 232, 239, 239, 15, 239, 8, 15, 232, 232, 239, 8, 15, 15, 15, 239, 239, 15, 8, 8, 232, 255},
	//24 shock 90
	{192, 232, 239, 239, 15, 239, 8, 15, 232, 232, 239, 8, 15, 15, 15, 232, 232, 15, 8, 8, 232, 255},
	//25 shock 100
	{192, 232, 239, 239, 15, 239, 8, 15, 232, 232, 239, 8, 15, 15, 232, 15, 239, 15, 8, 8, 232, 255}
};

void setup() {
	Serial.begin(4800); // Regular serial for the local server
	mySerialOut.begin(4800); // inverted software serial for the transmitter

	// Wait for a second, then send a command to pair with the collar
	// The actual command doesn't matter, I'm using a beep followed by a vibrate command
	// to get some feedback on sucessful pairing
	delay(1000);
	sendCmd(0);  // Send beep signal
	delay(1000);
	sendCmd(2);  // Send vibrate signal
}

// Check for incoming serial in the main loop
void loop() {
	if (Serial.available() > 0) { // If serial available
		int val = Serial.read();
		val = val - '0'; // Convert char to int
		if(val >= 0 && val < 49){
			sendCmd(val);
		} 
	}
}

// Send one of the commands. You seem to get the best results when sending the
// command twice instead of just once
void sendCmd(int index) {
	// Disable interrupt (otherwise the serial connection on my Arduino Nano
	// is too slow for some reason)
	uint8_t oldSREG = SREG;
	cli();
	delay(200);
	// Sometimes the signal isn't registered properly, so send
	// it out twice just to be sure
	for (int i = 0; i < 22; i++) {
		mySerialOut.write(commands[index][i]);
	}
	delay(20);
	for (int i = 0; i < 22; i++) {
		mySerialOut.write(commands[index][i]);
	}
	delay(200);
	SREG = oldSREG; // turn interrupts back on
	// Turn the LED on for half a second to provide some visual feedback	
	digitalWrite(13, HIGH);
	delay(500);
	digitalWrite(13, LOW);
}
