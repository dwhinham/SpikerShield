#define NUM_LED 6  //sets the maximum numbers of LEDs
#define MAX 18     //maximum posible reading. TWEAK THIS VALUE!!
#include <Servo.h>
Servo myservo;
int reading[10];
int finalReading;
byte litLeds = 0;
byte multiplier = 1;
byte leds[] = {8, 9, 10, 11, 12, 13};


void setup(){
  myservo.attach(10);
  Serial.begin(9600); //begin serial communications
  for(int i = 0; i < NUM_LED; i++){ //initialize LEDs as outputs
    pinMode(leds[i], OUTPUT);
  }
}

void loop(){
  for(int i = 0; i < 10; i++){    //take ten readings in ~0.02 seconds
    reading[i] = analogRead(A0) * multiplier;
    delay(2);
  }
  for(int i = 0; i < 10; i++){   //average the ten readings
    finalReading += reading[i];
  }
  finalReading /= 10;
  if (finalReading > 10) {
    myservo.write(180);
    delay(1000);
    myservo.write(0);
    
  }
  for(int j = 0; j < NUM_LED; j++){  //write all LEDs low
    digitalWrite(leds[j], LOW);
  }
  Serial.print("finalReading: " + finalReading);
  Serial.print("\t");
  finalReading = constrain(finalReading, 0, MAX);
  litLeds = map(finalReading, 0, MAX, 0, NUM_LED);
  Serial.println("LEDs: " + litLeds);
  for(int k = 0; k < litLeds; k++){
    digitalWrite(leds[k], HIGH);
  }
  //for serial debugging, uncomment the next two lines.
  //Serial.println(finalReading);
  //delay(100);
}
    
