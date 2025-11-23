#include "switches.h"
#include "simulation_state.h"
#include "grid.h"
#include "io.h"
#include <iostream>
#include <string>
using namespace std;
void updateSwitchCounters(int index,int direction) 
{    if ((index>=0&&index<NumSwitches)&&(direction>=0&&direction<=3))   //if entered index and dir are within limits so the switchcounter array will inc warna waisa hi stays the same
     {  if (direction==0)
        {
            Switch[index].counterUp++;
        }
        else if(direction==2)
        {
           Switch[index].counterDown++;                     //jo bhi direction we get uske accord we can inc the counters we intilaised simulation mein in switch struct
        }
        else if(direction==3)
        {
           Switch[index].counterLeft++;
        }
        else if(direction==1)
        {
           Switch[index].counterRight++;
        }
        else{
            cout<<"invalid direction variable"<<endl;
        }
     }
}

void queueSwitchFlips() 
{  
   bool if_flip=false;
   for (int i=0; i<NumSwitches;i++)
   {
    if (Switch[i].letter=='\0')
    {continue;                    //skipping the jaga jahan no letter is there bas only null character is there
    }

    else if (Switch[i].kUp>0 &&Switch[i].counterUp>= Switch[i].kUp) //comparing counterUp to kUp cuz agar ziada hai sirf then hi flip hoga
    {
            if_flip=true;
            Switch[i].counterUp=0;          //reset na hua tou val will keep on exceeding aisa hi n gives ajeeb ans
    }
    else if (Switch[i].kDown>0 &&Switch[i].counterDown>=Switch[i].kDown)
    {
        if_flip=true;
        Switch[i].counterDown=0;                      //vals reset cuz threshhold exceed hogya hai alr
    }
    else if (Switch[i].kRight>0 &&Switch[i].counterRight>=Switch[i].kRight)
    {
        if_flip=true;
        Switch[i].counterRight=0;                      //vals reset cuz threshhold exceed hogya hai alr
    }
    else if (Switch[i].kLeft>0 &&Switch[i].counterLeft>=Switch[i].kLeft)
    {
        if_flip=true;
        Switch[i].counterLeft=0;                      //vals reset cuz threshhold exceed hogya hai alr
    }

    if(if_flip==true)
    {
        Switch[i].queuedToFlip=true;}
   }
}


void applyDeferredFlips()
{
   int i=0;
   while(i<NumSwitches)                 //agar queuedToFlip hoga which means agar wo flip hora ho which means ke dir counter kisi direction mein exceed hora ho
    {
        if (Switch[i].queuedToFlip==true)                //phir hi we apply ye poori logic
        {
          Switch[i].currentState=(1-(Switch[i].currentState));           //ismein we flip the current state
          if(Switch[i].currentState==0)
          {
            int currentstate_output=Switch[i].state0;}    //ismein you sirf check ke current state kiya hogi agar 0 then assign state0 to it
           else                                           //agar 1 hogi tou j assign state 1 and baad mein all of this is passed to the SwitchState wali file
            {  currentstate_output=Switch[i].state1;}             //logfile made in io.cpp
            logSwitchState(CURRENT_TICK,Switch[i].letter,Switch[i].mode,currentstate_output);
            Switch[i].queuedToFlip=false;} 
        i++;}  }


void updateSignalLights()
{

}

void toggleSwitchState(int index)
{                                                           //gottamake sure ke index pe null charactr na ho and out of bounds na ho
          Switch[index].currentState=1-Switch[index].currentState;           //ismein we flip the current state
          if(index>=0&& index<NumSwitches&&Switch[index].letter!='\0')               //allows the user to do things manually like right key press krke
          {                                                     // its diff from deffered flips cuz wo happens to the tick once every round and ye wala only when right key ko dabaya
            int::state =Switch[index].state0;}    //ismein you sirf check ke current state kiya hogi agar 0 then assign state0 to it
           else                                           //agar 1 hogi tou j assign state 1 and baad mein all of this is passed to the SwitchState wali file
            { int::state =Switch[index].state1;}             //logfile made in io.cpp
            logSwitchState(CURRENT_TICK,Switch[index].letter,Switch[index].mode,state);
        }

int getSwitchStateForDirection(int index,int Entrydirection)
{                                                          //also used in agli files train wali for entry dir
    if (index>=0&& index<NumSwitches)                   //we check index<NumSwitches cuz what if num of switches are kam than 26
    {
        return(Switch[index].currentState);           //fetches state atthe index
    }    
    else
    {return -1; }                                      //we get -1 agar invalid ho index like outta range typa thing
}
