#ifndef __VEHICLE_BASE_H__
#define __VEHICLE_BASE_H__

// enum: see http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-enum
enum class Direction   {north, south, east, west};
enum class VehicleType {car, suv, truck};
enum class LightColor  {green, yellow, red};

#include <vector>
#include "StopLight.h"

using namespace std;

class VehicleBase
{
   public:
      static int vehicleCount;

   private:
      int         vehicleID;
      VehicleType vehicleType;
      Direction   vehicleDirection;
      int length;
      bool turnRight;
      int head;
      int tail;

   public:
      VehicleBase(VehicleType type, Direction originalDirection, bool toRight);
      VehicleBase(const VehicleBase& other);
      ~VehicleBase();


      inline int getVehicleID() const { return this->vehicleID; }

      inline VehicleType getVehicleType() const { return this->vehicleType; }
      inline Direction   getVehicleOriginalDirection() const { return this->vehicleDirection; }
      inline bool willTurnRight() {return this->turnRight; }

      // void moveForward(vector<VehicleBase*> &road, StopLight& light);
      // void moveRight(vector<VehicleBase*> &initialRoad, vector<VehicleBase*> &newRoad);
};

#endif
