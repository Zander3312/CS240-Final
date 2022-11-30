#ifndef __VEHICLE_BASE_CPP__
#define __VEHICLE_BASE_CPP__

#include "VehicleBase.h"

using namespace std;

int VehicleBase::vehicleCount = 0;

VehicleBase::VehicleBase(VehicleType type, Direction direction, bool toRight)
    : vehicleID(VehicleBase::vehicleCount++),
      vehicleType(type),
      vehicleDirection(direction),
      turnRight(toRight)
{
    if (type == VehicleType::car){
        length = 2;
        tail = 2;
        head = 3;
    }
    else if (type == VehicleType::suv){
        length = 3;
        tail = 1;
        head = 3;
    }
    else{
        length = 4;
        tail = 0;
        head = 3;
    }
}

VehicleBase::VehicleBase(const VehicleBase& other)
    : vehicleID(other.vehicleID),
      vehicleType(other.vehicleType),
      vehicleDirection(other.vehicleDirection),
      turnRight(other.turnRight)
{}



VehicleBase::~VehicleBase() {}

// void VehicleBase::moveForward(vector<VehicleBase*> &road, StopLight &light){
//     int stopLightPoint = road.size() / 2 - 2;
//     LightColor currentLight = light.getColor();
//     if (currentLight == LightColor::red){ //do nothing
//         return;
//     }
//     if (road[head + 1] == nullptr){                                                  //additional if?
//         if (currentLight == LightColor::green){
//             road[head + 1] = this;
//             head += 1;
//             road[tail] = nullptr;
//             tail += 1;
//         }
//         if (currentLight == LightColor::yellow){
//             if ((head != stopLightPoint) || (light.getTimeLeft() > length)){            //or >=?
//                 road[head + 1] = this;
//                 head += 1;
//                 road[tail] = nullptr;
//                 tail += 1;
//             }
//         }
//     }
// }

/*
void VehicleBase::moveRight(vector<VehicleBase*> &road1,
  vector<VehicleBase*> &road2){
    if (!turnRight){
        return;
    }
    int turningPoint = road.size() / 2 - 1;
    int newPoint = road.size() / 2 + 1;

}
*/
#endif
