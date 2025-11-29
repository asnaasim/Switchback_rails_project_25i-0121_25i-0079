#include "simulation_state.h"
#include <cstring>
#include <iostream>
#include <string>
using namespace std;

// GRID WALI
int rows = 0;
int cols = 0;

char grid[max_rows][max_cols];
char originalGrid[max_rows][max_cols];

// TRAINS INTIALISATION
int NumTrains = 0;

// Train coordinates alag alah
int train_previousx[max_trains];
int train_previousy[max_trains];
int train_x[max_trains];
int train_y[max_trains];
int train_nextx[max_trains];
int train_nexty[max_trains];
int train_destinationx[max_trains];
int train_destinationy[max_trains];

// Train directions
int train_direction[max_trains];
int train_nextdirection[max_trains];
bool train_plannedmove[max_trains];

// Train status
int train_status[max_trains];
int train_spawnticks[max_trains];
int train_waitticks[max_trains];

// Train general things
int train_colorindex[max_trains];
int train_delaytimer[max_trains];
int Train_ids[max_trains];

// SWITCHES
int NumSwitches = 0;

char switch_letter[max_switches];
int mode[max_switches];
int switch_x[max_switches];
int switch_y[max_switches];

int switch_currentState[max_switches];
char switch_statelabel1[max_switches][20];
char switch_statelabel0[max_switches][20];

int switch_kvalues[max_switches][4];
int switch_counters[max_switches][4];
int switch_globalcounter[max_switches];

bool switch_queuedtoflip[max_switches];
int switch_signalcolour[max_switches];

// SPAWN AND DEST POINTS
int spawnPointCount = 0;
int destPointCount = 0;

int spawnx[MAX_SPAWN];
int spawny[MAX_SPAWN];
int sdirection[MAX_SPAWN];

int destx[MAX_DESTINATION];
int desty[MAX_DESTINATION];
int destid[MAX_DESTINATION];


int currentTick = 0;
int seed = 0;
int weatherMode = WEATHER_NORMAL;
char levelName[100];
int safetytiles = 0;

// METRICS TO BE SHOWN IN OUT DIRECTORY
int metric_delivered = 0;
int metric_crashed = 0;
int metric_collisions = 0;
int metric_totaltrains = 0;

bool emergencyhalt_active = false;
int emergencyhalt_x = 0;
int emergencyhalt_y = 0;
int emergencyhalt_timer = 0;

int totalSwitchFlips = 0;
int signalViolations = 0;
int totalWaitTicks = 0;

int signallights[50];

void initializeSimulationState()
{
    // Initialization of the grid
    rows = 0;
    cols = 0;
    int i = 0;
    while (i < max_rows)
    {
        int j = 0;
        while (j < max_cols)
        {
            grid[i][j] = '.';
            originalGrid[i][j] = '.';
            j++;
        }
        i++;
    }

    // Initialization of train ke arrays
    NumTrains = 0;
    i = 0;
    while (i < max_trains)
    {
        train_x[i] = 0;
        train_y[i] = 0;
        train_destinationx[i] = 0;
        train_destinationy[i] = 0;
        train_previousx[i] = 0;
        train_previousy[i] = 0;
        train_nextx[i] = 0;
        train_nexty[i] = 0;
        Train_ids[i] = 0;
        train_direction[i] = 0;
        train_nextdirection[i] = DIR_RIGHT;
        train_plannedmove[i] = false;
        train_waitticks[i] = 0;
        train_spawnticks[i] = 0;
        train_colorindex[i] = 0;
        train_delaytimer[i] = 0;
        train_status[i] = TRAIN_INACTIVE;
        i++;
    }

    // Initialization of the switches walay arrays
    NumSwitches = 0;
    i = 0;
    while (i < max_switches)
    {
        switch_letter[i] = '\0';
        switch_currentState[i] = 0;
        switch_statelabel1[i][0] = '\0';
        switch_statelabel0[i][0] = '\0';
        switch_globalcounter[i] = 0;
        switch_signalcolour[i] = 0;
        switch_x[i] = 0;
        switch_y[i] = 0;
        mode[i] = 0;
        switch_queuedtoflip[i] = false;

        int j = 0;
        while (j < 4)
        {
            switch_kvalues[i][j] = 1;
            switch_counters[i][j] = 0;
            j++;
        }
        i++;
    }

    // Initialization of spawn points
    spawnPointCount = 0;
    i = 0;
    while (i < MAX_SPAWN)
    {
        spawnx[i] = 0;
        spawny[i] = 0;
        sdirection[i] = DIR_RIGHT;
        i++;
    }

    // Initialization of destination points
    destPointCount = 0;
    i = 0;
    while (i < MAX_DESTINATION)
    {
        destx[i] = 0;
        desty[i] = 0;
        destid[i] = 0;
        i++;
    }

    // Initialization of emergency halt
    emergencyhalt_active = false;
    emergencyhalt_x = 0;
    emergencyhalt_y = 0;
    emergencyhalt_timer = 0;

    // Initialized walay metrics
    metric_delivered = 0;
    metric_crashed = 0;
    metric_collisions = 0;
    metric_totaltrains = 0;
    totalSwitchFlips = 0;
    signalViolations = 0;
    totalWaitTicks = 0;

    // Initialization of simulation walay parameters
    currentTick = 0;
    seed = 0;
    levelName[0] = '\0';
    safetytiles = 0;
    weatherMode = WEATHER_NORMAL;

    // Initialization for the signal lights
    i = 0;
    while (i < 50)
    {
        signallights[i] = 0;
        i++;
    }
}
