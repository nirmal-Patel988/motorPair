#include<motorPair.h>

motorPair::motorPair(){}
motorPair::motorPair(Motor * mtr1 ,Motor * mtr2){
    motorPair:: m1 = mtr1;
    motorPair:: m2 = mtr2;
    myPID->SetMode(AUTOMATIC);
    myPID->SetSampleTime(1);
    myPID->SetOutputLimits(-255,255);
}

motorPair::getReadings(){
    return (abs(motorPair:: m1->getReadings())+abs(motorPair:: m2->getReadings()))/2;
}

// motorpair::setReadings(long readings){
//     motorPair::m1->setReadings(readings);
//     motorPair::m2->setReadings(readings);
// }

motorpair::reset(){
    #if defined(AVR)
        encoder->write(0);
    #elif defined(ESP_32)
        encoder->setCount(0);
    #endif
}

motorpair::setTunnings(double kp , double ki ,double kd){
    motorpair::Kp=kp;
    motorpair::Ki=ki;
    motorpair::kd=kd;
    myPID->SetTunings(motorpair::Kp,motorpair::Ki,motorpair::Kd);
}

motorPair::setPWM(int p){
    motorPair::PWM = p;
    // motorPair::m1->write(PWM);
    // motorPair::m2->write(PWM);
}

motorPair::enableBrake(bool b=true){
    motorPair::brake = b;
}

motorPair::disableBrake(bool b=true){   
    motorPair::brake = !b;
}

motorPair::compute(){
    long value1, value2;
    value1 = motorPair::m1->getReadings();
    value2 = (motorPair::brake) ? 0 : (motorPair::m2->getReadings());
    motorPair::Input = abs(abs(value1)-abs(value2))*(-1);

    if(motorPair::brake){
        if(motorPair::Input<170){
            motorPair::stable = false;
        }
        else{
            motorPair::stable = true;
        }
        if(value1 > 0){
            motorPair::setPWM(PWM*(-1));//backward
        }
        else if(value1 < 0){
            motorPair::setPWM(PWM);      //forward
        }
    }
    else{        
        myPID->SetTunings(motorPair::Kp,motorPair::Ki,motorPair::Kd);
    }
    
    myPID->Compute();
    if(abs(value1)>=abs(value2) && !brake){              
        if(motorPair::brake){    
            if(!motorPair::stable)
                motorPair::m1->setPWM((motorPair::PWM-motorPair::Output)<0 ? 0:(motorPair::PWM-motorPair::Output));
            else
                motorPair::m1->setPWM(1);
        }
        else if(!motorPair::brake){
            motorPair::m1->setPWM((motorPair::PWM-motorPair::Output)<0 ? 0:(motorPair::PWM-motorPair::Output));
            motorPair::m2->setPWM((motorPair::PWM+motorPair::Output)>255?255:(motorPair::PWM+motorPair::Output));      
        }
    }
    else{
        if(motorPair::brake){    
            if(!motorPair::stable)    
                motorPair::m1->setPWM((motorPair::PWM+motorPair::Output)>255 ? 255:(motorPair::PWM+motorPair::Output));
            else
                motorPair::m1->setPWM(1);
        }
        else
            motorPair::m1->setPWM((motorPair::PWM+motorPair::Output)>255 ? 255:(motorPair::PWM+motorPair::Output));
        if(!brake)
            motorPair::m2->setPWM((motorPair::PWM-motorPair::Output)<0 ? 0:(motorPair::PWM-motorPair::Output));
        }

    if(value1 > 100000 || value2>100000){
        motorPair::m1->reset();
        motorPair::m2->reset();
    }
}