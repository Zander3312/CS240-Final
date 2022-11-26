// Names: Zander Hesch and Alan Kassymkanov
// Date: 11-17-22
// Main method of traffic simulator. Handles movement of cars, right turns, making sure cars do not collide, reading input file, and RNG.
// Run this program using "./simulation <input file> <initial RNG seed>"
// First segment of cars should be spawned in using random number generation then this program will spawn the rest of the segments progressively as needed.
// NOTE: Currently only for use with cars. When vehicleBase.cpp is finalized, other types with be supported.

#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include "Animator.h"
#include "VehicleBase.h"
using namespace std;


//Declare methods
void moveVehicles(vector<VehicleBase*>& vehicles);
//void turnRight(vector<VehicleBase*>& vehicles);
void spawnVehicle(vector<VehicleBase*>& vehicles, Direction direction);

int main(int argc, char* argv[]) {
    //Check if number of arguments is correct; if not, print proper usage then exit program
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <input file> <initial RNG seed>" << endl;
        exit(0);
    }

    //Begin reading from input file
    ifstream inFile {argv[1]}; //Begin handling input file

    if (!inFile) { //Checks if given input file exists
        cerr << "ERROR: Could not open file: " << argv[1] << endl; //If it doesn't exist, then say so then exit program
        exit(0);
    }

    //Assign values from input file to corresponding variable
    string currentWord; //Declare string that will hold the name of the value from the input file; not actually used, only for advancing in the document
    string currentValue; //Declare string that will hold the value associated with that string
    inFile >> currentWord >> currentValue; //Get values from line of input file
    int maxSimulatedTime = stoi(currentValue); //Assign the value to a variable corresponding with the given value for later use
    inFile >> currentWord >> currentValue; //Repeat this process for all values
    int sectionsBeforeIntersection = stoi(currentValue);
    inFile >> currentWord >> currentValue;
    int greenNorthSouth = stoi(currentValue);
    inFile >> currentWord >> currentValue;
    int yellowNorthSouth = stoi(currentValue);
    inFile >> currentWord >> currentValue;
    int greenEastWest = stoi(currentValue);
    inFile >> currentWord >> currentValue;
    int yellowEastWest = stoi(currentValue);
    inFile >> currentWord >> currentValue;
    double probNewVehicleNorth = stod(currentValue);
    inFile >> currentWord >> currentValue;
    double probNewVehicleSouth = stod(currentValue);
    inFile >> currentWord >> currentValue;
    double probNewVehicleEast = stod(currentValue);
    inFile >> currentWord >> currentValue;
    double probNewVehicleWest = stod(currentValue);
    inFile >> currentWord >> currentValue;
    double proportionCars = stod(currentValue);
    inFile >> currentWord >> currentValue;
    double proportionSUVs = stod(currentValue);
    inFile >> currentWord >> currentValue;
    double proportionRightCars = stod(currentValue);
    inFile >> currentWord >> currentValue;
    double proportionLeftCars = stod(currentValue);
    inFile >> currentWord >> currentValue;
    double proportionRightSUVs = stod(currentValue);
    inFile >> currentWord >> currentValue;
    double proportionLeftSUVs = stod(currentValue);
    inFile >> currentWord >> currentValue;
    double proportionRightTrucks = stod(currentValue);
    inFile >> currentWord >> currentValue;
    double proportionLeftTrucks = stod(currentValue);

    inFile.close(); //Done with input file

    //Initialize values for RNG
    mt19937 randomNumberGenerator; //Mersenne twister; handles RNG
    uniform_real_distribution<double> rand_double(0,1); //rand_double will only generate values between 0 and 1
    randomNumberGenerator.seed(stoi(argv[2])); //Set RNG seed to the seed given in the command line
    double randNum = rand_double(randomNumberGenerator); //Random number used to generate car spawns

    Animator::MAX_VEHICLE_COUNT = 999;  // vehicles will be displayed with three digits

    Animator anim(sectionsBeforeIntersection);

    // construct vectors of VehicleBase* of appropriate size, init to nullptr
    vector<VehicleBase*> westbound(sectionsBeforeIntersection * 2 + 2, nullptr);
    vector<VehicleBase*> eastbound(sectionsBeforeIntersection * 2 + 2, nullptr);
    vector<VehicleBase*> southbound(sectionsBeforeIntersection * 2 + 2, nullptr);
    vector<VehicleBase*> northbound(sectionsBeforeIntersection * 2 + 2, nullptr);

    char dummy; //Used to wait for input

    // initialize test vehicles; should not be in final program and vehicles will spawn in randomly
    // VehicleBase vb1(VehicleType::car, Direction::south);
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

    //eastbound[0] = &vb1; //Initialize first half of car for testing movement east

    //Main loop for moving and drawing the vehicles
    for (int clock = 0; clock < maxSimulatedTime; clock++) {

        //Set vehicles moving in all directions
        anim.setVehiclesNorthbound(northbound);
        anim.setVehiclesWestbound(westbound);
        anim.setVehiclesSouthbound(southbound);
        anim.setVehiclesEastbound(eastbound);

        //Draw the cars into the intersection as given
        anim.draw(clock);

        //Wait for input to progress in the loop
        std::cin.get(dummy);

        //Move vehicles in all directions
        moveVehicles(northbound);
        moveVehicles(westbound);
        moveVehicles(southbound);
        moveVehicles(eastbound);

        //Initialize double used for caluclating car spawns
        double randNum = rand_double(randomNumberGenerator);

        //Run RNG to see if a vehicle will be spawned; if so, spawn the vehicle
        randNum = rand_double(randomNumberGenerator);
        if (randNum < probNewVehicleNorth)
            spawnVehicle(northbound, Direction::north);
        randNum = rand_double(randomNumberGenerator);
        if (randNum < probNewVehicleWest)
            spawnVehicle(westbound, Direction::west);
        randNum = rand_double(randomNumberGenerator);
        if (randNum < probNewVehicleSouth)
            spawnVehicle(southbound, Direction::south);
        randNum = rand_double(randomNumberGenerator);
        if (randNum < probNewVehicleEast)
            spawnVehicle(eastbound, Direction::east);
        
    }
}

