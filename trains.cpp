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
// TRAINS.CPP
// ==========================================================================


//calculation of manhattan wla distance
int calcdistance(int trainindex){
    if (trainindex<0||trainindex>=max_trains){
        return 999999;}
    int dest_x=train_destinationx[trainindex];
    int dest_y=train_destinationy[trainindex];
    int current_x=train_x[trainindex];
    int current_y=train_y[trainindex];
    int distance=abs(dest_x-current_x)+abs(dest_y-current_y);
    return distance;
}

void spawnTrainsForTick() {
    for (int i=0; i<NumTrains; i++){
        if (train_status[i]==TRAIN_INACTIVE && train_spawnticks[i]==currentTick){
            int xcoordinate=train_x[i];
            int ycoordinate=train_y[i];
        
            if (!isInBounds(xcoordinate, ycoordinate)) {        //check to see if trains coming on grids or not 
                std::cout<<"ERROR: Train " <<i<< " spawn at (" <<xcoordinate<<"," 
                          <<ycoordinate<< ") is OUT OF BOUNDS!" << endl;
                train_status[i]=TRAIN_CRASHED;
                metric_crashed++;
                continue;        //if not on track, tou crash ho jaye ga
            }
            bool spacetaken = false;
            for (int j=0; j<NumTrains; j++){
                if (i== j) continue;           
                if (train_status[j]==TRAIN_MOVING || train_status[j]==TRAIN_WAITING || train_status[j]==TRAIN_DELAYED){
                    if (train_x[j]==xcoordinate && train_y[j]==ycoordinate){
                        spacetaken = true;
                        break;
                    }         //check to see agar train occupying space or not
                }
            }
            if (!spacetaken){
                if (train_destinationx[i]==0 && train_destinationy[i]==0 && destPointCount>0) {
                    int minDist=999999;
                    int closestDest=-1;
                    for (int d= 0; d<destPointCount; d++) {
                        int dist= abs(xcoordinate - destx[d]) +abs(ycoordinate - desty[d]);
                        if (dist<minDist){ 
                            minDist=dist;
                            closestDest=d;}
                    }
                    if (closestDest!=-1){
                        train_destinationx[i]=destx[closestDest];
                        train_destinationy[i]=desty[closestDest];
                    }
                }
                train_status[i] =TRAIN_MOVING;
                train_previousx[i]=xcoordinate;
                train_previousy[i]=ycoordinate;
                metric_totaltrains++;
                std::cout <<"New train " << i << " spawned at (" << xcoordinate << "," << ycoordinate << ")";
                std::cout <<" heading to (" << train_destinationx[i] << "," << train_destinationy[i] << ")";
                std::cout <<" distance: " << calcdistance(i) << endl;
                
                logTrainTrace(currentTick, i, xcoordinate, ycoordinate, train_direction[i], 'M');
            }
            else {
                train_spawnticks[i]++;
                std::cout << "Train " << i << " spawn delayed to Tick " << train_spawnticks[i] << endl;
            }
        }
    }
}

bool determineNextPosition(int trainindex) {
    if (train_status[trainindex]!=TRAIN_MOVING && train_status[trainindex]!=TRAIN_DELAYED) {
        train_plannedmove[trainindex]=false;
        return false;}
    
    int currentx = train_x[trainindex];
    int currenty =train_y[trainindex];
    int currentdirection = train_direction[trainindex];
    
    //to see agar position valid or not 
    if (!isInBounds(currentx, currenty)) {
        train_status[trainindex]=TRAIN_CRASHED;
        metric_crashed++;
        std::cout<<"Train " <<trainindex<< " crashed (current position out of bounds)"<<endl;
        logTrainTrace(currentTick, trainindex, currentx, currenty, currentdirection, 'C');
        return false;
    }
    
    int nextx = currentx;
    int nexty = currenty;
    if (currentdirection==DIR_UP) {
        nextx= currentx-1;
    } else if (currentdirection == DIR_RIGHT) {
        nexty = currenty+1;
    } else if (currentdirection == DIR_DOWN) {
        nextx = currentx+1;
    } else if (currentdirection == DIR_LEFT) {
        nexty = currenty-1;
    }

    // CRITICAL FIX: Check bounds BEFORE accessing grid
    if (!isInBounds(nextx, nexty)) {
        train_status[trainindex] = TRAIN_CRASHED;
        metric_crashed++;
        std::cout << "Train " << trainindex << " crashed (Out of Bounds) - tried to move from (" 
                  << currentx << "," << currenty << ") to (" << nextx << "," << nexty << ")" << endl;
        logTrainTrace(currentTick, trainindex, currentx, currenty, currentdirection, 'C');
        return false;
    }

    char next_tile=grid[nextx][nexty];
    
    std::cout <<"Train " << trainindex << " at (" << currentx << "," << currenty 
              <<") dir=" << currentdirection << " checking (" << nextx << "," << nexty 
              << ") tile='" << next_tile << "'" << endl;

    if (!isTrackTile(next_tile)) {
        train_status[trainindex] = TRAIN_CRASHED;
        metric_crashed++;
        std::cout << "Train " << trainindex << " crashed (Invalid Tile: '" << next_tile 
                  << "' at " << nextx << "," << nexty << ")" << endl;
        logTrainTrace(currentTick, trainindex, currentx, currenty, currentdirection, 'C');
        return false;
    }
    
    int nextdirection = currentdirection;
    if (next_tile == '+') {
        nextdirection = getSmartDirectionAtCrossing(trainindex, nextx, nexty, currentdirection);
    } else {
        nextdirection = getNextDirection(next_tile, nextx, nexty, currentdirection);
    }
    
    train_nextx[trainindex] = nextx;
    train_nexty[trainindex] = nexty;
    train_nextdirection[trainindex] = nextdirection;
    train_plannedmove[trainindex] = true;

    if (next_tile == '=') {
        if (train_status[trainindex] != TRAIN_DELAYED) {
            train_delaytimer[trainindex] = 1;
            train_status[trainindex] = TRAIN_DELAYED;
        }
    }
    
    return true;
}

