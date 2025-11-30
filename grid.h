#ifndef GRID_H
#define GRID_H
#include "simulation_state.h"
#include "simulation.h"
#include "io.h"
#include "switches.h"
#include "trains.h"


bool isInBounds(int x,int y);
extern char grid[max_rows][max_cols];
bool isTrackTile(char tile);
bool isSwitchTile(char tile);
int getSwitchIndex(char tile);
bool isSpawnPoint(int x, int y);
bool isDestinationPoint(int x, int y);
void toggleSafetyTile(int x, int y);
#ifndef GRID_H
#define GRID_H
#include "simulation_state.h"
#include "simulation.h"
#include "io.h"
#include "switches.h"
#include "trains.h"


bool isInBounds(int x,int y);
extern char grid[max_rows][max_cols];
bool isTrackTile(char tile);
bool isSwitchTile(char tile);
int getSwitchIndex(char tile);
bool isSpawnPoint(int x, int y);
bool isDestinationPoint(int x, int y);
void toggleSafetyTile(int x, int y);

#endif

#endif
