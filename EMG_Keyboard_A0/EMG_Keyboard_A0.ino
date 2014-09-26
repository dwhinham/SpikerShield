#define TRIGGER_THRESHOLD 10 // The value the analog input must exceed in order to trigger the button press
#define KEY_DELAY 200         // How long in ms to wait before a new keypress can be sent

String incomingString = "";
boolean monitorMode = false;

int finalReading = 0;
int multiplier = 1;

void setup() {
  // Begin serial communications
  Serial.begin(9600);
}

void loop () {
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
  
  // Log value if debug mode enabled
  if (monitorMode) {
    Serial.print("Value: ");
    Serial.println(finalReading);
  }
  
  // Send button press if not in monitor mode and the averaged reading exceeds the threshold
  else if (finalReading > TRIGGER_THRESHOLD) {
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
  
  
}
