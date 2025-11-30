#include "simulation.h"
#include "simulation_state.h"
#include "trains.h"
#include "switches.h"
#include "io.h"
#include "grid.h"
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
using namespace std;

// ============================================================================
// SIMULATION.CPP - Implementation of main simulation logic
// ============================================================================

int rows, cols;
char grid[max_rows][max_cols];    
char originalGrid[max_rows][max_cols];
int train_x[max_trains];
int train_status[max_trains];
int train_spawnticks[max_trains];
int train_direction[max_trains];
int metric_delivered;
int metric_crashed;
int max_trains;
int NumTrains;
int NumSwitches;
int seed;
char levelName[100];
int currentTick;

//additional function for grid printing
void printGrid(){
    cout<<"Tick: "<<currentTick<<endl;          //provide the tick b4 starting

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
                display[x][y]='0'+(i%10);
            }
        }
    }
    //print grid wala loop
    for (int i =0; i<rows; i++){
        for (int j=0; j<cols; j++){
            cout<<display[i][j];
        }
        cout<<endl;
    }
    cout<<"\nActive Trains: \n";
for (int i=0; i<NumTrains; i++){
    if (train_status[i]==TRAIN_MOVING || train_status[i]== TRAIN_DELAYED || train_status[i]== TRAIN_WAITING) {
        const string dirNames[]= {"UP", "RIGHT", "DOWN", "LEFT"};
        const string stateNames[]= {"INACTIVE", "WAITING", "MOVING","CRASHED", "DELAYED", "ARRIVED"};
        cout<<" Train "<<i<<" at (" <<train_x[i]<<","<<train_y[i]<<") moving "<< dirNames[train_direction[i]]<<" state: "<<stateNames[train_status[i]]<<endl;}}
        cout<<"Delivered: "<<metric_delivered<< " \n Crashed: "<<metric_crashed<<"\n";    }

// ----------------------------------------------------------------------------
// INITIALIZE SIMULATION
// ----------------------------------------------------------------------------

void initializeSimulation() {
initializeSimulationState();
cout<<"Simulation Initialized!"<<endl;
cout<<"Your level: "<<(levelName[0]!='\0'? levelName:"Unknown")<<endl;
cout<<"Your Trains: "<<NumTrains<<endl;
cout<<"Grid: "<<rows<<"x"<< cols <<endl;
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
cout<<"route determination underway.."<<endl;
determineAllRoutes();
cout<<"switch counters are now being updated.."<<endl;
//updation of switch counters actuve trains kei liye
for (int i=0; i<NumTrains; i++){
    if (train_status[i]==TRAIN_MOVING || train_status[i]==TRAIN_WAITING){
        int x= train_x[i];
        int y=train_y[i];
        if (isInBounds(x, y)){
            char tile= grid[x][y];
            if (isSwitchTile(tile)){
                int switchIndex=getSwitchIndex(tile);
                if (switchIndex>=0){
                    updateSwitchCounters(switchIndex, train_direction[i]);
                }
            }
        }
    }
}
cout<<"Switch flips beeing queued"<<endl;
queueSwitchFlips();
applyDeferredFlips();
cout<<"Collisions being detected"<<endl;
detectCollisions();
cout<<"Moving trains"<<endl;
moveAllTrains();
printGrid();
currentTick++;
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
        if (train_status[i] == TRAIN_INACTIVE && train_spawnticks[i] <=currentTick){
            allspawned=false;
            break;
        }
    }
    return (active==0 && allspawned);
}