// Method moves vehicles in a given direction one unit at a time. Called every time input is given. Vehicles are moved after the animator is drawn for that frame
//(probably will change to before animator is drawn later but this is better for testing at the moment).
void moveVehicles(vector<VehicleBase*>& vehicles) {
    for (int i = 0; i < vehicles.size(); i++) {
        if (vehicles[vehicles.size()-1-i] != nullptr) { //if current space on grid is not a null pointer (iterates backwards), makes it so if the currently checked tile is empty no operations are done
            if (vehicles.size()-1-i == 0 && vehicles[vehicles.size()-1-i] != vehicles[vehicles.size()-i]) { //if current space is the first space and space ahead is not the same as current space (later include properties to account for other vehicle types than car)
                vehicles[vehicles.size()-i] = vehicles[vehicles.size()-1-i]; //Make current vehicle segment also occupy the space in front of it
                continue; //Do not delete previous segment location; previous segment location becomes next segment for a newly generated vehicle
            }
            if (vehicles.size()-1-i == vehicles.size()-1) { //If the current space is the last space
                if (vehicles[vehicles.size()-2-i] == nullptr) { //If this is the last segment of a car
                    delete vehicles[vehicles.size()-1]; //Deallocate memory allocated to the vehicle
                }
                vehicles[vehicles.size()-i-1] = nullptr; //Delete the segment; it does not get placed in the next segment because there is no next segment, the vehicle segment exits the intersection
            } else {
                vehicles[vehicles.size()-i] = vehicles[vehicles.size()-1-i]; //Make current vehicle segment also occupy the space in front of it
                if (vehicles[vehicles.size()-i-1] != vehicles[vehicles.size()-i-2]) { //If this is the last segment in a car (change to account for other sizes later)
                    vehicles[vehicles.size()-i-1] = nullptr; //Delete previous segment location
                }
            }
        }
    }
}

//Method changes the array a segment of a vehicle is in when it moves forward to simulate it having taken a right turn. Should be called once per segment to change
//each one individually as they go through the turn. Because the stoplights will only allow west/east or north/south to move through the intersection at the same time,
//there is no need to account for if a car is already on the space the turning car is attempting to move into.
// void turnRight(vector<VehicleBase*>& vehicles) {

// }

//Method spawns a vehicle in the given space. Called after RNG determines that a vehicle going a given direction should be spawned in the main for loop. First parameter
//contains the vector corresponding to the direction the spawning vehicle will travel and the vector the spawning vehicle will be placed at the start of.
//Second parameter determines which direction the spawned vehicle will be traveling in.
void spawnVehicle(vector<VehicleBase*>& vehicles, Direction direction) {
    if (vehicles[0] != nullptr) { //If the first space is already occupied
        return; //Don't do anything; you can't spawn in a vehicle if a vehicle is already occupying the space
    }

    //Place things that will determine the qualities of the vehicle here (what type of vehicle, will it turn right, etc.)

    VehicleBase* newVehicle = new VehicleBase(VehicleType::car, direction); //Dynamically allocate new vehicle; dynamic allocation is needed because otherwise the vehicle would stop existing once the function call ended
    vehicles[0] = newVehicle; //Place newly generated vehicle in the first space of the lane
}