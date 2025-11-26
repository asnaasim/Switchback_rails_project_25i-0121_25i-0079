#include "switches.h"
#include "simulation_state.h"
#include "grid.h"
#include "io.h"
#include <iostream>                                                        //change was done w arrays instead of structs
#include <string>
using namespace std;

void updateSwitchCounters(int index,int direction) 
{
   if ((index>=0&&index<NumSwitches)&&(direction>=0&&direction<=3))   //if entered index and dir are within limits so the switchcounter array will inc warna waisa hi stays the same
     {
        if (direction==0)
        {
           switchCounters[index][0]+=1;          //up counter will inc
        }
        else if(direction==2)
        {
          switchCounters[index][2]+=1;                     //jo bhi direction we get uske accord we can inc the counters we intilaised simulation mein in switch struct
        }
        else if(direction==3)
        {
           switchCounters[index][3]+=1;             //left counter will inc
        }
        else if(direction==1)
        {
           switchCounters[index][1]+=1;                //right counter will inc
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
    if (switchLetter[i]=='\0')
    {continue;                    //skipping the jaga jahan no letter is there bas only null character is there
    }

    else if (switchKValues[i][0]>0 &&switchCounters[i][0]>= switchKValues[i][0]) //comparing counterUp to kUp cuz agar ziada hai sirf then hi flip hoga
    {
            if_flip=true;
           switchCounters[i][0]=0;          //reset na hua tou val will keep on exceeding aisa hi n gives ajeeb ans
    }
    else if (switchKValues[i][2]>0 &&switchCounters[i][2]>= switchKValues[i][2])
    {
        if_flip=true;
        switchCounters[i][2]=0;                      //vals reset cuz threshhold exceed hogya hai alr
    }
    else if (switchKValues[i][1]>0 &&switchCounters[i][1]>= switchKValues[i][1])
    {
        if_flip=true;
        switchCounters[i][1]=0;                      //vals reset cuz threshhold exceed hogya hai alr
    }
    else if (switchKValues[i][3]>0 &&switchCounters[i][3]>= switchKValues[i][3])
    {
        if_flip=true;
        switchCounters[i][3]=0;                      //vals reset cuz threshhold exceed hogya hai alr
    }

    if(if_flip==true)
    {
        switchFlipQueued[i]=true;}
   }
}

void applyDeferredFlips()
{
   int i=0;
   while(i<NumSwitches)                 //agar queuedToFlip hoga which means agar wo flip hora ho which means ke dir counter kisi direction mein exceed hora ho
    {
        if (switchFlipQueued[i]==true)                //phir hi we apply ye poori logic
        {
          switchCurrentState[i]=(1-(switchCurrentState[i]));   
          int currentstate_output;                                 //ismein we flip the current state
          if(switchCurrentState[i]==0)
          {
            currentstate_output=switchStateLabel0[i];}    //ismein you sirf check ke current state kiya hogi agar 0 then assign state0 to it
           else                                           //agar 1 hogi tou j assign state 1 and baad mein all of this is passed to the SwitchState wali file
            {  currentstate_output=switchStateLabel1[i];}             //logfile made in io.cpp
            logSwitchState(CURRENT_TICK,switchLetter[i],switchMode[i],currentstate_output);
            switchFlipQueued[i]=false;} 
        i++;}  }
          

void updateSignalLights()
{
  
}

void toggleSwitchState(int index)
{                                                           //gottamake sure ke index pe null charactr na ho and out of bounds na ho
          switchCurrentState[index]=1-switchCurrentState[index]; 
              int state;                                         //ismein we flip the current state
          if(index>=0&& index<NumSwitches&& switchLetter[index]!='\0')               //allows the user to do things manually like right key press krke
          {                                                     // its diff from deffered flips cuz wo happens to the tick once every round and ye wala only when right key ko dabaya
            state =switchStateLabel0[index];}    //ismein you sirf check ke current state kiya hogi agar 0 then assign state0 to it
           else                                           //agar 1 hogi tou j assign state 1 and baad mein all of this is passed to the SwitchState wali file
            { state =switchStateLabel1[index];}             //logfile made in io.cpp
            logSwitchState(CURRENT_TICK,switchLetter[index],switchMode[index],state);
}

int getSwitchStateForDirection(int index,int Entrydirection)
{                                                          //also used in agli files train wali for entry dir
    if (index>=0&&index<NumSwitches)                   //we check index<NumSwitches cuz what if num of switches are kam than 26
    {
      return(switchCurrentState[index]);           //fetches state atthe index
    }    
    else
    {return -1; }                                      //we get -1 agar invalid ho index like outta range typa 
}
