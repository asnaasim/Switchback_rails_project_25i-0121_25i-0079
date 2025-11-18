#include "grid.h"
#include "simulation_state.h"
#inlude <iostream>
using namespace std;
bool isInBounds(int x, int y)           //checks if entered x n y are in the range of arr
{
    if (x>=0 && x<=rows && y>=0 && y<=cols)
      return true;
    else
      return false;}
bool isTrackTile(char tile)       //checks tiles ki vals the train can move on
 {
    if (tile== '-'||tile== '|'||tile == '/' ||tile == '\\'||tile == '+'||tile== '='||tile =='S'||tile== 'D'||(tile >= 'A' && tile <= 'Z'))
      return true;
    else 
      return false;
}

bool isSwitchTile(char tile) 
 {
    if (tile>='A' && tile<='Z')
      return true;
    else
      return false;
 }

int getSwitchIndex( char tile) 
 {
   int index;
   index= tile-'A';
   if (tile<='A' && tile>='Z')
      return -1;
   else
      return (index); 
 }

bool isSpawnPoint( int x, int y) //checks spawn points ki vals by comparing their x and y vals w int ki x and y vals
 {     
      int i=0;
      while (i < spawnCount) 
      {
        if (spawnPoints[i].x == x && spawnPoints[i].y == y) 
            return true;
        i++;
      }
      return false;
}
 

bool isDestinationPoint(int x, int y)  //checks dest points ki vals by comparing their x and y vals w int ki x and y vals
{
  int i=0;
      while (i < Destination_count) 
      {
        if (Destination_Points[i].x == x && Destination_Points[i].y == y) 
            return true;
        i++;
      }
      return false;
}

bool toggleSafetyTile( int x, int y)
 {
    char tile;
    char t;
    if (isInBounds(x,y)==false)
     return false;
    
     tile=grid[x][y];

     if (tile=='.')
      {
        grid[x][y]='=';
        return true;
      }
     else if (tile=='=')
     {
        grid[x][y]='.';
        return true;
     }
     else
        return false; 
 }
