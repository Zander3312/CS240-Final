// Names: Zander Hesch and Alan Kassymkanov
// Date: 11-17-22
// Main method of traffic simulator. Handles movement of cars, right turns, making sure cars do not collide, reading input file, and RNG.
// Run this program using "./simulation <input file> <initial RNG seed>"
// First segment of cars should be spawned in using random number generation then this program will spawn the rest of the segments progressively as needed.
// 
// NOTE: Currently only for use with cars. When vehicleBase.cpp is finalized, other types with be supported.

#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <map>
#include "Animator.h"
#include "VehicleBase.h"
using namespace std;


//Declare methods
void moveVehicles(vector<VehicleBase*>& vehicles, StopLight& light, map<string, string> inputMap);
void turnRight(vector<VehicleBase*>& directionFrom, vector<VehicleBase*>& directionTo, int sectionsBeforeIntersection);
void spawnVehicle(vector<VehicleBase*>& vehicles, Direction direction, map<string, string> inputMap, double typeRandNum, double turnRandNum);

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

    //Create map that will hold input data
    string currentWord; //Declare string that will hold the name of the value from the input file, used to check if input file is correct
    string currentValue; //Declare string that will hold the value associated with that string
    map<string, string> inputMap; //Initialize map
    while (inFile >> currentWord >> currentValue) { //Loop until all values from input file are exhausted
        inputMap.insert(make_pair(currentWord, currentValue)); //Insert data into map
    }

    inFile.close(); //Done with input file

    //Initialize values for RNG
    mt19937 randomNumberGenerator; //Mersenne twister; handles RNG
    uniform_real_distribution<double> rand_double(0,1); //rand_double will only generate values between 0 and 1
    randomNumberGenerator.seed(stoi(argv[2])); //Set RNG seed to the seed given in the command line
    double randNum = rand_double(randomNumberGenerator); //Random number used to generate car spawns

    Animator::MAX_VEHICLE_COUNT = 999;  // vehicles will be displayed with three digits

    Animator anim(stoi(inputMap["number_of_sections_before_intersection:"]));

    // construct vectors of VehicleBase* of appropriate size, init to nullptr
    vector<VehicleBase*> westbound(stoi(inputMap["number_of_sections_before_intersection:"]) * 2 + 2, nullptr);
    vector<VehicleBase*> eastbound(stoi(inputMap["number_of_sections_before_intersection:"]) * 2 + 2, nullptr);
    vector<VehicleBase*> southbound(stoi(inputMap["number_of_sections_before_intersection:"]) * 2 + 2, nullptr);
    vector<VehicleBase*> northbound(stoi(inputMap["number_of_sections_before_intersection:"]) * 2 + 2, nullptr);

    char dummy; //Used to wait for input

    // Initialize stoplight colors - delete when lights are actually implemented
    //anim.setLightNorthSouth(LightColor::green);
    //anim.setLightEastWest(LightColor::red);

    //StopLight testLight; //Create StopLight object for testing

    int greenNS = stoi(inputMap["green_north_south:"]);
    int yellowNS = stoi(inputMap["yellow_north_south:"]);
    int greenEW = stoi(inputMap["green_east_west:"]);
    int yellowEW = stoi(inputMap["yellow_east_west:"]);
    int redNS = greenEW + yellowEW;
    int redEW = greenNS + yellowNS;

    StopLight lightNS = StopLight(greenNS, yellowNS, redNS, LightColor::green);
    StopLight lightEW = StopLight(greenEW, yellowEW, redEW, LightColor::red);

    anim.setLightNorthSouth(LightColor::green);
    anim.setLightEastWest(LightColor::red);


    //Main loop for moving and drawing the vehicles
    for (int clock = 0; clock < stoi(inputMap["maximum_simulated_time:"]); clock++) {

        //Move vehicles in all directions
        moveVehicles(northbound, lightNS, inputMap);
        moveVehicles(westbound, lightEW, inputMap);
        moveVehicles(southbound, lightNS, inputMap);
        moveVehicles(eastbound, lightEW, inputMap);

        //Check for and execute right turns
        if (northbound[stoi(inputMap["number_of_sections_before_intersection:"])+1] != nullptr && northbound[stoi(inputMap["number_of_sections_before_intersection:"])+1]->VehicleBase::willTurnRight()) { //If there is a car segment in the first part of an intersection (later check if car is turning right)
           turnRight(northbound, eastbound, stoi(inputMap["number_of_sections_before_intersection:"])); //Change car from first lane to new lane after having turned right
        }
        if (westbound[stoi(inputMap["number_of_sections_before_intersection:"])+1] != nullptr && westbound[stoi(inputMap["number_of_sections_before_intersection:"])+1]->VehicleBase::willTurnRight()) { //If there is a car segment in the first part of an intersection (later check if car is turning right)
           turnRight(westbound, northbound, stoi(inputMap["number_of_sections_before_intersection:"])); //Change car from first lane to new lane after having turned right
        }
        if (southbound[stoi(inputMap["number_of_sections_before_intersection:"])+1] != nullptr && southbound[stoi(inputMap["number_of_sections_before_intersection:"])+1]->VehicleBase::willTurnRight()) { //If there is a car segment in the first part of an intersection (later check if car is turning right)
           turnRight(southbound, westbound, stoi(inputMap["number_of_sections_before_intersection:"])); //Change car from first lane to new lane after having turned right
        }
        if (eastbound[stoi(inputMap["number_of_sections_before_intersection:"])+1] != nullptr && eastbound[stoi(inputMap["number_of_sections_before_intersection:"])+1]->VehicleBase::willTurnRight()) { //If there is a car segment in the first part of an intersection (later check if car is turning right)
           turnRight(eastbound, southbound, stoi(inputMap["number_of_sections_before_intersection:"])); //Change car from first lane to new lane after having turned right
        }

        //Run RNG to see if a vehicle will be spawned; if so, spawn the vehicle
        randNum = rand_double(randomNumberGenerator);
        if (randNum < stod(inputMap["prob_new_vehicle_northbound:"]))
            spawnVehicle(northbound, Direction::north, inputMap, rand_double(randomNumberGenerator), rand_double(randomNumberGenerator));
        randNum = rand_double(randomNumberGenerator);
        if (randNum < stod(inputMap["prob_new_vehicle_westbound:"]))
            spawnVehicle(westbound, Direction::west, inputMap, rand_double(randomNumberGenerator), rand_double(randomNumberGenerator));
        randNum = rand_double(randomNumberGenerator);
        if (randNum < stod(inputMap["prob_new_vehicle_southbound:"]))
            spawnVehicle(southbound, Direction::south, inputMap, rand_double(randomNumberGenerator), rand_double(randomNumberGenerator));
        randNum = rand_double(randomNumberGenerator);
        if (randNum < stod(inputMap["prob_new_vehicle_eastbound:"]))
            spawnVehicle(eastbound, Direction::east, inputMap, rand_double(randomNumberGenerator), rand_double(randomNumberGenerator));


        lightEW.decrement();
        lightNS.decrement();

        anim.setLightNorthSouth(lightNS.getColor());
        anim.setLightEastWest(lightEW.getColor());

        //Set vehicles moving in all directions
        anim.setVehiclesNorthbound(northbound);
        anim.setVehiclesWestbound(westbound);
        anim.setVehiclesSouthbound(southbound);
        anim.setVehiclesEastbound(eastbound);

        //Draw the cars into the intersection as given
        anim.draw(clock);

        //Wait for input to progress in the loop
        std::cin.get(dummy);



        
    }
}

