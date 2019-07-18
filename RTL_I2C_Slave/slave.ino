//Powered by: @CHECHESWAP JESUS JOSE NAVARRETE BACA JUNIO 2019
#include <Arduino.h>
#include "seven_segslcd.h"
#include <Wire.h>

#define PIN_LED 14
#define KEY_LED 17
#define PIN_TRIG  13 
#define PIN_ECHO  12
#define CONST_TIME_1 29.1
#define CONST_TIME_TRANSLATE 2

seven_segslcd * obj_lcd = new seven_segslcd(100);

long double mark = 0;
boolean flag = false;
boolean flag2 = false;
long double before;
long double before2;
bool xtime(long double, long double, int);
boolean w1 = false;
boolean w2 = false;
long xduration = 0;
int xdistance = 0;
boolean flag_key = false;
long double last_incoming=0;

void func();
void _S0();
void _S1();
void _S2();

void setup()
{
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);   
  pinMode(PIN_LED, OUTPUT);
  pinMode(KEY_LED, OUTPUT);
  Wire.begin(8);
  Wire.onReceive(_S1);
}


void loop()
{            
     _S0(); 
     _S2();         
}

void _S2(){
  if(last_incoming > 0){
    if(xtime(last_incoming, millis(), 500)){
      digitalWrite(KEY_LED, LOW);
    }
  }
}
void key_blink(){
  flag_key = true;
  digitalWrite(KEY_LED, flag_key);
  last_incoming = millis();
}

void _S1(){
  if(Wire.available()){
    char tmp = Wire.read();

    if(tmp == '1' || tmp == '0'){
      key_blink();
    }
  }
}

void _S0(){

  if(!w1 && !w2 && !flag){
    w1 ^= 1; 
    flag = !flag;    
    digitalWrite(PIN_TRIG,LOW);
    before = micros();
  }
  else if(!w1 && !w2){
    w1 ^= 1;    
    digitalWrite(PIN_TRIG,LOW);
    before = micros();
  }

  if(w1 && !w2 && xtime(before, micros(), 3)){
    w1 ^= 1; 
  }
  
  if(!w1 && !w2 && !flag2){
    flag2 = !flag2;
    w2 ^= 1; 
    digitalWrite(PIN_TRIG,HIGH);
    before2= micros();
  }
  else if(!w1 && !w2){
    w2 ^= 1;
    digitalWrite(PIN_TRIG,HIGH);
    before2= micros();    
  }

  if(w2 && !w1 && xtime(before2, micros(), 20)){
    w2 ^= 1; 
    
    xduration = pulseIn(PIN_ECHO, HIGH);
    xdistance = xduration /CONST_TIME_1/CONST_TIME_TRANSLATE;   
         
    if((xdistance >= 0) && (xdistance <20)){      
      obj_lcd->print_digit(1,0);     
      digitalWrite(PIN_LED,HIGH); 
    }
    else{
      obj_lcd->print_digit(0,0);
      digitalWrite(PIN_LED,LOW); 
    }
  }
}
bool xtime(long double bef, long double now, int range){
  if(now - bef >= range){
    return true;
  }
  return false;
}

