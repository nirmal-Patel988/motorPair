#ifndef motorPair_h
#define motorPair_h
#include<UniversalEncoder.h>
#include<UniversalPWM.h>
#include<motor.h>

class motorPair{
    public:
    
    void getReadings();
    void SetReadings(long readings);
    void reset();
    void setTunnings();
    void setPWM(int pwm);
    void compute();
    void brakeEnable();
    void brakeDisable(bool brake);
};

#endif