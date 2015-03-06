#include <UltraSonic.h>
#include <Arduino.h> 

UltraSonic::UltraSonic(short echo, short trig){

    echoPin = echo;
    trigPin = trig;
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
}

int UltraSonic::getDistance(){     
       
	long duration, cm;
       
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);    //pulse
    delayMicroseconds(5);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);

    cm = microsecondsToCentimeters(duration);
    return cm;

    delay(50);
}


void UltraSonic::setEchoPin(short echo){
    echoPin = echo;
    pinMode(echoPin, INPUT);
}
void UltraSonic::setTriggerPin(short trig){
    trigPin = trig;
    pinMode(trigPin, OUTPUT);
}    

long UltraSonic::microsecondsToCentimeters(long microseconds){
    return microseconds / 29 / 2;
}
