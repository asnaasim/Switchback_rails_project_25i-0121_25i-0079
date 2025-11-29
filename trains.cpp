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
