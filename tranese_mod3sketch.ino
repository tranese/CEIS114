// === @tranese ====
// Mod3: Setup Main St. traffic lights. (LED1)

// assign LEDs to ESP32 pins
const int yelLED1 = 12;  // Main yellow LED to GPIO12
const int grnLED1 = 13;  // Main green LED to GPIO13
const int redLED1 = 14;  // Main red LED to GPIO14

// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(yelLED1, OUTPUT);  // initialize yelLED1(GPIO12) to output
  pinMode(grnLED1, OUTPUT);  // initialize grnLED1(GPIO13) to output
  pinMode(redLED1, OUTPUT);  // initialize redLED1(GPIO14) to output
}

// the loop function runs over and over again forever
void loop() {
  // Main light red
  digitalWrite(redLED1, HIGH);  // Main red LED on
  digitalWrite(yelLED1, LOW);   // Main yellow LED off
  digitalWrite(grnLED1, LOW);   // Main green LED off

  delay(2000);  // wait for 2 seconds

  // Main light green
  digitalWrite(redLED1, LOW);   // Main red LED off
  digitalWrite(yelLED1, LOW);   // Main yellow LED off
  digitalWrite(grnLED1, HIGH);  // Main green LED on

  delay(2000);  // wait for 2 seconds

  // Main light yellow
  digitalWrite(redLED1, LOW);   // Main red LED off
  digitalWrite(yelLED1, HIGH);  // Main yellow LED on
  digitalWrite(grnLED1, LOW);   // Main green LED off

  delay(2000);  // wait for 2 seconds
}
