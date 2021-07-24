#include<motorPair.h>

motorPair::motorPair(){}
motorPair::motorPair(Motor * mtr1 ,Motor * mtr2){
    motorPair:: m1 = mtr1;
    motorPair:: m2 = mtr2;
    myPID->SetMode(AUTOMATIC);
    myPID->SetSampleTime(1);
    myPID->SetOutputLimits(-150,150);
}

long motorPair::getReadings(){
    return (motorPair::m1->getReadings() + motorPair::m2->getReadings())/2;
}

void motorPair::setReadings(long readings){
    motorPair::m1->setReadings(readings);
    motorPair::m2->setReadings(readings);
}
void motorPair::setSoftTunings(double Kp,double Kd,double Ki){
    motorPair::setTunings(motorPair::Kp,motorPair::Ki,motorPair::Kd);
}

void motorPair::setAggTunings(double Kp,double Kd,double Ki){
    motorPair::setTunings(motorPair::Kp,motorPair::Ki,motorPair::Kd);
}
void motorPair::reset(){
    motorPair::m1->reset();
    motorPair::m2->reset();
}

void motorPair::setTunings(double kp , double ki ,double kd){
    motorPair::Kp=kp;
    motorPair::Ki=ki;
    motorPair::Kd=kd;
    myPID->SetTunings(motorPair::Kp,motorPair::Ki,motorPair::Kd);
}

void motorPair::setPWM(int p){
    motorPair::PWM = p;
}

void motorPair::setThreshold(int threshold=400){
    motorPair::Threshold = threshold;
}

void motorPair::enableBrake(bool b){
    motorPair::brake = b;
    stable=true;
}

void motorPair::disableBrake(bool b){   
    motorPair::brake = !b;
}

void motorPair::compute(){
    if(motorPair::enable){
        if(!motorPair::brake){
            motorPair::Input = motorPair::m1->getReadings() - motorPair::m2->getReadings();
            if(abs(Input)<motorPair::Threshold){
                this->setSoftTunings(double Kp,double Kd,double Ki)
            }
            else{
                this->setAggTunings(double Kp,double Kd,double Ki)
            }
            
            motorPair::myPID->Compute();
            motorPair::m1->setPWM(motorPair::PWM + motorPair::Output);
            motorPair::m2->setPWM(motorPair::PWM - motorPair::Output);
        }
        else{
            if (stable){
            Input = motorPair::m1->getReadings();
            motorPair::myPID->Compute();
            motorPair::m1->setPWM(motorPair::PWM + motorPair::Output);
            }
        }
    }
}