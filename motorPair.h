#ifndef motorPair_h
#define motorPair_h

#include <motor.h>
#include <PID_v1.h>

class motorPair{
    public:
    
    bool brake = false;
    bool enable = false;
    bool stable = false;
    
    PID *myPID =  new PID(&Input,&Output,&Setpoint,Kp,Ki,Kd,DIRECT);
    double Kp=0, Ki=0, Kd = 0;
    double Setpoint = 0,Output = 0,Input = 0;
    Motor *m1,*m2;
    int PWM = 0;
    

    void getReadings();
    // void SetReadings(long readings);
    void reset();
    void setTunnings(double Kp , double Ki ,double Kd);
    void setPWM(int pwm);
    void compute();
    void brakeEnable(bool brake);
    void brakeDisable(bool brake);
};

#endif