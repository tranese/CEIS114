// === @tranese ====
// Mod5: Setup crosswalk button

// assign LEDs to ESP32 pins.  LED1 = Main St.  LED2 = Side St.
const int yelLED1 = 12;  // Main yellow LED to GPIO12
const int grnLED1 = 13;  // Main green LED to GPIO13
const int redLED1 = 14;  // Main red LED to GPIO14

const int redLED2 = 25;  // 2nd red LED to GPIO25
const int yelLED2 = 26;  // 2nd yellow LED to GPIO26
const int grnLED2 = 27;  // 2nd green LED to GPIO27

int xwButtonState = 1;    // variable: stores xwButton state
const int xwButton = 19;  // white pushbutton to GPIO19

int secsLeft;  // variable: countdown remaining seconds

// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(xwButton, INPUT_PULLUP);  // 0 = pressed 1 = no press
  attachInterrupt(xwButton, buttonPressed, FALLING);

  Serial.begin(115200);  //set baud rate

  pinMode(yelLED1, OUTPUT);  // initialize yelLED1(GPIO12) to output
  pinMode(grnLED1, OUTPUT);  // initialize grnLED1(GPIO13) to output
  pinMode(redLED1, OUTPUT);  // initialize redLED1(GPIO14) to output
  pinMode(redLED2, OUTPUT);  // initialize redLED2(GPIO25) to output
  pinMode(yelLED2, OUTPUT);  // initialize yelLED2(GPIO26) to output
  pinMode(grnLED2, OUTPUT);  // initialize grnLED2(GPIO27) to output
}

void loop() {
  // xwButton
  // xwButtonState = digitalRead(xwButton);
  if (xwButtonState == 0) {
    digitalWrite(yelLED1, LOW);  // Main yellow LED off
    digitalWrite(grnLED1, LOW);  // Main green LED off
    digitalWrite(yelLED2, LOW);  // 2nd yellow LED off
    digitalWrite(grnLED2, LOW);  // 2nd green LED off

    secsLeft = 15;  // set countdown timer

  while (secsLeft > 0) {
    if (secsLeft >= 10) {
      Serial.print("   -= Walk =-   ");
      Serial.println(secsLeft);
    } else {
      Serial.print("   -= Walk =-   ");
      Serial.println(secsLeft);
    }
    digitalWrite(redLED1, HIGH);  // Main red LED on
    digitalWrite(redLED2, HIGH);  // 2nd red LED on
    delay(500);                   // .5s delay (flash on)

    digitalWrite(redLED1, LOW);  // Main red LED off
    digitalWrite(redLED2, LOW);  // 2nd red LED off
    delay(500);                  // .5s delay (flash off)
    secsLeft -= 1;
  }  // end for loop
    xwButtonState = 1;
  }     // end if statement
  else  // no xwButton detected
  {
    Serial.println(" --- NO CROSSING ---");
    // Main light red
    digitalWrite(redLED1, HIGH);  // Main red LED on
    digitalWrite(yelLED1, LOW);   // Main yellow LED off
    digitalWrite(grnLED1, LOW);   // Main green LED off
    delay(1000);                  // 2s delay

    // 2nd light green
    digitalWrite(redLED2, LOW);   // 2nd red LED off
    digitalWrite(yelLED2, LOW);   // 2nd yellow LED off
    digitalWrite(grnLED2, HIGH);  // 2nd green LED on
    delay(2000);                  // 2s delay

    // Main light red
    digitalWrite(redLED1, HIGH);  // Main red LED on
    digitalWrite(yelLED1, LOW);   // Main yellow LED off
    digitalWrite(grnLED1, LOW);   // Main green LED off

    // 2nd light yellow
    digitalWrite(redLED2, LOW);   // 2nd red LED off
    digitalWrite(yelLED2, HIGH);  // 2nd yellow LED on
    digitalWrite(grnLED2, LOW);   // 2nd green LED off
    delay(2000);                  // 2s delay

    // 2nd light red
    digitalWrite(redLED2, HIGH);  // 2nd red LED on
    digitalWrite(yelLED2, LOW);   // 2nd yellow LED off
    digitalWrite(grnLED2, LOW);   // 2nd green LED off
    delay(1000);                  // 1s delay for transition

    // Main light green
    digitalWrite(redLED1, LOW);   // Main red LED off
    digitalWrite(yelLED1, LOW);   // Main yellow LED off
    digitalWrite(grnLED1, HIGH);  // Main green LED on
    delay(2000);                  // 2s delay

    // Main light yellow
    digitalWrite(redLED1, LOW);   // Main red LED off
    digitalWrite(yelLED1, HIGH);  // Main yellow LED on
    digitalWrite(grnLED1, LOW);   // Main green LED off
    delay(2000);                  // 2s delay
  }                               // close xwButton
}
void buttonPressed() 
{
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();

  if (interruptTime - lastInterruptTime > 20) 
  {
    xwButtonState = 0;
  }
    lastInterruptTime = interruptTime;
}