// Method moves vehicles in a given direction one unit at a time. Called every time input is given. Vehicles are moved before the animator is drawn for that frame
//(after might be better for the sake of fulfilling Dr. Szajda's condition of no vehicles should be on the grid at the start). First parameter is the lane that is
//to be moved.
// void moveVehicles(vector<VehicleBase*>& vehicles) {
//     for (size_t i = 0; i < vehicles.size(); i++) {
//         if (vehicles[vehicles.size()-1-i] != nullptr) { //If current space on grid is not a null pointer (iterates backwards), makes it so if the currently checked tile is empty no operations are done
//             if (vehicles.size()-1-i == 0 && vehicles[vehicles.size()-1-i] != vehicles[vehicles.size()-i]) { //If current space is the first space and space ahead is not the same as current space (later include properties to account for other vehicle types than car)
//                 vehicles[vehicles.size()-i] = vehicles[vehicles.size()-1-i]; //Make current vehicle segment also occupy the space in front of it
//                 continue; //Do not delete previous segment location; previous segment location becomes next segment for a newly generated vehicle
//             }
//             if (vehicles.size()-1-i == vehicles.size()-1) { //If the current space is the last space
//                 if (vehicles[vehicles.size()-2-i] != vehicles[vehicles.size()-1]) { //If this is the last segment of a car
//                     delete vehicles[vehicles.size()-1]; //Deallocate memory allocated to the vehicle
//                 }
//                 vehicles[vehicles.size()-i-1] = nullptr; //Delete the segment; it does not get placed in the next segment because there is no next segment, the vehicle segment exits the intersection
//             } else if (vehicles.size()-1-i == 0) { //If current space is the first space; we know from earlier this is the last segment of the vehicle (ACCOUNT FOR OTHER VEHICLE TYPES LATER!)
//                 vehicles[vehicles.size()-i] = vehicles[vehicles.size()-1-i]; //Make current vehicle segment also occupy the space in front of it
//                 vehicles[vehicles.size()-i-1] = nullptr; //Delete previous segment location
//             } else { //If current space is anywhere in between the first and last spaces
//                 vehicles[vehicles.size()-i] = vehicles[vehicles.size()-1-i]; //Make current vehicle segment also occupy the space in front of it
//                 if (vehicles[vehicles.size()-i-1] != vehicles[vehicles.size()-i-2]) { //If this is the last segment in a car (change to account for other sizes later)
//                     vehicles[vehicles.size()-i-1] = nullptr; //Delete previous segment location
//                 }
//             }
//         }
//     }
// }

