#ifndef __STOP_LIGHT_CPP__
#define __STOP_LIGHT_CPP__

#include "StopLight.h"
#include <string>

using namespace std;

//Default constructor
StopLight::StopLight(){
    redTime = 0;
    greenTime = 0;
    yellowTime = 0;
    timeLeft = 0;
    color = LightColor::red;
}


StopLight::StopLight(int greenLight, int yellowLight, int redLight, LightColor curColor){

    // Member initilization
    redTime = redLight;
    greenTime = greenLight;
    yellowTime = yellowLight;
    this->color = curColor;
    if(color == LightColor::green)
        timeLeft = greenTime;
    else if(color == LightColor::yellow)
        timeLeft = yellowTime;
    else
        timeLeft = redTime;
}

//Decrement time on StopLight
void StopLight::decrement(){
    timeLeft--;
    if(timeLeft == 0){                     //changing color when needed
        if (color == LightColor::red){
            color = LightColor::green;
            timeLeft = greenTime;
        }
        else if (color == LightColor::yellow){
            color = LightColor::red;
            timeLeft = redTime;
        }
        else{
            color = LightColor::yellow;
            timeLeft = yellowTime;
        }
    }
}

//Getting current color
LightColor StopLight::getColor(){
    return color;
}

//Getting remaining time
int StopLight::getTimeLeft(){
    return timeLeft;
}

#endif
