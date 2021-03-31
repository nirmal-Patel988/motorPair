#include<motorPair.h>

motorPair::motorPair(){}
motorPair::motorPair(Motor * mtr1 ,Motor * mtr2){
    motorPair:: m1 = mtr1;
    motorPair:: m2 = mtr2;
    myPID->SetMode(AUTOMATIC);
    myPID->SetSampleTime(1);
    myPID->SetOutputLimits(-255,255);
}

long motorPair::getReadings(){
    return (motorPair::m1->getReadings() + motorPair::m2->getReadings())/2;
}

void motorPair::setReadings(long readings){
    motorPair::m1->setReadings(readings);
    motorPair::m2->setReadings(readings);
}

void motorPair::reset(){
    motorPair::m1->reset();
    motorPair::m2->reset();
}

void motorPair::setTunnings(double kp , double ki ,double kd){
    motorPair::Kp=kp;
    motorPair::Ki=ki;
    motorPair::Kd=kd;
    myPID->SetTunings(motorPair::Kp,motorPair::Ki,motorPair::Kd);
}

void motorPair::setPWM(int p){
    motorPair::PWM = p;
}

void motorPair::enableBrake(bool b){
    motorPair::brake = b;
}

void motorPair::disableBrake(bool b){   
    motorPair::brake = !b;
}

void motorPair::compute(){
    if(motorPair::enable){
        if(!motorPair::brake){
            motorPair::Input = motorPair::m1->getReadings() - motorPair::m2->getReadings();
            motorPair::myPID->Compute();
            motorPair::m1->setPWM(motorPair::PWM + motorPair::Output);
            motorPair::m2->setPWM(motorPair::PWM - motorPair::Output);
        }
        else{
            Input = motorPair::m1->getReadings();
            motorPair::myPID->Compute();
            motorPair::m1->setPWM(motorPair::PWM + motorPair::Output);
        }
        Serial.println(String(Input)+","+String(Output));
    }
}