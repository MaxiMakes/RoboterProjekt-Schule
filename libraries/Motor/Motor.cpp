#include <Motor.h>
#include <Arduino.h> 

Motor::Motor(short Motor){

    MPin = Motor;
    pinMode(MPin, OUTPUT);
    
}

void Motor::setSpeed(short v){
  
  /*l = l *(-1);
  if(l > 0){
    l = map(l, 1, 100, 129, 254);
  }
  else if(l < 0){
    l = map(l, -100, -1, 1, 127);
  }
  else l = 128;  
  */
	//constain to -100 ~ 100 
	v=constrain(v,-100,100);
	//convert the logic-level from -100 ~ 100 to 0 ~ 255
  if(v > 0){
    v = map(v, 1, 100, 129, 254);
  }
  else if(v < 0){
    v = map(v, -100, -1, 1, 127);
  }
  else v = 128;  
  //write to PWM-Pin
  analogWrite(MPin, v);  
}

void Motor::setSpeedFor(short m, short duration){
    Motor::setSpeed(m);
    delay(duration);
    Motor::setSpeed(0);
}

void Motor::setZero(short duration){
  analogWrite(MPin, 128);
  delay(duration); 

}

void Motor::setMotorPin(short m){
    MPin = m;
    pinMode(MPin, OUTPUT);
}

short Motor::getMotorPin(){
    return MPin;
}

