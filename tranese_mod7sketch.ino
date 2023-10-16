// === @tranese ====
// Mod7: Integrate wireless & implement MQTT connection (Option #1)

#include <Wire.h>               // I2C header file
#include <LiquidCrystal_I2C.h>  // LCD header file
#include <WiFi.h>               // Wi-Fi header file
#include <PubSubClient.h>       // MQTT header file (publish/subscribe)

const char* ssid = "GL-MT300N-V2-644";          // wi-fi access point
const char* password = "goodlife";              // wi-fi password
const char* mqttServer = "test.mosquitto.org";  // MQTT broker
int port = 1883;                                //  MQTT listening port
String stMac;                                   // c-string comparison convenience
char mac[50];                                   // c-string array holds ESP32 MAC address
char clientId[50];                              // client ID of user accessing of MQTT broker

WiFiClient espClient;            //  instantiate Wi-Fi client object
PubSubClient client(espClient);  //  instantiate publish/subscribe client object

LiquidCrystal_I2C LCD(0x27, 16, 2);  // LCD address - SDA to GPIO 21  + LCD SCL to GPIO 22

// assign LEDs to ESP32 pins.  LED1 = Main St.  LED2 = Side St.
const int yelLED1 = 12;  // Main yellow LED to GPIO12
const int grnLED1 = 13;  // Main green LED to GPIO13
const int redLED1 = 14;  // Main red LED to GPIO14

const int blueLED = 23;  // Blue emergency light to GPIO16

const int xwButton = 19;  // white pushbutton to GPIO19

const int redLED2 = 25;  // 2nd red LED to GPIO25
const int yelLED2 = 26;  // 2nd yellow LED to GPIO26
const int grnLED2 = 27;  // 2nd green LED to GPIO27

const int buzzer = 32;  // active buzzer to GPIO32

int xwButtonState = 1;  // variable: stores xwButton state
int loopCount;          // variable: count light pattern loops
int secsLeft;           // variable: countdown remaining seconds
int iotControl = 0;     // varible: switch- xwButton/normal traffic


// setup function - runs on power cycle
void setup() {
  Serial.begin(115200);       // set baud rate
  randomSeed(analogRead(0));  // seed: random(function)
  delay(10);                  // .1s delay

  //  print strings & SSID to serial monitor
  Serial.println("");
  Serial.print("Attempting connection with ");
  Serial.println(ssid);

  WiFiConnect();  // initiate Wi-FI access point connection

  Serial.println("");                        // skip line serial monitor
  Serial.println("Connection successful!");  //  print string to serial monitor
  Serial.println("The IP address is: ");     //  print string to serial monitor
  Serial.println(WiFi.localIP());            //  print local IP to serial monitor
  Serial.println(WiFi.macAddress());         //  print MAC address to serial monitor
  stMac = WiFi.macAddress();                 //  copy/store MAC address
  stMac.replace(":", "_");                   //  replace : with _
  Serial.println(stMac);                     //  print clean MAC address to serial monitor

  client.setServer(mqttServer, port);  // connect to MQTT server
  client.setCallback(callback);        //  set callback function

  pinMode(xwButton, INPUT_PULLUP);  // set xwButton buttonPressed to input
  attachInterrupt(xwButton, buttonPressed, FALLING);

  pinMode(blueLED, OUTPUT);  // initialize blueLED(GPIO23) to output
  pinMode(yelLED1, OUTPUT);  // initialize yelLED1(GPIO12) to output
  pinMode(grnLED1, OUTPUT);  // initialize grnLED1(GPIO13) to output
  pinMode(redLED1, OUTPUT);  // initialize redLED1(GPIO14) to output
  pinMode(redLED2, OUTPUT);  // initialize redLED2(GPIO25) to output
  pinMode(yelLED2, OUTPUT);  // initialize yelLED2(GPIO26) to output
  pinMode(grnLED2, OUTPUT);  // initialize grnLED2(GPIO27) to output
  pinMode(buzzer, OUTPUT);   // initialize buzzer(GPIO32)  to output
 

  LCD.init();                     //  initialize LCD
  LCD.backlight();                //  backlight on
  LCD.setCursor(0, 0);            //  set cursor to col1, row1
  LCD.print("    @tranese    ");  // print string to LCD
}

void WiFiConnect() {
  WiFi.mode(WIFI_STA);                     // Wi-Fi mode = STA
  WiFi.begin(ssid, password);              // connect to Wi-Fi
  while (WiFi.status() != WL_CONNECTED) {  //  while Wi-Fi connects
    delay(500);                            // .5s delay
    Serial.println("Wi-Fi connecting, please wait...");
  }
}

void mqttReconnect() {           // if MQTT loses connection, call(function)
  while (!client.connected()) {  // while reconnecting
    Serial.println("Establishing MQTT server connection.");
    long r = random(1000);                 // generate long int
    sprintf(clientId, "clientId-%ld", r);  // print clientId

    if (client.connect(clientId)) {  // if reconnection is successful, print to serial monitor
      Serial.print(clientId);
      Serial.println(" was successfully reconnected.");
      client.subscribe("LED");  // subscribe to LED topic
    } else {                    // if reconnection fails, print to serial monitor
      Serial.print("Reconnection failed., rc=");
      Serial.print(client.state());  // print clientId to serial monitor
      Serial.println(".  Check settings or try again in 5s.");
      delay(5000);  // 5s
    }
  }
}

