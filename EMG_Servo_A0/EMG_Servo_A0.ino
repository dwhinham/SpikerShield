#define NUM_LED 6            // Sets the maximum numbers of LEDs
#define MAX 18               // Maximum posible reading. TWEAK THIS VALUE!!
#define TRIGGER_THRESHOLD 10 // The value the analog input must exceed in order to trigger the servo
#define SERVO_DIGITAL_PIN 3  // Arduino Uno PWM pins are 3, 5, 6, 9, 10, and 11; pin 10 is in use by LED

#include <Servo.h>

String incomingString = "";
boolean monitorMode = false;
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

void loop() {
  // Check for commands from serial terminal
  while (Serial.available() > 0) {
    char incomingChar = Serial.read();
    if (incomingChar == '\r') {
      // User typed 'mon'
      if (incomingString == "mon") {
          // Toggle monitor mode
          monitorMode = !monitorMode;
          
          // Tell user whether enabled or disabled
          String message = "Monitor mode ";
          message += monitorMode ? "enabled." : "disabled.";
          Serial.println(message);
          
          // Pause if entering monitor mode
          if (monitorMode) {
            delay(1000);
          }
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
  
  // Move the servo back and forth if not in monitor mode and the averaged reading exceeds the threshold
  if (!monitorMode && finalReading > TRIGGER_THRESHOLD) {
    Serial.println("Servo triggered!");
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
  
  // Dump values to serial console if in monitor mode
  if (monitorMode) {
    Serial.print("finalReading:\t");
    Serial.print(finalReading);
    Serial.print("\t");
    Serial.print("LEDs:\t");
    Serial.println(litLeds);
  }
}