// Method moves vehicles in a given direction one unit at a time. Called every time input is given. Vehicles are moved before the animator is drawn for that frame
//(after might be better for the sake of fulfilling Dr. Szajda's condition of no vehicles should be on the grid at the start). First parameter is the lane that is
//to be moved. Second parameter is the color of the stoplight so vehicles can stop at the stoplight when needed. Third parameter is the input map for use when inputted
//values are needed.
void moveVehicles (vector<VehicleBase*>& vehicles, StopLight& light, map<string, string> inputMap) {
    for (size_t i = 0; i < vehicles.size(); i++) { //Loop through all spaces in lane

        //Check if the current space has a vehicle pointer in it
        if (vehicles[vehicles.size()-1-i] == nullptr) { //If current space does not have a vehicle in it
            continue; //Skip past this space and continue looking for vehicles
        }

        //If any code in the method past here is read, that means there is a vehicle in the current space (vehicles.size()-i-1)

        //Operations for if the current space is the first space
        if (vehicles.size()-i-1 == 0) { //If the current space is the first space
            if (vehicles[vehicles.size()-1-i] != vehicles[vehicles.size()-i+1+(vehicles[vehicles.size()-1-i]->getLength()-2)]) { //If segment ahead is not the same as current segment
                vehicles[vehicles.size()-i] = vehicles[vehicles.size()-1-i]; //Make current vehicle segment also occupy the space in front of it
                continue; //Do not delete previous segment location; previous segment location becomes next segment for a newly generated vehicle
            } else { //If this is the last segment of a car in the first space, we treat it like any other space where it just needs to move forward
                vehicles[vehicles.size()-i] = vehicles[vehicles.size()-1-i]; //Make current vehicle segment also occupy the space in front of it
                vehicles[vehicles.size()-i-1] = nullptr; //Delete previous segment location
                continue;
            }
        }

        //Operations for if the current space is the last space
        if (vehicles.size()-i-1 == vehicles.size()-1) { //If the current space is the last space
            if (vehicles[vehicles.size()-2-i] != vehicles[vehicles.size()-1]) { //If this is the last segment of a car
                delete vehicles[vehicles.size()-1]; //Deallocate memory allocated to the vehicle
            }
            vehicles[vehicles.size()-i-1] = nullptr; //Delete the segment; it does not get placed in the next segment because there is no next segment, the vehicle segment exits the intersection
            continue;
        }

        //Check if the space ahead of the vehicle is filled already
        if (vehicles[vehicles.size()-i] != nullptr) { //If the space ahead of the current space is not empty
            continue; //The space ahead already has a vehicle in it and thus the current segment cannot move forward, otherwise they would overlap
        }

        //Check if the vehicle is in front of the intersection, and if so, check if it can enter the intersection
        if (vehicles.size()-i-1 == stoi(inputMap["number_of_sections_before_intersection:"]) - 1) { //If the current space is right in front of the intersection

        //     //Add stoplight check here later once the class works on my computer

            LightColor currentColor = light.getColor();
            if (currentColor == LightColor::red){
                continue;
            }
            else if (currentColor == LightColor::yellow){
                if (vehicles[(vehicles.size() - i - 1) - (vehicles[vehicles.size() - i - 1]->getLength() - 1)] == vehicles[vehicles.size() - i  - 1]){  //if this is the first segment 
                    if (light.getTimeLeft() <= vehicles[vehicles.size() - i - 1]->getLength()){
                        continue;
                    }
                }
            }

        }

        int pointBeforeIntersection = vehicles.size() / 2 - 2;
        if (vehicles.size() - i - 1 == pointBeforeIntersection){
            
        }


        //If the current space anywhere that isn't a special case
        vehicles[vehicles.size()-i] = vehicles[vehicles.size()-1-i]; //Make current vehicle segment also occupy the space in front of it
        vehicles[vehicles.size()-i-1] = nullptr; //Delete previous segment location

        
    }
}

