#ifndef __STOP_LIGHT_H__
#define __STOP_LIGHT_H__

#include <string>
using namespace std;

#include "VehicleBase.h"

//StopLight class controls road traffic.
class StopLight{
private:
    int redTime;            // duration of the red light
    int greenTime;          // suration of the green light
    int yellowTime;         // duration of the yellow light
    int timeLeft;           // the remaining time for the current light
    LightColor color;       // the color of the light

public:

    // Default constructor
    StopLight();
    // Constructor with all parameters: greenLight, yellowLight, rightLight(duration of the **corresponding color** light), color of the stopLight
    StopLight(int greenLight, int yellowLight, int redLight, LightColor curColor);
    // Destructor
    ~StopLight(){}

    void decrement();       // decrements by 1 the time left on the stopLight
    LightColor getColor();  // returns the color of the light
    int getTimeLeft();      // returns the amount of time left until the light changes
};

#endif
