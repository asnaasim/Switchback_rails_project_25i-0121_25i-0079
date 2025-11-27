#include "grid.h"
#include "simulation_state.h"

bool isInBounds(int x,int y)           //checks if entered x n y are in the range of arr
{
    if (x>=0&&x<rows&&y>=0&&y<cols)
      return true;
    else
      return false;
}

bool isTrackTile(char tile)       //checks tiles ki vals the train can move on
 {
    if (tile=='-'||tile=='|'||tile=='/'||tile=='\\'||tile=='+'||tile=='='||tile=='S'||tile=='D'||(tile>='A'&&tile<='Z'))
      return true;
    else 
      return false;
}

bool isSwitchTile(char tile) 
 {
    if (tile>='A'&&tile<='Z')
      {return true;}
    else if(tile<'A'||tile>'Z')          //bas checks ke A se Z
      {return false;}
 }

int getSwitchIndex(char tile) 
 {
   int index;
   index=tile-'A';
   if (tile>='A'&&tile<='Z')
      return (index);
   else
      return (-1); 
 }
bool isSpawnPoint(int x,int y) //checks spawn points ki vals by comparing their x and y vals w int ki x and y vals
 {     
      int index=0;
      while (index<spawnPointCount) 
      {
        if (SpawnPointsX[index]==x&&SpawnPointsY[index]==y) 
            return true;
        index++;
      }
      return false;
}
bool isDestinationPoint(int x,int y)  //checks dest points ki vals by comparing their x and y vals w int ki x and y vals
{
  int index=0;
      while (index<destinationPointCount) 
      {
        if ((DestPointsX[index]==x)&&(DestPointsY[index]==y)) 
        { 
           return true;
         }
         index++;
     }
    return false; 
}

bool toggleSafetyTile(int x,int y)         //this one asal mein j swaps but for that we check ke if the passed params are range mein or not
 {
    char tile;
    if (isInBounds(x,y)==false)
     return false;
    else if(isInBounds(x,y)==true)
    {
      tile=grid[x][y];                  //we assign aik val to tile taka its gets easier
      if (tile=='-'||tile=='|')            //safety tile can only be toggled agar - or | ho. and if sucessfully toggled tou true warna false
    {
        grid[x][y]='=';
        safetyTilesUsed++;                     //use of stfy tile is inc
        return true;
    }
    if (tile=='=')                                      
    {
        grid[x][y]=originalGrid[x][y];
        safetyTilesUsed--;                                                               
        return true;
    }

    return false; 
  }}

    