// MQTT message handling callback(function)
void callback(char* topic, byte* message, unsigned int length) {
  String stMessage;                   // create C string object
  for (int i = 0; i < length; i++) {  // loop: process all MQTT message characters
    stMessage += (char)message[i];    // add each char to C string
  }
  if (String(topic) == "LED") {       // if string topic is LED
    if (stMessage == "ON") {          // and user is subscribed
      iotControl = 1;                 // turn iotControl on
    } else if (stMessage == "OFF") {  // if user not subscribed
      iotControl = 0;                 //  turn iotControl off
    }
  }
}
void buttonPressed() {
  static unsigned long lastInterruptTime = 0;  // variable: stores interruptTime
  unsigned long interruptTime = millis();      // current time in milliseconds

  if (interruptTime - lastInterruptTime > 20) {
    xwButtonState = 0;
  }
  lastInterruptTime = interruptTime;
}
void loop() {
  if (!client.connected()) {  // if MQTT disconnects
    mqttReconnect();          // attempt reconnect function
  }
  while (iotControl == 0) {  //  while iotControl off = normal traffic
    LCD.setCursor(0, 1);
    LCD.print(" Normal Traffic ");
    Serial.println(" Traffic flowing normally.");
    delay(1000);   // 1s delay

    digitalWrite(blueLED, LOW);  // blueLED off

    // xwButton - xwButtonState = digitalRead(xwButton);
    if (xwButtonState == 0) {      //  if crosswalk button is pressed
      digitalWrite(yelLED1, LOW);  // Main yellow LED off
      digitalWrite(grnLED1, LOW);  // Main green LED off
      digitalWrite(yelLED2, LOW);  // 2nd yellow LED off
      digitalWrite(grnLED2, LOW);  // 2nd green LED off

      LCD.setCursor(0, 1);
      LCD.print("  -CROSS  NOW-  ");

      secsLeft = 10;  // set countdown timer to 10s

      while (secsLeft > 0) {
        if (secsLeft >= 10) {                              // if less than 10s remain
          Serial.print("-= Pedestrians traffic detected. =-   ");  // print string to serial monitor
          Serial.println(secsLeft);                        //  print remaining seconds to serial monitor

          LCD.setCursor(14, 1);  // set cursor to col14, row2
          LCD.print(secsLeft);   // print remaining seconds to LCD
        } 
        else {
          Serial.print("-= Pedestrians crossing. =-   ");  // print string to serial monitor
          Serial.println(secsLeft);                               //  print remaining seconds to serial monitor
          LCD.setCursor(0, 1);
          LCD.print("Crossing =   ");

          LCD.setCursor(14, 1);  // set cursor to col15, row2
          LCD.print("  ");       // clear last 2 rows
          LCD.setCursor(15, 1);  // set cursor to col16, row2
          LCD.print(secsLeft);   // print single digit countdown to col16
        }
        digitalWrite(redLED1, HIGH);  // Main red LED on
        digitalWrite(redLED2, HIGH);  // 2nd red LED on
        delay(500);                   // .5s delay (flash on)

        digitalWrite(redLED1, LOW);  // Main red LED off
        digitalWrite(redLED2, LOW);  // 2nd red LED off
        delay(500);                  // .5s delay (flash off)
        secsLeft -= 1;               // countdown remaining seconds
        
        client.loop();
        if (iotControl == 1) {  //  if iotControl on
          break;                // break while loop
        }
      }  
      xwButtonState = 1;
    } 
    else {                        // if iotControl off
      // Main light red
      digitalWrite(redLED1, HIGH);  // Main red LED on
      digitalWrite(yelLED1, LOW);   // Main yellow LED off
      digitalWrite(grnLED1, LOW);   // Main green LED off

      // 2nd light red
      digitalWrite(redLED2, HIGH);  // 2nd red LED on
      digitalWrite(yelLED2, LOW);   // 2nd yellow LED off
      digitalWrite(grnLED2, LOW);   // 2nd green LED off
      delay(1000);                  // 1s delay for transition

      client.loop();  //  check publishing
      if (iotControl == 1) {
        break;
      }
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

      client.loop();  //  check publishing
      if (iotControl == 1) {
        break;
      }
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
      client.loop();  //  check publishing
      if (iotControl == 1) {
        break;
      }
    }
  }  
  while (iotControl == 1) {
    LCD.setCursor(0, 1);
    LCD.println("!! EMERGENCY !! ");
    Serial.println(" !!! Emergency vehicle detected. !!!");

    digitalWrite(yelLED1, LOW);  //  yelLED1 off
    digitalWrite(grnLED1, LOW);  //  grnLED1 off
    digitalWrite(yelLED2, LOW);  //  yelLED2 off
    digitalWrite(grnLED2, LOW);  //  grnLED2 off

    digitalWrite(redLED1, HIGH);  //  redLED1 on
    digitalWrite(redLED2, HIGH);  //  redLED2 on

    digitalWrite(blueLED, HIGH);  // blueLED on
    digitalWrite(buzzer, HIGH);   // active buzzer on (beep)
    delay(500);                   // .5s delay (flash on)

    digitalWrite(blueLED, LOW);  // blueLED off
    digitalWrite(buzzer, LOW);   // active buzzer off
    delay(500);                  // .5s delay (flash off)

    client.loop();  //  check: new MQTT message published
    if (iotControl == 0) {
      break;
    }
  }
    client.loop();  //  check: new MQTT message published
}
