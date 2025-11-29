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

// Storage for planned moves (for collisions).

// Previous positions (to detect switch entry).

// ----------------------------------------------------------------------------
// SPAWN TRAINS FOR CURRENT TICK
// ----------------------------------------------------------------------------
// Activate trains scheduled for this tick.
// ----------------------------------------------------------------------------
void spawnTrainsForTick() {
    for (int i=0; i<NumTrains; i++){
        if (train_status[i] == TRAIN_INACTIVE && train_spawnticks[i]==currentTick){
            int xcoordinate= train_x[i];
            int ycoordinate= train_y[i];
         //check to see if train should be spawnd
        
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
        //lopp to actual mei spawn the train
        if (!spacetaken){
            train_status[i]= TRAIN_MOVING;
            train_previousx[i]= xcoordinate;
            train_previousy[i]= ycoordinate;
            metric_totaltrains++;
            std::cout<<"New train spawned at "<<xcoordinate<<"and "<<ycoordinate<<endl;
            logTrainTrace(currentTick, i, xcoordinate, ycoordinate, train_direction[i], 'M');
        }
        else {
            train_spawnticks[i]++;
            std::cout<<"Train "<<i<<"spawn delayed as space taken"<<endl;

        }
    }
}
}
//helper function to help us calculate manhattan distance to the actual destination
int calcdistance(int trainindex){
    if (trainindex<0 || trainindex>=NumTrains){
        return -1;
    }
    int dest_x= train_destinationx[trainindex];
    int dest_y= train_destinationy[trainindex];
    int current_x= train_x[trainindex];
    int current_y= train_y[trainindex];

    int distance= abs(dest_x-current_x)+ abs(dest_y-current_y);
    return distance;
}

// ----------------------------------------------------------------------------
// DETERMINE NEXT POSITION for a train
// ----------------------------------------------------------------------------
// Compute next position/direction from current tile and rules.
// ----------------------------------------------------------------------------
bool determineNextPosition(int trainindex) {
    if (train_status[trainindex]!= TRAIN_WAITING && train_status[trainindex]!=TRAIN_MOVING && train_status[trainindex]!= TRAIN_DELAYED ){
        return false;
    }
    int currentx= train_x[trainindex];
    int currenty= train_y[trainindex];
    int currentdirection= train_direction[trainindex];
    //calculating next posoition based on direction
    int nexty=currenty;
    int nextx=currentx;
    if (currentdirection==DIR_UP){
        nextx=currentx-1;
    }
    else if (currentdirection==DIR_RIGHT){
        nexty=currenty+1;
    }
    else if (currentdirection==DIR_DOWN){
        nextx=currentx+1;
    }
    else if (currentdirection==DIR_LEFT){
        nexty=currenty-1;
    }

    //loop to see agar next position valid or not
    if (!isInBounds(nextx, nexty)){
        train_status[trainindex]=TRAIN_CRASHED;
        metric_crashed++;
        std::cout<<"Train"<<trainindex<<" crashed"<<endl;
        logTrainTrace(currentTick, trainindex, currentx, currenty, currentdirection, 'C');
        return false;
    }

    char next_tile= grid[nextx][nexty];

    if (!isTrackTile(next_tile)){
        train_status[trainindex]=TRAIN_CRASHED;
        metric_crashed++;
        std::cout<<"Traim"<<trainindex<<" crashed/ invalid tile"<<endl;
        logTrainTrace(currentTick, trainindex, currentx, currenty, currentdirection, 'C');
        return false;
    }
    //to figure out next direction taken by train
    int nextdirection= currentdirection;
    if (next_tile=='+'){
        nextdirection=getSmartDirectionAtCrossing(trainindex, nextx, nexty, currentdirection);
    }
    else {
        nextdirection= getNextDirection(next_tile, nextx, nexty, currentdirection);
    }
    // storing the next planned move
    train_nextx[trainindex]=nextx;
    train_nexty[trainindex]=nexty;
    train_nextdirection[trainindex]=nextdirection;
    train_plannedmove[trainindex]=true;

    //loop to handle safety tiles
    if (next_tile=='='){
        train_delaytimer[trainindex]=1;
        train_status[trainindex]=TRAIN_DELAYED;
        safetytiles++;
    }

    return true;

}

