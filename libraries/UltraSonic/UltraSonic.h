#ifndef UltraSonic_H 
#define UltraSonic_H
//(short echo, short trig)
class UltraSonic{
    public:
        //Setup Pins	    
        void setEchoPin(short echo);
        void setTriggerPin(short trig);  
        //Get Pins
        short getEchoPin();
        short getTriggerPin();        
        int getDistance();
        //constructor
        UltraSonic(short echo, short trig);
    private:
        short echoPin;
        short trigPin;
        long microsecondsToCentimeters(long);
};
#endif
