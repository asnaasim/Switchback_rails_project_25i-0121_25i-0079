#ifndef SIMULATION_STATE_H
#define SIMULATION_STATE_H


const int max_rows = 70;
const int max_cols = 70;

const int max_trains = 100;
const int max_colors = 10;


const int DIR_UP = 0;
const int DIR_RIGHT = 1;
const int DIR_DOWN = 2;
const int DIR_LEFT = 3;

// Trainstates
const int TRAIN_INACTIVE = 0;
const int TRAIN_WAITING = 1;
const int TRAIN_MOVING = 2;
const int TRAIN_CRASHED = 3;
const int TRAIN_DELAYED = 4;
const int TRAIN_ARRIVED = 5;

const int max_switches = 26;

// Switch modes
const int SWITCH_MODE_PER_DIR = 0;
const int SWITCH_MODE_GLOBAL = 1;
// ----------------------------------------------------------------------------
// WEATHER CONSTANTS
// ----------------------------------------------------------------------------
const int WEATHER_RAIN = 0;
const int WEATHER_NORMAL = 1;
const int WEATHER_FOG = 2;

// ----------------------------------------------------------------------------
// SIGNAL CONSTANTS
// ----------------------------------------------------------------------------
const int GREEN = 0;
const int YELLOW = 1;
const int RED = 2;

const int MAX_SPAWN = 20;
const int MAX_DESTINATION = 20;

extern int rows;
extern int cols;
extern char grid[max_rows][max_cols];
extern char originalGrid[max_rows][max_cols];

extern int NumTrains;


extern int train_x[max_trains];
extern int train_y[max_trains];
extern int train_nextx[max_trains];
extern int train_nexty[max_trains];
extern int train_previousy[max_trains];
extern int train_previousx[max_trains];
extern int train_destinationx[max_trains];
extern int train_destinationy[max_trains];


extern int train_status[max_trains];
extern int train_spawnticks[max_trains];
extern int train_waitticks[max_trains];


extern int train_direction[max_trains];
extern int train_nextdirection[max_trains];
extern bool train_plannedmove[max_trains];


extern int train_colorindex[max_trains];
extern int train_delaytimer[max_trains];
extern int Train_ids[max_trains];


extern int NumSwitches;

extern char switch_letter[max_switches];
extern int mode[max_switches];
extern int switch_x[max_switches];
extern int switch_y[max_switches];

extern int switch_currentState[max_switches];
extern char switch_statelabel1[max_switches][20];
extern char switch_statelabel0[max_switches][20];

extern int switch_kvalues[max_switches][4];
extern int switch_counters[max_switches][4];
extern int switch_globalcounter[max_switches];

extern bool switch_queuedtoflip[max_switches];
extern int switch_signalcolour[max_switches];


extern int spawnPointCount;
extern int spawnx[MAX_SPAWN];
extern int spawny[MAX_SPAWN];
extern int sdirection[MAX_SPAWN];

extern int destPointCount;
extern int destx[MAX_DESTINATION];
extern int desty[MAX_DESTINATION];
extern int destid[MAX_DESTINATION];

extern int currentTick;
extern int seed;
extern int weatherMode;
extern char levelName[100];
extern int safetytiles;


extern int metric_delivered;
extern int metric_crashed;
extern int metric_collisions;
extern int metric_totaltrains;
extern int signallights[50];


extern bool emergencyhalt_active;
extern int emergencyhalt_x;
extern int emergencyhalt_y;
extern int emergencyhalt_timer;

void initializeSimulationState();

#endif