// ----------------------------------------------------------------------------
// GET NEXT DIRECTION based on current tile and direction
// ----------------------------------------------------------------------------
// Return new direction after entering the tile.
// ----------------------------------------------------------------------------
int getNextDirection(char tiletype, int x, int y, int currentdirection ) {
     //direction based on tile type
     //---straight tiles ki condition---//
    if (tiletype=='-' || tiletype=='|' || tiletype=='='|| tiletype=='S'||tiletype=='D'){
        return currentdirection;        //will keep movng in same direction
    }

    //-----curved tiles ki condition----//
    if (tiletype=='/') {
        if (currentdirection==DIR_LEFT){
            return DIR_DOWN;
        }
        else if (currentdirection==DIR_RIGHT){
            return DIR_UP;
        }
        else if (currentdirection==DIR_UP){
            return DIR_RIGHT;
        }
        else if (currentdirection==DIR_DOWN) {
            return DIR_LEFT;
            
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
    //crossings kei liye
    if ( tiletype=='+'){
        return currentdirection;
    }
    if (isSwitchTile(tiletype)){
        int switch_index= getSwitchIndex(tiletype);
        if (switch_index>=0 && switch_index<NumSwitches){
            int state= switch_currentState[switch_index];
            char exitdirection;
        
            if (state==0){
            exitdirection=switch_statelabel0[switch_index][0];}
            else {
                exitdirection=switch_statelabel1[switch_index][0];
            }

            if (exitdirection=='0') return DIR_UP;
            if (exitdirection=='1') return DIR_RIGHT;
            if (exitdirection=='2') return DIR_DOWN;
            if (exitdirection=='3') return DIR_LEFT;
        }

    }
    return currentdirection;
    }


}

// ----------------------------------------------------------------------------
// SMART ROUTING AT CROSSING - Route train to its matched destination
// ----------------------------------------------------------------------------
// Choose best direction at '+' toward destination.
// ----------------------------------------------------------------------------
int getSmartDirectionAtCrossing(int trainindex, int current_x, int current_y, int currentdirection) {
    int dest_x= train_destinationx[trainindex];
    int dest_y= train_destinationy[trainindex];

    int dx= dest_x-current_x;
    int dy= dest_y-current_y;

    if (abs(dx)>abs(dy)){
        if (dx>0 && currentdirection != DIR_UP) return DIR_DOWN;
        if (dx<0 && currentdirection != DIR_DOWN) return DIR_UP;

    }
    else {
        if (dy>0 && currentdirection != DIR_LEFT) return DIR_RIGHT;
       if (dy>0 && currentdirection != DIR_RIGHT) return DIR_LEFT; 
    }
    return currentdirection;
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
                std::cout<<"Train number"<<i<<"arrived at destination"<<endl;
                logTrainTrace(currentTick, i, train_x[i], train_y[i], train_direction[i], 'A');

            }
        }
    }
}

// ----------------------------------------------------------------------------
// DETERMINE ALL ROUTES (PHASE 2)
// ----------------------------------------------------------------------------
// Fill next positions/directions for all trains.
// ----------------------------------------------------------------------------
void determineAllRoutes() {
    for (int w=0; w<NumTrains;w++){
        if (train_status[w]==TRAIN_MOVING || train_status[w]==TRAIN_WAITING || train_status[w]==TRAIN_DELAYED){
            determineNextPosition(w);
        }
    }
}

// ----------------------------------------------------------------------------
// MOVE ALL TRAINS (PHASE 5)
// ----------------------------------------------------------------------------
// Move trains; resolve collisions and apply effects.
// ----------------------------------------------------------------------------
void moveAllTrains() {
    for (int i=0; i<NumTrains; i++){
        if (train_status[i]==TRAIN_CRASHED || train_status[i]==TRAIN_ARRIVED || train_status[i]==TRAIN_INACTIVE){
            continue;
        }
        if (train_status[i]==TRAIN_DELAYED){
            if (train_delaytimer[i]>0){
                train_delaytimer[i]--;
                logTrainTrace(currentTick, i, train_x[i], train_y[i], train_direction[i], 'D');
                if (train_delaytimer[i]==0){
                    train_status[i]=TRAIN_MOVING;
                }
                continue;
            }
        }
        if (train_status[i]==TRAIN_WAITING){
            logTrainTrace(currentTick, i, train_x[i], train_y[i], train_direction[i], 'W');
            train_status[i]=TRAIN_MOVING;
            continue;
        }
        if (train_plannedmove[i]){
            train_previousx[i]=train_x[i];
            train_previousy[i]=train_y[i];

            train_x[i]=train_nextx[i];
            train_y[i]=train_nexty[i];
            train_direction[i]= train_nextdirection[i];
            train_plannedmove[i]=false;
            logTrainTrace(currentTick, i, train_x[i], train_y[i], train_direction[i], 'M');
                
        }
    }
    checkArrivals();
    applyDeferredFlips();
}

