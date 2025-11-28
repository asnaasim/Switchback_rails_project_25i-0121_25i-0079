#ifndef SIMULATION_STATE_H
#define SIMULATION_STATE_H

// ============================================================================
// SIMULATION_STATE.H - Global constants and state
// ============================================================================
// Global constants and arrays used by the game.
// ============================================================================

// ----------------------------------------------------------------------------
// GRID CONSTANTS
const int max_rows=100;
const int max_cols=100;

// ----------------------------------------------------------------------------
// TRAIN CONSTANTS
const int max_trains=100;
const int max_colors=10;


//direction constants
const int DIR_UP=0;
const int DIR_RIGHT=1;
const int DIR_DOWN=2;
const int DIR_LEFT=3;

//TRAin states
const int TRAIN_INACTIVE=0;
const int TRAIN_WAITING=1;
const int TRAIN_MOVING=2;
const int TRAIN_CRASHED=3;
const int TRAIN_DELAYED=4;
const int TRAIN_ARRIVED=5;




// ----------------------------------------------------------------------------
// SWITCH CONSTANTS

const int max_switches=26; //26 as represents from a-z

//switch modes
const int SWITCH_MODE_PER_DIR=0;
const int SWITCH_MODE_GLOBAL=1;




// ----------------------------------------------------------------------------
// WEATHER CONSTANTS
    const int WEATHER_RAIN=0;
    const int WEATHER_NORMAL=1;
    const int WEATHER_FOG=2;

// ----------------------------------------------------------------------------
// SIGNAL CONSTANTS
const int signal_green=0;
const int signal_yellow=1;
const int signal_red=2;

//SPAWN DESTINATION CONSTANTss
const int MAX_SPAWN=20;
const int MAX_DESTINATION=20;



// ----------------------------------------------------------------------------
// GLOBAL STATE: GRID
extern int rows;
extern int cols;
extern char grid[max_rows][max_cols];
extern char originalGrid[max_rows][max_cols];


// ----------------------------------------------------------------------------
// GLOBAL STATE: TRAINS
// ----------------------------------------------------------------------------
extern int train_spawnx[max_trains];
extern int train_x[max_trains];  //x coordinate train ka
extern int train_y[max_trains];      //y coordinate train ka
extern int train_desty[max_trains];
extern int train_destx[max_trains];
extern int train_status[max_trains];       //crashed or not
extern int train_currentticks[max_trains];
extern int train_spawnticks[max_trains];
extern int train_waitticks[max_trains];
extern int train_direction[max_trains];
extern int train_ids[max_trains];          //DIFFERETIATINg between trains
extern int train_colorindex[max_trains];
extern int lag[max_trains];
extern int train_previousx[max_trains];



extern int NumTrains=0; 

// ----------------------------------------------------------------------------
// GLOBAL STATE: SWITCHES (A-Z mapped to 0-25)
 extern int NumSwitches=0;

    extern char switch_letter[max_switches];  //A, B, C se Z tk
    extern int switch_currentState[max_switches]; //1 agar active root, 0 agar nae
    extern char switch_statelabel1[max_switches][20];
    extern char switch_statelabel0[max_switches][20];
    
    //k vals show each direction wali cheez
    extern int switch_kvalues[max_switches][4];
    extern int currentState[max_switches];
    extern int initialState[max_switches];
    extern int mode[max_switches];
    //counters for each direction
    extern int switch_counters[max_switches][4];// 4 as up, down, left, right
    extern int switch_globalcounter[max_switches];  //for comparison

    extern bool switch_queuedtoflip[max_switches];
    extern int switch_signalcolour[max_switches];


// ----------------------------------------------------------------------------
// GLOBAL STATE: SPAWN POINTS
   const int MAX_SPAWN=50;

    extern int spawnx[MAX_SPAWN];     
    extern int spawny[MAX_SPAWN];      
    extern int sdirection[MAX_SPAWN];

        
    

    extern int spawnPointCount;
    extern int destPointCount;
    


// ----------------------------------------------------------------------------
// GLOBAL STATE: DESTINATION POINTS
const int MAX_DESTINATION=50;
extern int destPointCount;
extern int destx[MAX_DESTINATION];
extern int desty[MAX_DESTINATION];
extern int destid[MAX_DESTINATION];      

// ----------------------------------------------------------------------------
// GLOBAL STATE: SIMULATION PARAMETERS
// ----------------------------------------------------------------------------
extern int currentTick;
extern int seed;
extern int weathermode;
extern char levelName[100];

// ----------------------------------------------------------------------------
// GLOBAL STATE: METRICS
    extern int metric_delivered; 
    extern int metric_crashed; 
    extern int metric_collisions;
    extern int metric_totaltrains;
    extern int safetytiles;


// ----------------------------------------------------------------------------
// GLOBAL STATE: EMERGENCY HALT
// ----------------------------------------------------------------------------
extern bool emergencyhalt_active;
extern int emergencyhalt_x;     
extern int emergencyhalt_y;    
extern int emergencyhalt_timer;


// ----------------------------------------------------------------------------
// INITIALIZATION FUNCTION
// ----------------------------------------------------------------------------
// Resets all state before loading a new level.
void initializeSimulationState();

#endif