int getNextDirection(char tiletype, int x, int y, int currentdirection ) {
     (void)x;
     (void)y;
     
    if (tiletype == '-' || tiletype == '|' || tiletype == '=' || tiletype == 'S' || tiletype == 'D') {
        return currentdirection;}

    if (tiletype == '/') {
        if (currentdirection == DIR_LEFT)   return DIR_DOWN;
        if (currentdirection == DIR_RIGHT)  return DIR_UP;
        if (currentdirection == DIR_UP)     return DIR_RIGHT;
        if (currentdirection == DIR_DOWN)   return DIR_LEFT;
    }
    if (tiletype == '\\') {
        if (currentdirection == DIR_LEFT)   return DIR_UP;
        if (currentdirection == DIR_RIGHT)  return DIR_DOWN;
        if (currentdirection == DIR_UP)     return DIR_LEFT;
        if (currentdirection == DIR_DOWN)   return DIR_RIGHT;
    }
    
    if (isSwitchTile(tiletype)) {
        int switch_index = getSwitchIndex(tiletype);
        if (switch_index >= 0 && switch_index < NumSwitches) {
            int state = switch_currentState[switch_index];
            char exit_direction_char;
        
            if (state==0){
                exit_direction_char = switch_statelabel0[switch_index][0];} 
            else {
                exit_direction_char=switch_statelabel1[switch_index][0];
            }

            if (exit_direction_char >= '0' && exit_direction_char <= '3') {
                return exit_direction_char - '0';
            }
            
            if (exit_direction_char=='1') return DIR_RIGHT;
            if (exit_direction_char=='2') return DIR_DOWN;
        }
    }
    
    return currentdirection;
}

int getSmartDirectionAtCrossing(int trainindex, int current_x, int current_y, int currentdirection) {
    int dest_x=train_destinationx[trainindex];
    int dest_y=train_destinationy[trainindex];

    int dx = dest_x - current_x;
    int dy = dest_y - current_y;

    if (abs(dx) > abs(dy)) {
        if (dx > 0 && currentdirection != DIR_UP) {
            return DIR_DOWN;
        }
        if (dx < 0 && currentdirection != DIR_DOWN) {
            return DIR_UP;
        }
    }
    
    if (abs(dy) >= abs(dx)) {
        if (dy>0 && currentdirection!=DIR_LEFT) {
            return DIR_RIGHT;}
        if (dy<0 && currentdirection!=DIR_RIGHT) {
            return DIR_LEFT;}
    }
    return currentdirection;
}

void checkArrivals(){
    for (int i=0; i<NumTrains; i++){
        if (train_status[i]==TRAIN_MOVING){ 
            if (train_x[i]==train_destinationx[i] && train_y[i]==train_destinationy[i]){
                train_status[i]=TRAIN_ARRIVED;
                metric_delivered++;
                std::cout<<"Train "<<i<<" ARRIVED at (" << train_x[i] << "," << train_y[i] << ")" << endl;
                logTrainTrace(currentTick, i, train_x[i], train_y[i], train_direction[i], 'A');
            }
        }
    }
}

