#include "trains.h"
#include "simulation_state.h"
#include "grid.h"
#include "switches.h"
#include "io.h"
#include <cmath>
#include <iostream>
#include <cstdlib>
using namespace std;

// ============================================================================
// TRAINS.CPP - Train logic
// ============================================================================
// helper function to help us calculate manhattan distance to check priority of trains
int manhattandistance(int x1, int y1, int x2, int y2){
    int distance= (abs(x1-x2)+ abs(y1-y2));
    return distance;
}
// Storage for planned moves (for collisions).

// Previous positions (to detect switch entry).

// ----------------------------------------------------------------------------
// SPAWN TRAINS FOR CURRENT TICK
// ----------------------------------------------------------------------------
// Activate trains scheduled for this tick.
// ----------------------------------------------------------------------------
void spawnTrainsForTick() {
    for (int i=0; i<NumTrains; i++){
        if (train_status[i] == TRAIN_INACTIVE && train_status[i]==currentTick){
            int xcoordinate= train_x[i];
            int ycoordinate= train_y[i];
         //this function will help us determine if train has moved even the slightest
        
        //to see if spawn position occupied
        bool spacetaken= false;
        for (int j=0; j<NumTrains; j++){
            if (train_status[j]!= TRAIN_INACTIVE && train_status[j]!=TRAIN_ARRIVED && train_status[j]!= TRAIN_CRASHED && i!=j){
                if (train_x[j]==xcoordinate && train_y[j]==ycoordinate){
                    spacetaken=true;
                    break;

                }
            }
        }
        if (spacetaken){
            train_spawnticks[i]++;
        }
        else {
            train_previousx[i]=xcoordinate;
            train_previousy[i]=ycoordinate;
            train_status[i]=TRAIN_MOVING;
            cout<<"A new train spawned"<<endl;

        }
    }
}
}

// ----------------------------------------------------------------------------
// DETERMINE NEXT POSITION for a train
// ----------------------------------------------------------------------------
// Compute next position/direction from current tile and rules.
// ----------------------------------------------------------------------------
bool determineNextPosition() {
}

// ----------------------------------------------------------------------------
// GET NEXT DIRECTION based on current tile and direction
// ----------------------------------------------------------------------------
// Return new direction after entering the tile.
// ----------------------------------------------------------------------------
int getNextDirection(char tiletype, int switch_state, int currentdirection ) {
     //direction based on tile type
     //---straight tiles ki condition---//
    if (tiletype=='-'){
        if (currentdirection==DIR_LEFT || currentdirection==DIR_RIGHT){
            return currentdirection;
        }
        else{
            return currentdirection;
        }
    }
    if (tiletype=='|'){
        if (currentdirection==DIR_UP || currentdirection==DIR_DOWN){
            return currentdirection;
        }
        else{
            return currentdirection;
        }
    }

    //-----curved tiles ki condition----//
    if (tiletype=='/') {
        if (currentdirection==DIR_LEFT){
            return DIR_LEFT;
        }
        else if (currentdirection==DIR_RIGHT){
            return DIR_RIGHT;
        }
        else if (currentdirection==DIR_UP){
            return DIR_UP;
        }
        else if (currentdirection==DIR_DOWN {
            return DIR_DOWN;
            
        }
    }
    if (tiletype=='\\') {
        if (currentdirection==DIR_LEFT){
            return DIR_UP;
        }
        else if (currentdirection==DIR_RIGHT){
            return DIR_DOWN;
        }
        else if (currentdirection==DIR_UP){
            return DIR_LEFT;
        }
        else if (currentdirection==DIR_DOWN) {
            return DIR_RIGHT;
            
        }
    if (tiletype=='D'|| tiletype=='S'||tiletype=='='|| tiletype=='+'){
        return currentdirection;
    }
    if (isSwitchTile(tiletype)){
        
    }
    }


}

// ----------------------------------------------------------------------------
// SMART ROUTING AT CROSSING - Route train to its matched destination
// ----------------------------------------------------------------------------
// Choose best direction at '+' toward destination.
// ----------------------------------------------------------------------------
int getSmartDirectionAtCrossing() {
}

// ----------------------------------------------------------------------------
// DETERMINE ALL ROUTES (PHASE 2)
// ----------------------------------------------------------------------------
// Fill next positions/directions for all trains.
// ----------------------------------------------------------------------------
void determineAllRoutes() {
}

// ----------------------------------------------------------------------------
// MOVE ALL TRAINS (PHASE 5)
// ----------------------------------------------------------------------------
// Move trains; resolve collisions and apply effects.
// ----------------------------------------------------------------------------
void moveAllTrains() {
}

// ----------------------------------------------------------------------------
// DETECT COLLISIONS WITH PRIORITY SYSTEM
// ----------------------------------------------------------------------------
// Resolve same-tile, swap, and crossing conflicts.
// ----------------------------------------------------------------------------
void detectCollisions() {

}

// ----------------------------------------------------------------------------
// CHECK ARRIVALS
// ----------------------------------------------------------------------------
// Mark trains that reached destinations.
// ----------------------------------------------------------------------------
void checkArrivals() {
    for (int i=0; i<NumTrains; i++){
        if (train_status[i]==TRAIN_MOVING){
            if (train_x[i]==train_destinationx[i]&& train_y[i]==train_destinationy[i]){
                train_status[i]=TRAIN_ARRIVED;
                metric_delivered++;
                cout<<"Train number"<<i<<"arrived at destination"<<endl;
                logTrainTrace(i);

            }
        }
    }
}

// ----------------------------------------------------------------------------
// APPLY EMERGENCY HALT
// ----------------------------------------------------------------------------
// Apply halt to trains in the active zone.
// ----------------------------------------------------------------------------
void applyEmergencyHalt() {
    if (!emergencyhalt_active){
        return;
        
    }
    for (int z=0;z<NumTrains;z++){
        if (train_status[z]==TRAIN_MOVING){
            int dx= abs(train_x[z]-emergencyhalt_x);
            int dy= abs(train_y[z]-emergencyhalt_y);

            if (dx<=1 && dy<=1){
                train_plannedmove[z]=false;
                train_waitticks[z]++;
            }

        }
    }
}


// ----------------------------------------------------------------------------
// UPDATE EMERGENCY HALT
// ----------------------------------------------------------------------------
// Decrement timer and disable when done.
// ----------------------------------------------------------------------------
void updateEmergencyHalt() {            
    if (emergencyhalt_active==true){
        emergencyhalt_timer--;          //decrementing value 
        if (emergencyhalt_timer<=0){            //condition to ensure emegerncy halt timer doesnt go below 1
            emergencyhalt_active=false;
            cout<<"emergency halt disabled"<<endl;
        }
    }
}
