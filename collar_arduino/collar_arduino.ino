#include <SoftwareSerial.h>

// SoftwareSerial is used with the inverted flag set to true.
// This way an inverted serial signal is used (idle = low, active = high), which is required
// by the receiver of the particular collar I'm using. This could be different for other types.
SoftwareSerial mySerialOut(3, 4, true);

bool receiving_command = false;

// All of the available commands for my receiver
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
  // Open a serial connection to the local server (regular, non-inverted serial)
  Serial.begin(4800);
  // mySerialOut is used to send serial data (inverted) to the collar
  mySerialOut.begin(4800);

  // Wait for a bit, than send two signals to the collar. If the collar has been turned on, the
  // first signal should pair it with the transmitter. The second will make it vibrate, to provide
  // some feedback on the successful pairing  
  delay(1000); // Wait one second
  sendCmd(0);  // Send pairing signal (0 = beep, but any will do)
  delay(1000); // Wait one second
  sendCmd(2);  // Send command 2: vibrate
}

// The main loop that checks for incoming Serial data from
// the server. If received, go over to processsInput()
void loop() {
  if (Serial.available() > 0) {
    processInput();
  }
}

// The incoming signal is a bit weird. I had some trouble sending/receiving just numbers so
// they are wrapped in a longer string like so: <10> (including the < and > characters).
void processInput () {
  // Holds the number that came in
  static long receivedNumber = 0;

  // Get the first available byte
  const byte c = Serial.read ();
  switch (c) {
    // A new command is coming in, reset the receivedNumber
    case '<':
      receiving_command = true;
      receivedNumber = 0;
      break;
    // Numbers, part of the command.
    case '0' ... '9': 
      if (receiving_command) {
        receivedNumber *= 10;
        receivedNumber += c - '0';
      }
      break;
    // The closing delimiter, so the currently saved receivedNumber is the
    // entire command 
    case '>':
      receiving_command = false; // No longer waiting for more digits for this command
      // Remove 1 to translate the incoming command number to an array index.
      receivedNumber -= 1;
      if(receivedNumber >= 0){ // If all went OK, nothing below 0 should be coming in
	// Send all the bytes of the command
        sendCmd(receivedNumber);
      }
      break;
  }
}

// Send one of the commands. You seem to get the best results when sending the
// command twice instead of just once
void sendCmd(int index) {
  // Disable interrupt (otherwise the serial data will be send out in packets, which is too slow
  uint8_t oldSREG = SREG;
  cli();
  delay(200);
  for (int i = 0; i < 22; i++) {
    mySerialOut.write(commands[index][i]);
  }
  delay(20);
  for (int i = 0; i < 22; i++) {
    mySerialOut.write(commands[index][i]);
  }
  delay(20);
  for (int i = 0; i < 22; i++) {
    mySerialOut.write(commands[index][i]);
  }
  delay(20);
  for (int i = 0; i < 22; i++) {
    mySerialOut.write(commands[index][i]);
  }
  delay(200);
  SREG = oldSREG; // turn interrupts back on
  digitalWrite(13, HIGH);
  delay(500);
  digitalWrite(13, LOW);
}