void determineAllRoutes() {
    for (int w=0; w<NumTrains;w++){
        if (train_status[w] == TRAIN_MOVING || train_status[w] == TRAIN_DELAYED){
            determineNextPosition(w);
        } else {
            train_plannedmove[w]=false;
        }
    }
}

void moveAllTrains() {
    for (int i=0; i<NumTrains; i++){
        if (train_status[i]==TRAIN_CRASHED || train_status[i]==TRAIN_ARRIVED || train_status[i]==TRAIN_INACTIVE){
            continue;
        }
        
        if (train_status[i] == TRAIN_DELAYED){
            if (train_delaytimer[i]>0){
                train_delaytimer[i]--;
                logTrainTrace(currentTick, i, train_x[i], train_y[i], train_direction[i], 'D');
                
                if (train_delaytimer[i]=0){
                    train_status[i]=TRAIN_MOVING;
                }
                continue;
            }
        }
        
        if (train_status[i] == TRAIN_WAITING){
            logTrainTrace(currentTick, i, train_x[i], train_y[i], train_direction[i], 'W');
            train_status[i] = TRAIN_MOVING;
            continue;
        }

        if (train_plannedmove[i]){
            train_previousx[i]=train_x[i];
            train_previousy[i]=train_y[i];
            train_x[i]=train_nextx[i];
            train_y[i]=train_nexty[i];
            train_direction[i]=train_nextdirection[i];
            train_plannedmove[i]=false;
            
            logTrainTrace(currentTick, i, train_x[i], train_y[i], train_direction[i], 'M');
        }
    }
    checkArrivals();
}
//trains ki collisions detect
void detectCollisions(){
    for (int i = 0; i < NumTrains; i++) {
        if (!train_plannedmove[i] || train_status[i]==TRAIN_CRASHED) {
            continue;
        }
        for (int j = i + 1; j < NumTrains; j++) {
            if (!train_plannedmove[j] || train_status[j]==TRAIN_CRASHED) {
                continue;
            }
            
            bool collision_detected = false;
            
            if (train_nextx[i]==train_nextx[j] && train_nexty[i] == train_nexty[j]) {
                collision_detected=true;
            }
            
            if (train_nextx[i]==train_x[j] && train_nexty[i]==train_y[j] &&
                train_nextx[j]==train_x[i] && train_nexty[j]==train_y[i]) {
                collision_detected=true;
            }

            if (collision_detected){
                int distanceI=calcdistance(i);
                int distanceJ=calcdistance(j);
                // the priority wali cheez
                if (distanceI > distanceJ) {
                    train_status[j] = TRAIN_WAITING;
                    train_waitticks[j]++;        //train further away from destination, to move towards the destination due to priority
                    totalWaitTicks++;
                    train_plannedmove[j] = false;
                    cout << "Collision: Train " << j << " waiting" << endl;
                }
                else if (distanceJ > distanceI) {
                    train_status[i] =TRAIN_WAITING;
                    train_waitticks[i]++;
                    totalWaitTicks++;                //check for other train
                    train_plannedmove[i]=false;
                    cout<<"Collision: Train"<<i<<" waiting"<< endl;
                }
                else {
                    train_status[j]=TRAIN_WAITING;
                    train_waitticks[j]++;
                    totalWaitTicks++;            //trains coming saath, collision
                    train_plannedmove[j]=false;
                    cout << "Collision: Train " << j << "waiting (tie-breaker)" << endl;
                }
            }
        }
    }
}
//
void applyEmergencyHalt(){
    if (!emergencyhalt_active){
        return;}
    int halt_x=emergencyhalt_x;
    int halt_y=emergencyhalt_y;
     for (int i=0; i<NumTrains; i++){
        if (train_status[i] !=TRAIN_MOVING && train_status[i]!=TRAIN_DELAYED){
            continue;
        }
        
        int trainx=train_x[i];
        int trainy=train_y[i];
        int dx=abs(trainx- halt_x);
        int dy=abs(trainy- halt_y);
        if (dx<=1 && dy<=1){ 
            if (train_status[i]==TRAIN_MOVING) {
                train_status[i]=TRAIN_WAITING;
                train_waitticks[i]++;
                train_plannedmove[i]=false; 
                cout<<"Train"<<i<<"halted by emergency halt!"<<endl;
            }
        }
    }
}
// emergency halt ko activate ya deactivate karey ga
void updateEmergencyHalt(){            
    if (emergencyhalt_active==true){
        emergencyhalt_timer--;
        if (emergencyhalt_timer<=0){
            emergencyhalt_active=false;
            cout<<"Emergency halt disabled"<<endl;
        }
    }
}
