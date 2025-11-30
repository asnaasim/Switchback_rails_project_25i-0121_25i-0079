#include "grid.h"
#include "simulation.h"
#include "io.h"
#include "switches.h"
#include "trains.h"
#include "simulation_state.h"
#include <string>
#include <iostream>
using namespace std;
char grid[max_rows][max_cols];
char originalGrid[max_rows][max_cols];
int rows, cols;
char tile;
int spawnPointCount, destPointCount;
int destx[MAX_DESTINATION], desty[MAX_DESTINATION], spawnx[MAX_SPAWN], spawny[MAX_SPAWN];
int safetytiles=0;

bool isInBounds(int x,int y)           //checks if entered x n y are in the range of arr
{
    if (x>=0&&x<rows&&y>=0&&y<cols)
        return true;
    else
        return false;
}

bool isTrackTile(char tile)       //checks tiles ki vals the train can move on
{
    if (tile=='-'|| tile=='/'|| tile=='\\'|| tile=='+'|| tile=='|'|| tile=='='|| tile=='S'|| tile=='D'||(tile>='A'&&tile<='Z'))
        return true;
    else 
        return false;
}

bool isSwitchTile(char tile) 
{
    if (tile>='A'&&tile<='Z')
        return true;
    else
        return false;
}

int getSwitchIndex(char tile) 
{
    if (tile>='A' && tile<='Z')
    {
        int index=tile-'A';
        return index;
    }
    else
        return -1; 
}

bool isSpawnPoint(int x,int y) //checks spawn points ki vals by comparing their x and y vals w int ki x and y vals
{     
    int index=0;
    while (index<spawnPointCount) 
    {
        if (spawnx[index]==x && spawny[index]==y) 
            {return true;}
        index++;
    }
    return false;
}
      
bool isDestinationPoint(int x,int y)  //checks dest points ki vals by comparing their x and y vals w int ki x and y vals
{
    int index=0;
    while (index<destPointCount) 
    {
        if (destx[index]==x && desty[index]==y) 
        { return true;}
        index++;
    }
    return false;
}

void toggleSafetyTile(int x,int y)         //this one asal mein j swaps but for that we check ke if the passed params are range mein or not
{
    if (isInBounds(x,y)==false)
        {return;}

    else if(isInBounds(x,y)==true)
    {  char tile=grid[x][y];                  //we assign aik val to tile taka its gets easier
        if (tile=='-'||tile=='|')            //safety tile can only be toggled agar - or | ho. and if sucessfully toggled tou true warna false
       {
        grid[x][y]='=';
        safetytiles++;                    //use of stfy tile is inc cuz toggle hore hai
        return;
       }
       else if (tile=='=')                                      
         {
        grid[x][y]=originalGrid[x][y];
        safetytiles--;                                                               
        return;   }}
    
    return; 
}
