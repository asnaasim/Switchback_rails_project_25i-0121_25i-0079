#include "simulation.h"
#include "simulation_state.h"
#include "trains.h"
#include "switches.h"
#include "io.h"
#include "grid.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;

// ============================================================================
// SIMULATION.CPP - Implementation of main simulation logic
// ============================================================================
//additional function for grid printing
void printGrid(){
    cout<<"Tick: "<<currentTick<<endl;

    //creation of a grid
    char display[max_rows][max_cols];
    for (int i=0; i<rows; i++){
        for (int j=0; j<cols; j++){
            display[i][j]=originalGrid[i][j];
        }
    }
    //the trains thatll come over the track
    for (int i=0; i<NumTrains; i++){
        if (train_status[i]==TRAIN_MOVING || train_status[i]==TRAIN_WAITING || train_status[i]==TRAIN_DELAYED){
            int x= train_x[i];
            int y= train_y[i];
            if (isInBounds(x, y)){
                display[y][x]='0'+(i%10);
            }
        }
    }
    
    for (int i =0; i<rows; i++){
        for (int j=0; j<cols; j++){
            cout<<display[i][j];
        }
        cout<<endl;
    }
    cout<<"\nActive Trains: \n";
for (int i=0; i<NumTrains; i++){
    if (train_status[i]==TRAIN_MOVING || train_status[i]== TRAIN_DELAYED || train_status[i]== TRAIN_WAITING) {
        const char* dirNames[]= {"UP", "RIGHT", "DOWN", "LEFT"};
        const char* stateNames[]= {"INACTIVE", "WAITING", "MOVING", "DELAYED", "ARRIVED", "CRASHED"};
        cout<<" Train "<<i<<" at (" <<train_x[i]<<","<<train_y[i]<<") moving "<< dirNames[train_direction[i]]<<" state: "<<stateNames[train_status[i]]<<"\n";}}
        cout<<"Delivered: "<<metric_delivered<< " \n Crashed: "<<metric_crashed<<"\n";    }

// ----------------------------------------------------------------------------
// INITIALIZE SIMULATION
// ----------------------------------------------------------------------------

void initializeSimulation() {
initializeSimulationState();
cout<<"Simulation Initialized!"<<endl;
cout<<"Your level: "<<levelName<<endl;
cout<<"Your Trains: "<<NumTrains<<endl;
cout<<"Grid: "<<grid<<endl;
cout<<"Switches: "<<NumSwitches<<endl;
cout<<"seed: "<<seed<<endl;

currentTick=0;

initializeLogFiles();
}


// ----------------------------------------------------------------------------
// SIMULATE ONE TICK
// ----------------------------------------------------------------------------

void simulateOneTick() {
cout<<"The tick is currently: "<<currentTick<<endl;
cout<<"trains will now be spawned.."<<endl;
spawnTrainsForTick();
cout<<"route determination is underway.."<<endl;
determineAllRoutes();
cout<<"switch counders are now being updated.."<<endl;
updateSwitchCounters();
cout<<"Switch flips beeing queued"<<endl;
queueSwitchFlips();
cout<<"Collisions being detected"<<endl;
detectCollisions();
 }



// ----------------------------------------------------------------------------
// CHECK IF SIMULATION IS COMPLETE
// ----------------------------------------------------------------------------

bool isSimulationComplete() {
    //loop kei all trains r delivered/crashed
    int active=0;
    for (int i=0; i<NumTrains; i++){
        if (train_status[i] != TRAIN_INACTIVE && train_status[i] != TRAIN_ARRIVED && train_status[i] != TRAIN_CRASHED){
            active++;
        }
    }
    //check to see if trains are attempted to spawn
    bool allspawned=true;
        for (int i=0; i<NumTrains; i++){
        if (train_status[i] == TRAIN_INACTIVE && train_status[i] <=currentTick){
            allspawned=false;
            break;
        }
    }
    return (active==0 && allspawned);
}

