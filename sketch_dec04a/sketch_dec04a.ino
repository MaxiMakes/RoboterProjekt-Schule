#include <Motor.h>
#include <UltraSonic.h>

//Pinmapping

//TODO
const static byte trigL = 8;
const static byte echoL = 7;
const static byte trigR = 8;
const static byte echoR = 7;

const static byte irR = A1;
const static byte irL = A2;
const static byte irM = A3;

const static byte tfPin = 2;
const static byte trPin = 3;
const static byte tlPin = 4;

const static byte motorR = 10;
const static byte motorL = 9;
const static byte motorRRich = 6;
const static byte motorLRich = 5;

const static byte ledPin = 13;
//Constance
const boolean dbug = true; //Set to false to deactivate [DEBUG] messages

int lmwL = 100;     //light-medium 
int lmwR = 100;
int lmwM = 100;
const int v = 50;   //velocity 
short r180 = 0;     //rotate 180° 
short r90 = r180/2; //rotate 90°

//creating sensor-objects and setting their pins 
UltraSonic sol(echoL, trigL);
UltraSonic sor(echoR, trigR);

//creating motor-object;
Motor ml(motorL);
Motor mr(motorR);


void setup(){
  pinMode(ledPin, OUTPUT);
  Serial.begin(4800);
  calib();
}

void calib(){
  debug("[DEBUG]: start calib()");
  calibLight();
  debug("[DEBUG]: finished calibLight()");
  r180 = calibRotation();
  debug("[DEBUG]: finished calibRotation() the time to rotate 180 degrees is:" + r180);
  r90 = 180 /2;
}
int calibRotation(){
  long duration;
  drive(v,-v);
  while(true){
    if(ms()){
      duration = millis();
      while(ms());      
      while(!ms());
      duration = millis() - duration;
      blinken(4, 80);
      break;
    }
  }
  long dt = millis();
  drive(-v,v);
  while(duration < (millis() - dt));
  return duration;
}

void calibLight(){
  short lastLightLevel[3];
  short minLightLevel[3];
  short maxLightLevel[3];
  long time = 0;
  short del = 10;
  while(true){
    drive(-50,50);
      
    lastLightLevel[0] = analogRead(irL);
    lastLightLevel[1] = analogRead(irM);
    lastLightLevel[2] = analogRead(irR);
   
    // when minLightLevel[0] = null or less than lastLightLevel[0] asign lastLevel to minLevel
    if(!minLightLevel[0] || minLightLevel[0] > lastLightLevel[0]) minLightLevel[0] = lastLightLevel[0];
    else if(!maxLightLevel[0] || maxLightLevel[0] < lastLightLevel[0]) maxLightLevel[0] = lastLightLevel[0];
   
    if(!minLightLevel[1] || minLightLevel[1] > lastLightLevel[1]) minLightLevel[1] = lastLightLevel[1];
    else if(!maxLightLevel[1] || maxLightLevel[1] < lastLightLevel[1]) maxLightLevel[1] = lastLightLevel[1];
    
    if(!minLightLevel[2] || minLightLevel[2] > lastLightLevel[2]) minLightLevel[2] = lastLightLevel[2];
    else if(!maxLightLevel[2] || maxLightLevel[2] < lastLightLevel[2]) maxLightLevel[2] = lastLightLevel[2];
    //takes time to rotate ~1000ms
    delay(del);
    time += del;
    
    if(time > 1000) break;
  }
  //rotate back
  driveFor(50,-50,1000);
  lmwL = (maxLightLevel[0] - minLightLevel[0]) / 2 + minLightLevel[0];
  lmwM = (maxLightLevel[1] - minLightLevel[1]) / 2 + minLightLevel[1];
  lmwR = (maxLightLevel[2] - minLightLevel[2]) / 2 + minLightLevel[2];

  return;
}


//------------------------------------------------MAIN-------------------------------------------------------------\\

void loop(){
  followLine(); 
  obstacle(); 
}

void followLine(){
  if(!ls() && rs()) drive(v, -v);
  else if(ls() && !rs()) drive(-v, v);
  else if(!ls() && !rs()) drive(v, v);
  else drive(-v, -v); 
  return;
}

void obstacle(){
  if(tof()){
    debug("[DEBUG]: Detected obstacle");
    if(sr() < 20){
      debug("[DEBUG]: drive to the right");
      drive(-50, -50); //drive back
      delay(500);
      
      drive(50, -50);  //rotate right
      delay(r90);
      
      while(true){
        drive(20,50); //drive left turn
        if(tol()) correctL();  //left
        if(rs() || ls()) return;
      }
    }
    else if(sl() < 20){
      debug("[DEBUG]: drive to the left");
      drive(-50, -50); //drive back
      delay(500);
      
      drive(-50, 50);  //rotate left
      delay(r90);
      
      while(true){
        drive(20,50);
        if(tor()) correctR();  //right
        if(rs() || ls()) return;
      }
    }
    else{
      debug("[DEBUG]: turn back");
      drive(50,-50);
      delay(r180);
    }
  }
  else{
     return;   
  }
}

void correctL(){ //rotate to the right side
  drive(50, -50);
  delay(200);
}

void correctR(){ //rotate to the left side
  drive(-50, 50);
  delay(200);
}


//Shortcuts for distancesensors
int sl(){ return sol.getDistance();}
int sr(){ return sol.getDistance();}

//Shortcuts for Motors
void drive(short l, short r){ 
  mr.setSpeed(r);
  ml.setSpeed(l);
}
void driveFor(short l, short r, short d){ 
  ml.setSpeedFor(r, d);
  mr.setSpeedFor(l, d);
}

//if(debug)Serial.println("");
void debug(String msg){
  if(dbug)Serial.println(msg);
}
void debug2(String msg){
  if(dbug)Serial.print(msg);
}

//i = amount and dt = interval 
void blinken(short i, short dt){
  for(short v = 0; v >= i; v++){
    digitalWrite(ledPin, HIGH);
    delay(dt/2);
    digitalWrite(ledPin, LOW);
    delay(dt/2);
  }
}
//Check if a sensor sees something black
bool ls(){
  if(analogRead(irL) < lmwL) return true;
  else return false;
}
bool rs(){
  if(analogRead(irR) < lmwR) return true;
  else return false;
}
bool ms(){
  if(analogRead(irM) < lmwM) return true;
  else return false;
}
//Checks the touch sensors
bool tof(){
  if(digitalRead(tfPin) == HIGH)return true;
  else return false;
}
bool tor(){
  if(digitalRead(trPin) == HIGH)return true;
  else return false;
}
bool tol(){
  if(digitalRead(tlPin) == HIGH)return true;
  else return false;
}
void allSensorValues(){
 Serial.println("-------Light Sensors-------");
 Serial.println("Left------Middle------Right");
 Serial.println(analogRead(irL) + "--------------" + analogRead(irR));
 Serial.println(ls()+"-----------------"+rs());
 Serial.println("----Ultra Sonic Sensors----");
 Serial.println("----Left----------Right----");
 Serial.print("----");
 Serial.print(sl());
 Serial.print("------"); 
 Serial.print(sr()); 
 Serial.println("----");
 Serial.println("-------Touch Sensors-------");
 Serial.println("Left------Middle------Right");
 Serial.print(tol());
 Serial.print("------");
 Serial.print(tof());
 Serial.print("------");
 Serial.println(tor());
}
