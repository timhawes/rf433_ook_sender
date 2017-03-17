const int txPin = 10;

unsigned int highOnTime = 1125;
unsigned int highOffTime = 375;
unsigned int lowOnTime = 375;
unsigned int lowOffTime = 1125;
unsigned int gapTime = 10000;
unsigned int repeat = 10;
unsigned int preambleHighTime = 0;
unsigned int preambleLowTime = 0;

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

void setup() {
  pinMode(txPin, OUTPUT);

  // initialize serial:
  Serial.begin(115200);
  // while the serial stream is not open, do nothing:
  while (!Serial) ;
  Serial.println("RF433 OOK sender 1.0 ready");
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
}

void loop() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n' || inChar == '\r') {
      stringComplete = true;
      break;
    } else {
      // add it to the inputString:
      inputString += inChar;
    }
  }

  // print the string when a newline arrives:
  if (stringComplete) {
    switch (inputString[0]) {
      case 'A':
        highOnTime = inputString.substring(1).toInt();
        break;
      case 'B':
        highOffTime = inputString.substring(1).toInt();
        break;
      case 'C':
        lowOnTime = inputString.substring(1).toInt();
        break;
      case 'D':
        lowOffTime = inputString.substring(1).toInt();
        break;
      case 'E':
        gapTime = inputString.substring(1).toInt();
        break;
      case 'F':
        repeat = inputString.substring(1).toInt();
        break;
      case 'G':
        preambleHighTime = inputString.substring(1).toInt();
        break;
      case 'H':
        preambleLowTime = inputString.substring(1).toInt();
        break;
      case 'X':
        Serial.print(highOnTime); Serial.print(" ");
        Serial.print(highOffTime); Serial.print(" ");
        Serial.print(lowOnTime); Serial.print(" ");
        Serial.print(lowOffTime); Serial.print(" ");
        Serial.print(gapTime); Serial.print(" ");
        Serial.print(repeat); Serial.print(" ");
        Serial.print(preambleHighTime); Serial.print(" ");
        Serial.print(preambleLowTime); Serial.print(" ");
        Serial.println(repeat);
        break;
      case 'Z':
        send_message(inputString.substring(1));
        break;
      default:
        //Serial.println("ERROR");
        break;
    }
    
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}

void sendbit(boolean b) {
  if (b) {
    digitalWrite(txPin, HIGH);
    delayMicroseconds(highOnTime);
    digitalWrite(txPin, LOW);
    delayMicroseconds(highOffTime);
  } else{
    digitalWrite(txPin, HIGH);
    delayMicroseconds(lowOnTime);
    digitalWrite(txPin, LOW);
    delayMicroseconds(lowOffTime);
  }
}

void send_message(String msg) {
  for (unsigned int r=0; r<repeat; r++) {
    if (preambleHighTime > 0 || preambleLowTime > 0) {
      digitalWrite(txPin, HIGH);
      delayMicroseconds(preambleHighTime);
      digitalWrite(txPin, LOW);
      delayMicroseconds(preambleLowTime);
    }
    for (unsigned int i=0; i<msg.length(); i++) {
      if (msg[i]=='1') {
        sendbit(1);
      } else if (msg[i]=='0') {
        sendbit(0);
      }
    }
    delayMicroseconds(gapTime);
  }
}