//Method changes the array a segment of a vehicle is in when it moves forward to simulate it having taken a right turn. Should be called once per segment to change
//each one individually as they go through the turn. Because the stoplights will only allow west/east or north/south to move through the intersection at the same time,
//there is no need to account for if a car is already on the space the turning car is attempting to move into. First parameter is the car's original vector, second
//parameter is the car's new vector it is turning into, and third parameter is the number of sections before the intersection.
void turnRight(vector<VehicleBase*>& directionFrom, vector<VehicleBase*>& directionTo, int sectionsBeforeIntersection) {
    directionTo[sectionsBeforeIntersection+2] = directionFrom[sectionsBeforeIntersection+1]; //Move segment from current lane to new lane
    directionFrom[sectionsBeforeIntersection+1] = nullptr; //Remove segment from old lane
}

//Method spawns a vehicle in the given space. Called after RNG determines that a vehicle going a given direction should be spawned in the main for loop. First parameter
//contains the vector corresponding to the direction the spawning vehicle will travel and the vector the spawning vehicle will be placed at the start of.
//Second parameter determines which direction the spawned vehicle will be traveling in. Third parameter is for using the values from the input file in this scope.
//Fourth and fifth parameters are random numbers generated prior that decide the type of the vehicle and whether it will turn right or not respectively.
void spawnVehicle(vector<VehicleBase*>& vehicles, Direction direction, map<string, string> inputMap, double typeRandNum, double turnRandNum) {
    if (vehicles[0] != nullptr) { //If the first space is already occupied
        return; //Don't do anything; you can't spawn in a vehicle if a vehicle is already occupying the space
    }

    //Initialize variables to pass into VehicleBase constructor; no need for direction since that was determined from where the method was called
    VehicleType type = VehicleType::car; //Type of vehicle
    bool toRight; //Whether the vehicle will turn right or not

    //Calculate the type of the vehicle and whether or not it will turn right
    if (typeRandNum < stod(inputMap["proportion_of_cars:"])) { //If the random number is within the odds of the probability it will be a car
        type = VehicleType::car; //Make the vehicle a car
        if (turnRandNum < stod(inputMap["proportion_right_turn_cars:"])) { //If the random number is within the odds of the car turning right
            toRight = true; //The car will turn right
        } else {
            toRight = false; //The car will not turn right
        }
    } else if (typeRandNum < stod(inputMap["proportion_of_SUVs:"]) + stod(inputMap["proportion_right_turn_cars:"])) { //If the random number is within the odds of the probability it will be an SUV
        type = VehicleType::suv; //Make the vehicle an SUV
        if (turnRandNum < stod(inputMap["proportion_right_turn_SUVs:"])) { //If the random number is within the odds of the SUV turning right
            toRight = true; //The SUV will turn right
        } else {
            toRight = false; //The SUV will not turn right
        }
    } else { //If the random number is not within the odds of either of the previous two options, it must be a truck by process of elimination
        type = VehicleType::truck; //Make the vehicle a truck
        if (turnRandNum < stod(inputMap["proportion_right_turn_trucks:"])) { //If the random number is within the odds of the truck turning right
            toRight = true; //The truck will turn right
        } else {
            toRight = false; //The truck will not turn right
        }
    }

    vehicles[0] = new VehicleBase(type, direction, toRight); //Place newly generated vehicle in the first space of the lane
}