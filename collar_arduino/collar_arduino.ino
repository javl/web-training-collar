#include <SoftwareSerial.h>
SoftwareSerial mySerialOut(3, 4, true);
bool receiving_command = false;

// All of the available commands
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
  pinMode(13, OUTPUT);
  Serial.begin(4800);
  mySerialOut.begin(4800);
  
  delay(1000);
  playCmd(0);
  delay(1000);
  playCmd(2);
}

void loop() {
  if (Serial.available() > 0) {
    processInput();
  }
}

void processInput () {
  static long receivedNumber = 0;
  static bool receivingTimer = false;

  const byte c = Serial.read ();
  Serial.println("Incoming");
  Serial.println(c);
  switch (c) {
    case '0' ... '9':
      if (receiving_command) {
        receivedNumber *= 10;
        receivedNumber += c - '0';
      }
      break;
    case '>':
      receiving_command = false;
      receivedNumber -= 1;
      if(receivedNumber >= 0){
        Serial.print("play number: ");
        Serial.println(receivedNumber);
        playCmd(receivedNumber);
      }
      break;
    case '<':
      //      Serial.println("start command");
      receiving_command = true;
      receivedNumber = 0;
      break;
  }
}
// Play back one of the commands. You seem to get the best results when sending the
// command twice instead of just once
void playCmd(int index) {
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