// ----------------------------------------------------------------------------
// DETECT COLLISIONS WITH PRIORITY SYSTEM
// ----------------------------------------------------------------------------
// Resolve same-tile, swap, and crossing conflicts.
// ----------------------------------------------------------------------------
void detectCollisions() {
    //checking for ek hi jagah wali collisions
    for (int i=0; i<NumTrains; i++){
        if (!train_plannedmove[i]||train_status[i]==TRAIN_CRASHED){
            continue;
        }
        for (int j=i+1; j<NumTrains; j++){
            if (!train_plannedmove[j]||train_status[j]==TRAIN_CRASHED){
                continue;
        }
        if (train_nextx[i]==train_nextx[j] && train_nexty[i]==train_nexty[j]){
            int distanceI= calcdistance(i);
            int distanceJ= calcdistance(j);
            //train i further from destination than train j, higher priority
            if (distanceI>distanceJ){
                train_status[j]=TRAIN_WAITING;
                train_waitticks[j]++;
                totalWaitTicks++;
                train_plannedmove[j]=false;
                cout<<"Train"<<j<<" waiting as lower priority than Train"<< i<<endl;

            }
            else if (distanceJ>distanceI){
                train_status[i]=TRAIN_WAITING;
                train_waitticks[i]++;
                totalWaitTicks++;
                train_plannedmove[i]=false;
                cout<<"Train"<<i<<" waiting as lower priority than Train"<<j<<endl;

            }
            else {
                train_status[i]=TRAIN_CRASHED;
                train_status[j]=TRAIN_CRASHED;
                metric_crashed= metric_crashed+2;
                 metric_collisions++;
                cout<<"Trains "<<i<<" and "<< j<<"have crashed!"<<endl;
                logTrainTrace(currentTick, i, train_x[i], train_y[i], train_direction[i], 'C');
                logTrainTrace(currentTick, j, train_x[j], train_y[j], train_direction[j], 'C');
                train_plannedmove[i]=false;
                train_plannedmove[j]=false;
            }
        }
        if (train_nextx[i]==train_x[j]&& train_nexty[i]==train_y[j] && train_nextx[j]==train_x[i] && train_nexty[j]==train_y[i]){
            int distanceI=calcdistance(i);
            int distanceJ=calcdistance(j);

            if (distanceI>distanceJ){
                train_status[j]=TRAIN_WAITING;
                 train_waitticks[j]++;
                totalWaitTicks++;
                train_plannedmove[j]=false;
                cout<<"Train"<<j<<" waiting as lower priority than Train"<< i<<endl;              
            }
            else if (distanceJ>distanceI){
                train_status[i]=TRAIN_WAITING;
                train_waitticks[i]++;
                totalWaitTicks++;
                train_plannedmove[i]=false;
                cout<<"Train"<<i<<" waiting as lower priority than Train"<<j<<endl;}
            else {
                train_status[i]=TRAIN_CRASHED;
                train_status[j]=TRAIN_CRASHED;
                metric_crashed= metric_crashed+2;
                metric_collisions++;
                cout<<"Trains "<<i<<" and "<< j<<"have crashed!"<<endl;
                logTrainTrace(currentTick, i, train_x[i], train_y[i], train_direction[i], 'C');
                logTrainTrace(currentTick, j, train_x[j], train_y[j], train_direction[j], 'C');
                train_plannedmove[i]=false;
                train_plannedmove[j]=false;
            }
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
    int halt_x= emergencyhalt_x;
    int halt_y= emergencyhalt_y;

    //halt applied on trains in 3x3 zone (active zone)
    for (int i=0; i<NumTrains; i++){
        if (train_status[i]!=TRAIN_MOVING && train_status[i]!=TRAIN_DELAYED){
            continue;
        }
    int trainx= train_x[i];
    int trainy= train_y[i];

    int dx= abs(trainx- halt_x);
    int dy= abs(trainy- halt_y);

    //check to see if train within 3x3 zone
    if (dx<=1 && dy<=1){        //manhattan distance 2 sei kam ho ga ya equal to 2
        train_status[i]=TRAIN_WAITING;
        train_waitticks[i]++;
        cout<<"Train"<<i<<"halted by emergency halt!"<<endl;
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

