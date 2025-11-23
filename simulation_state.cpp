#include "simulation_state.h"
#include <cstring>
#include <iostream>
#include <string>
using namespace std;

// GRID wala code
const int MAX_ROWS = 100;
const int MAX_COLS = 100;
int rows = 0;  
int cols = 0;                           //making it generic sa
char grids[MAX_ROWS][MAX_COLS];
// TRAINS
const int NumTrains=100;
struct trains{
    int currentx, currenty; //current coordinates
    int destinationX, destinationY; //jahan the train has to reach/ D wali D can be read iske andar
    bool status; // wheather train reached or not
    bool crash; // true for agar it crashed, false agar it never crashed
    int spawnTicks;
    int direction;
    int trainid;
    int SigColor;
};

trains Train[NumTrains];
int traincount=0; //total num of trains jo from 0
    
// SWITCHES
const int NumSwitches=26;
struct switches{
    char letter;  //A, B, C se Z tk
    string mode;
    int currentState; //1 agar active root, 0 agar nae
    int initialState;
    
    //k vals show each direction wali cheez
    int kUp;
    int kRight;
    int kDown;
    int kLeft;
    int currentState;
    int initialState
    string mode;
    //counters for each direction
    int counterUp=0;
    int counterRight=0;
    int counterDown=0;
    int counterLeft=0;
    bool queuedToFlip;};
switches Switch[NumSwitches];


// SPAWN AND DESTINATION POINTS KE LIYA WE USE CONST CUZ RUNIME PE CANT BE DEFINED
    const int MAX_SPAWN=50;
    const int MAX_DESTINATION=50;

    struct spawnPoint 
    {
        int xl
        int y;
        int direction;
    };

    struct destPoint
     {
        int x
        int y;
        char id;  //will help differentate trains going to multiple destinations
    };

    spawnPoint SpawnPoints[MAX_SPAWN];
    int spawnPointCount=0;
    destPoint DestPoints[MAX_DESTINATION];
    int destPointCount=0;

// SIMULATION PARAMETERS
// METRICS
struct metrics
{
    int delivered=0; //num of trains delivered
    int crashed=0; //num of trains that crashed
    int collisions=0;//num of accidents
    int totaltrains=0; //total number of trains that were spawned
};
metrics metric;
struct emergencyHalt
 {
    bool active=false;           //emergency halt active or not?
};

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
    for (int i=0; i<rows; i++){
        for (int j=0; j<cols; j++){
            grid[i][j] = ' ';
        }}
    
    //initializing trains
    while (int i=0<NumTrains){
        Train[i].currentx = 0;
        Train[i].currenty = 0;
        Train[i].destinationX = 0;
        Train[i].destinationY = 0;
        Train[i].status = false;  // Not active
        Train[i].crash = false;
        Train[i].spawnTicks = 0;
        Train[i].direction = 0;
        Train[i].trainid = i;
        i++;                    //cuz iter through poori array takay all trains items can be intialised
    }
    int traincount = 0;
    
    for (int i = 0; i < NumSwitches; i++) 
    {
        Switch[i].letter = '\0';
        Switch[i].currentState = 0;
        Switch[i].initialState = 0;
        Switch[i].kUp = 0;
        Switch[i].kRight = 0;
        Switch[i].kDown = 0;
        Switch[i].kLeft = 0;
        Switch[i].counterUp = 0;
        Switch[i].counterRight = 0;
        Switch[i].counterDown = 0;
        Switch[i].counterLeft = 0;
        Switch[i].queuedToFlip = false;
    }
    for (int i = 0; i < spawn; i++) {
        SpawnPoints[i].x = 0;
        SpawnPoints[i].y = 0;
        SpawnPoints[i].direction = 0;
    }
    int spawnPointCount = 0;
    
    //initializing dest ke points
    for (int i = 0; i < destination; i++) {
        DestPoints[i].x = 0;
        DestPoints[i].y = 0;
        DestPoints[i].id = '\0';
    }
    int destPointCount = 0;}
