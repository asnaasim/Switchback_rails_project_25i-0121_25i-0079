#include "simulation_state.h"
#include <cstring>
#include <iostream>
#include <string>
using namespace std;

// GRID wala code

int rows = 0;  
int cols = 0;                           //making it generic sa


char grid[max_rows][max_cols];    
char originalGrid[max_rows][max_cols];

//________TRAINS_________//
int NumTrains=0;                        //total num of trains jo from 0
//----All possible coordinates of train-----//
//previous coordinates
int train_previousx[max_trains];        
int train_previousy[max_trains];  
//current coordinates
int train_x[max_trains];                    
int train_y[max_trains];    
//next coordinates      
int train_nextx[max_trains];   
int train_nexty[max_trains];       
//destination coordinates    
int train_destinationx[max_trains];
int train_destinationy[max_trains];

//<===Possible Directions=======>//
//direction of train (current wali)
int train_direction[max_trains];
//next direction that itll take
int train_nextdirection[max_trains];
bool train_plannedmove[max_trains];

//---tRAIN STATUS----//
int train_status[max_trains];       //moving, waiting, crashed, arrived
//----possible ticks---//
int train_spawnticks[max_trains];  
int train_waitticks[max_trains];   

//---general---//
int train_colorindex[max_trains];     //differentiating trains 
int train_delaytimer[max_trains];
int Train_ids[max_trains];
    
// SWITCHES
 int NumSwitches=0;
//<---basic switches--->//
    char switch_letter[max_switches];  //A, B, C se Z tk
    int mode[max_switches];
    int switch_x[max_switches];
    int switch_y[max_switches];

//<--possible states---->//
    int switch_currentState[max_switches]; //1 agar active root, 0 agar nae
    char switch_statelabel1[max_switches][20];
    char switch_statelabel0[max_switches][20];

//--k-values show each direction
    int switch_kvalues[max_switches][4];

//counters for each direction
    int switch_counters[max_switches][4];// 4 as up, down, left, right
    int switch_globalcounter[max_switches];  //for comparison

    bool switch_queuedtoflip[max_switches];
    int switch_signalcolour[max_switches];


// SPAWN AND DESTINATION POINTS KE LIYA WE USE CONST CUZ RUNIME PE CANT BE DEFINED
    int spawnPointCount=0;
    int destPointCount=0;

    //spawn points
        int spawnx[MAX_SPAWN];      //x coordinates of spawn
        int spawny[MAX_SPAWN];      //y coordinates of spawn
        int sdirection[MAX_SPAWN];
    //destination points
        int destx[MAX_DESTINATION];
        int desty[MAX_DESTINATION];
        int destid[MAX_DESTINATION];        //will help differentate trains going to multiple destinations
    
// SIMULATION PARAMETERS
//------------------------------------
int currentTick=0;
int seed=0;
int weatherMode=  WEATHER_NORMAL;
char levelName[100];
int safetytiles=0;

// METRICS
    int metric_delivered=0;     //num of trains delivered
    int metric_crashed=0;   //num of trains that crashed
    int metric_collisions=0;    //num of accidents
    int metric_totaltrains=0;   //total number of trains that were spawned

    bool emergencyhalt_active=false;           //emergency halt active or not?
    int emergencyhalt_x=0;              //specific coordinates where train taking a halt
    int emergencyhalt_y=0;
    int emergencyhalt_timer=0;

    int totalSwitchFlips=0;
    int signalViolations=0;
    int totalWaitTicks=0; 

    int signallights[50];


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
    int i=0;
    while (i<max_rows){
        int j=0;
        while (j<max_cols){
            grid[i][j] = '.';
            originalGrid[i][j] = '.';
            j++;
            
        }
        i++;
    }
    
    //initializing trains
    NumTrains=0;
    i=0;
    while (i<max_trains){
        train_x[i]=0;
        train_y[i]=0;
        train_destinationx[i]=0;
        train_destinationy[i]=0;
        train_previousx[i]=0;
        train_previousy[i]=0;
        train_nextx[i]=0;
        train_nexty[i]=0;
        train_direction[i]=0;
        train_nextdirection[i]=DIR_RIGHT;
        train_plannedmove[i]=false;
        train_waitticks[i]=0;
        train_spawnticks[i]=0;
        Train_ids[i]=0;
        train_colorindex[i]=0;
        train_direction[i]=0;
        train_status[i]= TRAIN_INACTIVE;
        i++;
    }
    
    //initializing all the switches
    NumSwitches=0;
    i=0;
    while (i<max_switches)
    {
        switch_letter[i]= '\0';
        switch_x[i]=0;
        switch_y[i]=0;
        switch_currentState[i] = 0;
        switch_statelabel1[i][0]='\0';
        switch_statelabel0[i][0]='\0';
        switch_globalcounter[i]=0;
        switch_signalcolour[i]=0;
        mode[i]=0;
        switch_queuedtoflip[i] = false;
        int j=0;
        while (j<4){
            switch_kvalues[i][j]=1;
            switch_counters[i][j]=0;
            j++;
        }
        i++;
    }

    //destination nd spawn points
    destPointCount=0;
    spawnPointCount=0;
    i=0;
    while ( i < MAX_SPAWN ) {
        spawnx[i] = 0;
        spawny[i] = 0;
        sdirection[i] = DIR_RIGHT;
        i++;
    }
    i=0;
    //initializing dest ke points
    while ( i < MAX_DESTINATION) {
        destx[i] = 0;
        desty[i] = 0;
        destid[i]=0;
        i++;

    }

    emergencyhalt_active=false;          
    emergencyhalt_x=0;              
    emergencyhalt_y=0;
    emergencyhalt_timer=0;

    //overall metric initializing
    metric_delivered=0; 
    metric_crashed=0; 
    metric_collisions=0;
    metric_totaltrains=0;
    totalSwitchFlips=0;
    signalViolations=0;
    totalWaitTicks=0;

    //variables that are not arrays
    currentTick=0;
    seed=0;
    levelName[0]='\0';
    safetytiles=0;
    weatherMode= WEATHER_NORMAL;

    i=0;
    while ( i<50){
        signallights[i]=0;
        i++;
    }
}
