#include <iostream>
#include <vector>
#include "Animator.h"
#include "VehicleBase.h"

// Names: Zander Hesch and Alan Kassymkanov
// Date: 11-17-22
// Main method of traffic simulator. Handles movement of cars, right turns, making sure cars do not collide, etc.
// First segment of cars should be spawned in using random number generation then this program will spawn the rest of the segments progressively as needed.
// NOTE: Currently only for use with cars. When vehicleBase.cpp is finalized, other types with be supported.

using namespace std;

void moveVehicles(vector<VehicleBase*>& vehicles);
//void spawnCars();

int main() {
    Animator::MAX_VEHICLE_COUNT = 999;  // vehicles will be displayed with three digits

    int halfSize = 8;  // number of sections before intersection

    Animator anim(halfSize);

    // construct vectors of VehicleBase* of appropriate size, init to nullptr
    vector<VehicleBase*> westbound(halfSize * 2 + 2, nullptr);
    vector<VehicleBase*> eastbound(halfSize * 2 + 2, nullptr);
    vector<VehicleBase*> southbound(halfSize * 2 + 2, nullptr);
    vector<VehicleBase*> northbound(halfSize * 2 + 2, nullptr);

    char input; //Used to wait for input

    // initialize test vehicles
    VehicleBase vb1(VehicleType::car, Direction::south);
    // VehicleBase vb2(VehicleType::suv, Direction::east);
    // VehicleBase vb3(VehicleType::truck, Direction::east);
    // VehicleBase vb4(VehicleType::car, Direction::west);
    // VehicleBase vb5(VehicleType::suv, Direction::west);
    // VehicleBase vb6(VehicleType::truck, Direction::west);

    // VehicleBase vb7(VehicleType::car, Direction::south);
    // VehicleBase vb8(VehicleType::suv, Direction::south);
    // VehicleBase vb9(VehicleType::truck, Direction::south);
    // VehicleBase vb10(VehicleType::car, Direction::north);
    // VehicleBase vb11(VehicleType::suv, Direction::north);
    // VehicleBase vb12(VehicleType::truck, Direction::north);
    // VehicleBase vb13(vb12);  // just test the copy constructor -- should be #011

    // Initialize stoplight colors - delete when lights are actually implemented
    anim.setLightNorthSouth(LightColor::green);
    anim.setLightEastWest(LightColor::green);

    southbound[0] = &vb1; //Initialize first half of car for testing movement east

    //Main loop for moving and drawing the vehicles
    for (int clock = 0; clock < 1000; clock++) {

        //Set vehicles moving in all directions
        anim.setVehiclesNorthbound(northbound);
        anim.setVehiclesWestbound(westbound);
        anim.setVehiclesSouthbound(southbound);
        anim.setVehiclesEastbound(eastbound);

        //Draw the cars into the intersection as given
        anim.draw(clock);

        //Wait for input to progress in the loop
        std::cin.get(input);

        //Move vehicles in all directions
        moveVehicles(northbound);
        moveVehicles(westbound);
        moveVehicles(southbound);
        moveVehicles(eastbound);
    }
}

// Method moves vehicles in a given direction one unit at a time. Called every time input is given. Vehicles are moved after the animator is drawn for that frame (probably will change to before animator is drawn later but this is better for testing at the moment).
void moveVehicles(vector<VehicleBase*>& vehicles) {
    for (int i = 0; i < vehicles.size(); i++) {
        if (vehicles[vehicles.size()-1-i] != nullptr) { //if current space on grid is not a null pointer (iterates backwards), makes it so if the currently checked tile is empty no operations are done
            if (vehicles.size()-1-i == 0 && vehicles[vehicles.size()-1-i] != vehicles[vehicles.size()-i]) { //if current space is the first space and space ahead is not the same as current space (later include properties to account for other vehicles types than car)
                vehicles[vehicles.size()-i] = vehicles[vehicles.size()-1-i]; //Make current car segment also occupy the space in front of it
                continue; //Do not delete previous segment location; previous segment location becomes next segment for a newly generated vehicle
            }
            vehicles[vehicles.size()-i] = vehicles[vehicles.size()-1-i]; //Make current car segment also occupy the space in front of it
            if (vehicles[vehicles.size()-i-1] != vehicles[vehicles.size()-i-2]) { //If this is the last segment in a car (change to account for other sizes later)
                vehicles[vehicles.size()-i-1] = nullptr; //Delete previous segment location
            }
        }
    }
}