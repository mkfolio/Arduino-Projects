#include <Wire.h>
#include <Servo.h>

Servo servoSonic;
Servo servoSwipe;

#define trigPin 13
#define echoPin 12
#define led 11
#define led2 10

int pos = 0;
int x = 1;

long duration;
int distance;

void setup() {
  Wire.begin();
  servoSonic.attach(9);
  servoSwipe.attach(10);
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(led, OUTPUT);
  pinMode(led2, OUTPUT);

}

void loop() {
//~~~ Servo Ultrasonic Start from 0 deg to 180 deg
while (pos = 0; pos <= 180; pos+=1){
    servoSonic.write(pos);
    delay(80);
    int long distance = calculateDistance();
    Serial.println(distance);
      if (distance > 5 && distance <20) {
        servoSonic.write(90);
        //Sending byte to master to stop
        Wire.beginTransmission(16);
        Wire.write(c);              
        Wire.endTransmission();
        //~~~ Servo swipe Start from 180 deg to 0 deg
        for (pos = 0; pos <= 180; pos+=1){
             servoSwipe.write(pos);
              delay(15);
        }
        //~~~ Servo swipe Start from 0 deg to 180 deg
        for (pos = 180; pos >= 0; pos-=1){
             servoSwipe.write(pos);
              delay(15);
        }
        
      }
  }
//~~~ Servo Ultrasonic Start from 180 deg to 0 deg
while (pos = 180; pos >= 0; pos-=1){
    servoSonic.write(pos);
    delay(80);
    int long distance = calculateDistance();
    Serial.println(distance);
     if (distance > 5 && distance <20) {
        servoSonic.write(90);
        //Sending byte to master to stop
        Wire.beginTransmission(16);
        Wire.write(c);              
        Wire.endTransmission();
        //~~~ Servo swipe Start from 180 deg to 0 deg
        for (pos = 0; pos <= 180; pos+=1){
             servoSwipe.write(pos);
             delay(30);
        }
        //~~~ Servo swipe Start from 0 deg to 180 deg
        for (pos = 0; pos >= 180; pos-=1){
             servoSwipe.write(pos);
             delay(30);
        }
        
      }
  }
    
 
}

int calculateDistance(){
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(20);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  return distance;
}

