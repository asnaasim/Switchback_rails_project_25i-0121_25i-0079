#ifndef GRID_H
#define GRID_H


bool isInBounds(int x, int y);
extern char grid;
bool isTrackTile(char tile);
bool isSwitchTile(char tile);
int getSwitchIndex(char tile);
bool isSpawnPoint(int x, int y);
bool isDestinationPoint(int x, int y);
bool toggleSafetyTile(int x, int y);

#endif
