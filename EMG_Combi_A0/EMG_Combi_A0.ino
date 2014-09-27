#define NUM_LED 6            // Sets the maximum numbers of LEDs
#define MAX 18               // Maximum posible reading. TWEAK THIS VALUE!!
#define TRIGGER_THRESHOLD 10 // The value the analog input must exceed in order to trigger the servo
#define SERVO_DIGITAL_PIN 3  // Arduino Uno PWM pins are 3, 5, 6, 9, 10, and 11; pin 10 is in use by LED
#define KEY_DELAY 200        // How long in ms to wait before a new keypress can be sent

#include <Servo.h>

typedef enum {
  MONITOR,
  SERVO,
  KEYBOARD
} mode;

String incomingString = "";
mode myMode = SERVO;
Servo myServo;
int finalReading = 0;
int litLeds = 0;
int multiplier = 1;
int leds[] = {8, 9, 10, 11, 12, 13};

void setup() {
  // Initialise servo pin
  myServo.attach(SERVO_DIGITAL_PIN);
  
  // Begin serial communications
  Serial.begin(9600);
  
  // Initialize LED pins as outputs
  for (int i = 0; i < NUM_LED; i++){ 
    pinMode(leds[i], OUTPUT);
  }
}

void loop () {
  // Check for commands from serial terminal
  while (Serial.available() > 0) {
    char incomingChar = Serial.read();
    if (incomingChar == '\r') {
      // User typed 'mon'
      if (incomingString == "mon") {
          // Enable monitor mode
          myMode = MONITOR;
          
          // Tell user and pause
          Serial.println("Switched to monitor mode.");
          delay(1000);
      } else if (incomingString == "key") {
          // Enable keyboard mode
          myMode = KEYBOARD;
          
          // Tell user
          Serial.println("Switched to game mode.");
      } else if (incomingString == "servo") {
          // Enable servo mode
          myMode = SERVO;
          
          // Tell user
          Serial.println("Switched to servo mode.");
      }
      incomingString = "";
    } else incomingString += incomingChar;
  }
  
  // Clear last final reading
  finalReading = 0;
  
  // Take ten readings in ~0.02 seconds
  for (int i = 0; i < 10; i++) {
    finalReading += analogRead(A0) * multiplier;
    delay(2);
  }
  
  // Average the ten readings
  finalReading /= 10;
  
  // Dump values to serial console if in monitor mode
  if (myMode == MONITOR) {
    Serial.print("finalReading:\t");
    Serial.print(finalReading);
    Serial.print("\t");
    Serial.print("LEDs:\t");
    Serial.println(litLeds);
  }
  
  // Send button press if in game mode and the averaged reading exceeds the threshold
  else if (myMode == KEYBOARD && finalReading > TRIGGER_THRESHOLD) {
    Serial.print("Button press triggered! (value: ");
    Serial.print(finalReading);
    Serial.println(")");
    
    // Turn on LED
    digitalWrite(13, HIGH);
    
    // Space bar down
    Keyboard.set_key1(KEY_SPACE);
    Keyboard.send_now();
    
    // Space bar up
    Keyboard.set_key1(0);
    Keyboard.send_now();
    
    delay(KEY_DELAY);
    
    // Turn off LED
    digitalWrite(13, LOW);
  }
  
  // Move the servo back and forth if in servo mode and the averaged reading exceeds the threshold
  else if (myMode == SERVO && finalReading > TRIGGER_THRESHOLD) {
    Serial.print("Servo triggered! (value: ");
    Serial.print(finalReading);
    Serial.println(")");
    myServo.write(180);
    delay(1000);
    myServo.write(0);
  }
  
  // Turn off all LEDs
  for (int j = 0; j < NUM_LED; j++) {
    digitalWrite(leds[j], LOW);
  }
  
  // Constrain range and map value to number of LEDs
  finalReading = constrain(finalReading, 0, MAX);
  litLeds = map(finalReading, 0, MAX, 0, NUM_LED);
  
  // Turn on LEDs accordingly
  for (int k = 0; k < litLeds; k++) {
    digitalWrite(leds[k], HIGH);
  }
}
