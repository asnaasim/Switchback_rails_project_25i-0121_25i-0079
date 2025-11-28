#include "simulation_state.h"
#include <cstring>
#include <iostream>
#include <string>
using namespace std;

// GRID wala code

int rows = 0;  
int cols = 0;                           //making it generic sa
char grids[max_rows][max_cols];    
char originalGrid[max_rows][max_cols];

// TRAINS
int NumTrains=0; //total num of trains jo from 0

int train_x[max_trains];  //current x coordinate train ka
int train_y[max_trains];      //y coordinate train ka
int train_desty[max_trains];  //arrays for destination point
int train_destx[max_trains];
int train_spawnx[max_trains];
int train_spawny[max_trains];
int train_status[max_trains];       //crashed or not, w
int train_currentticks[max_trains];
int train_spawnticks[max_trains];
int train_waitticks[max_trains];
int train_direction[max_trains];
int train_ids[max_trains];          //DIFFERETIATINg between trains
int train_colorindex[max_trains];
int lag[max_trains];        
int train_previousx[max_trains];
int train_previousy[max_trains];  
int train_spawnx[max_trains];
int train_spawny[max_trains];

int currentTick=0;
    
// SWITCHES
 int NumSwitches=0;

    char switch_letter[max_switches];  //A, B, C se Z tk
    int switch_currentState[max_switches]; //1 agar active root, 0 agar nae
    char switch_statelabel1[max_switches][20];
    char switch_statelabel0[max_switches][20];
    
    //k vals show each direction wali cheez
    int switch_kvalues[max_switches][4];
    int mode[max_switches];
    //counters for each direction
    int switch_counters[max_switches][4];// 4 as up, down, left, right
    int switch_globalcounter[max_switches];  //for comparison

    bool switch_queuedtoflip[max_switches];
    int switch_signalcolour[max_switches];


// SPAWN AND DESTINATION POINTS KE LIYA WE USE CONST CUZ RUNIME PE CANT BE DEFINED

        int spawnx[MAX_SPAWN];      //x coordinates of spawn
        int spawny[MAX_SPAWN];      //y coordinates of spawn
        int sdirection[MAX_SPAWN];

        int destx[MAX_DESTINATION];
        int desty[MAX_DESTINATION];
        int destid[MAX_DESTINATION];        //will help differentate trains going to multiple destinations
    

    int spawnPointCount=0;
    int destPointCount=0;

// SIMULATION PARAMETERS
// METRICS


    int metric_delivered=0; //num of trains delivered
    int metric_crashed=0; //num of trains that crashed
    int metric_collisions=0;//num of accidents
    int metric_totaltrains=0; //total number of trains that were spawned

    bool emergencyhalt_active=false;           //emergency halt active or not?
    int emergencyhalt_x=0;              //specific coordinates where train taking a halt
    int emergencyhalt_y=0;
    int emergencyhalt_timer=0;

    int safetytiles=0;

    int currentTick=0;
    int seed=0;
    int weathermode=0;
    char levelName[100];


// ============================================================================
// INITIALIZE SIMULATION STATE
// ============================================================================
// ----------------------------------------------------------------------------
// Resets all global simulation state.
// ----------------------------------------------------------------------------
// Called before loading a new level.
// ----------------------------------------------------------------------------
void initializeSimulationState() 
{
    //initializinng the grid
    rows=0;
    cols=0;      //setting our rows and columns to zero
    for (int i=0; i<rows; i++){
        for (int j=0; j<cols; j++){
            grids[i][j] = '.';
            originalGrid[i][j] = '.';
        }}
    
    //initializing trains
    NumTrains=0;
    while (int i=0<NumTrains){
        train_x[max_trains]=0;  //current x coordinate train ka
        train_y[max_trains]=0;      //y coordinate train ka
        train_desty[max_trains]=0;  //arrays for destination point
        train_destx[max_trains]=0;
        train_spawnx[max_trains]=0;
        train_spawny[max_trains]=0;
        train_status[max_trains]=0;       //crashed or not, w
        train_currentticks[max_trains]=0;
        train_spawnticks[max_trains]=0;
        train_waitticks[max_trains]=0;
        train_direction[max_trains]=0;
        train_ids[max_trains]=0;          //DIFFERETIATINg between trains
        train_colorindex[max_trains]=0;
        lag[max_trains]=0;        
        train_previousx[max_trains]=0;
        train_previousy[max_trains]=0;  
        train_spawnx[max_trains]=0;
        train_spawny[max_trains]=0;
        i++;                    //cuz iter through poori array takay all trains items can be intialised
    }
    int traincount = 0;
    
    //initializing all the switche
    for (int i = 0; i < NumSwitches; i++) 
    {
        switch_letter[i]= '\0';
        switch_currentState[i] = 0;
        switch_statelabel1[i][0]='/0';
        switch_statelabel0[i][0]='/0';
        switch_globalcounter[i]=0;
        switch_queuedtoflip[i] = false;
        for (int j=0; j<4; j++){
            switch_kvalues[i][j]=1;
            switch_counters[i][j]=0;
        }
    }
    for (int i = 0; i < MAX_SPAWN; i++) {
        spawnx[i] = 0;
        spawny[i] = 0;
        sdirection[i] = DIR_RIGHT;
    }
    
    //initializing dest ke points
    for (int i = 0; i < MAX_DESTINATION; i++) {
        destx[i] = 0;
        desty[i] = 0;

    }
    destPointCount = 0;
    bool emergencyhalt_active=false;          
    emergencyhalt_x=0;              
    emergencyhalt_y=0;
    emergencyhalt_timer=0;
    //overall metric initializing
    metric_delivered=0; 
    metric_crashed=0; 
    metric_collisions=0;
    metric_totaltrains=0;

    //variables that are not arrays
    currentTick=0;
    seed=0;
    levelName[0]='\0';
    spawnPointCount=0;
    destPointCount=0;

    safetytiles=0;


}

