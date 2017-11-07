#include <SoftwareSerial.h>
#include <Wire.h>

#define LiR 12
#define RiR 13

int E1 = 9; 
int E2 = 10;     
int M1 = 8;     
int M2 = 11; 

int select = 2;
int c;

SoftwareSerial bt(0, 1);

void stop(void){
  Serial.println("Stop");
  digitalWrite(E1,0); 
  digitalWrite(M1,LOW);    
  digitalWrite(E2,0);   
  digitalWrite(M2,LOW);
  }

void advance(char a,char b){
  Serial.println("Advance");
  digitalWrite(E1,a); 
  digitalWrite(M1,LOW);    
  digitalWrite(E2,b);   
  digitalWrite(M2,LOW);
  }

 void left(char a,char b){
  Serial.println("Left");
  digitalWrite(E1,a); 
  digitalWrite(M1,HIGH);    
  digitalWrite(E2,b);   
  digitalWrite(M2,LOW);
  }

void right(char a,char b){
  Serial.println("right");
  digitalWrite(E1,a); 
  digitalWrite(M1,LOW);    
  digitalWrite(E2,b);   
  digitalWrite(M2,HIGH);
  }
  
void setup() {
  int z;
  for(z=8;z<=11;z++)
    pinMode(z, OUTPUT);

 Wire.begin(16); 
 Wire.onReceive(receiveEvent);
 Serial.begin(9600);
 bt.begin(9600);
 
}

void receiveEvent(int bytes) {
    c = Wire.read();    
  }


void loop() {
  char x;
  if(bt.available() > 0){
    x=bt.read();
    bt.println(x); 
  }
      switch(x){
        case 'Q':
              stop();          
              select = 1;
          break;
        case 'E':   
              stop();      
              select = 2;
          break;
      }
  if(select == 1){
       if(digitalRead(LiR) && digitalRead(RiR)){
        advance (100, 100);
       }
       else if ((!digitalRead(LiR)) && digitalRead(RiR)){
        left (100,100);
       }
       else if (digitalRead(LiR) && (!digitalRead(RiR))){
        right (100,100);
       }
       else if ((!digitalRead(LiR)) && (!digitalRead(RiR))){
        stop();
       }  
       if (c == 1){
          Serial.println(c);
          stop();
          delay(35000);
          c = 0;
       }
  }

  else if (select == 2) {
        if (x == 'W'){   
          advance (50, 50);
        }

        else if (x == 'A'){          
          left (100,100);
        }

        else if (x == 'D'){          
          right (100,100);
        }

        else if (x == 'X'){          
          stop();
        }
  }
  delay(1);
}
