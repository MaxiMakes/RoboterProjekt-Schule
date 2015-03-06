#ifndef Motor_H 
#define Motor_H

class Motor{
    public:
        //Setup Pins	    
        void setMotorPin(short m);
        //Get Pins
        short getMotorPin();
        void setSpeed(short v);
	void setZero(short duration);
	void setSpeedFor(short v, short duration);
        //constructor
        Motor(short Motor);
    private:
        short MPin;
        
};
#endif
