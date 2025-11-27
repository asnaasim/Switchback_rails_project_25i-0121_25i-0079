#include "switches.h"
#include "simulation_state.h"
#include "grid.h"
#include "io.h"
#include <iostream>
#include <string>
using namespace std;                       //we check direcfions ke liya ke what is train ki director right left up ya down
void updateSwitchCounters(int index,int direction) 
{
   if ((index>=0&&index<NumSwitches)&&(train_direction>=0&&train_direction<=3))   //if entered index and dir are within limits so the switchcounter array will inc warna waisa hi stays the same
     {
        if (train_direction==0)
        {
           switch_counters[index][0]+=1;          //up counter will inc
        }
        else if(train_direction==2)
        {
          switch_counters[index][2]+=1;                     //jo bhi direction we get uske accord we can inc the counters we intilaised simulation mein in switch struct
        }
        else if(train_direction==3)
        {
           switch_counters[index][3]+=1;             //left counter will inc
        }
        else if(train_direction==1)
        {
           switch_counters[index][1]+=1;                //right counter will inc
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
    if (switch_letter[i]=='\0')
    {continue;                    //skipping the jaga jahan no letter is there bas only null character is there
    }

    else if (switch_kvalues[i][0]>0 &&switch_counters[i][0]>= switch_kvalues[i][0]) //comparing counterUp to kUp cuz agar ziada hai sirf then hi flip hoga
    {
            if_flip=true;
           switch_counters[i][0]=0;          //reset na hua tou val will keep on exceeding aisa hi n gives ajeeb ans
    }
    else if (switch_kvalues[i][2]>0 &&switch_counters[i][2]>= switch_kvalues[i][2])
    {
        if_flip=true;
        switch_counters[i][2]=0;                      //vals reset cuz threshhold exceed hogya hai alr
    }
    else if (switch_kvalues[i][1]>0 &&switch_counters[i][1]>= switch_kvalues[i][1])
    {
        if_flip=true;
        switch_counters[i][1]=0;                      //vals reset cuz threshhold exceed hogya hai alr
    }
    else if (switch_kvalues[i][3]>0 &&switch_counters[i][3]>= switch_kvalues[i][3])
    {
        if_flip=true;
        switch_counters[i][3]=0;                      //vals reset cuz threshhold exceed hogya hai alr
    }

    if(if_flip==true)
    {
        switch_queuedtoflip[i]=true;}
   }
}

void applyDeferredFlips()
{
   int i=0;
   while(i<NumSwitches)                 //agar queuedToFlip hoga which means agar wo flip hora ho which means ke dir counter kisi direction mein exceed hora ho
    {
        if (switch_queuedtoflip[i]==true)                //phir hi we apply ye poori logic
        {
          switchcurrentState[i]=(1-(switchcurrentState[i]));   
          int currentstate_output;                                 //ismein we flip the current state
          if(switchcurrentState[i]==0)
          {
            currentstate_output=switchstatelabel0[i];}    //ismein you sirf check ke current state kiya hogi agar 0 then assign state0 to it
           else                                           //agar 1 hogi tou j assign state 1 and baad mein all of this is passed to the SwitchState wali file
            {  currentstate_output=switchstateLabel1[i];}             //logfile made in io.cpp
            logSwitchState(CURRENT_TICK,switch_letter[i],switchmode[i],currentstate_output);
            switch_queuedtofliped[i]=false;} 
        i++;}  }
          

void updateSignalLights()
{
  
}

void toggleSwitchState(int index)
{                                                           //gottamake sure ke index pe null charactr na ho and out of bounds na ho
          switchcurrentState[index]=1-switchcurrentState[index]; 
              int state;                                         //ismein we flip the current state
          if(index>=0&&index<NumSwitches&&switch_letter[index]!='\0')               //allows the user to do things manually like right key press krke
          {                                                     // its diff from deffered flips cuz wo happens to the tick once every round and ye wala only when right key ko dabaya
            state=switchstatelabel0[index];}    //ismein you sirf check ke current state kiya hogi agar 0 then assign state0 to it
           else                                           //agar 1 hogi tou j assign state 1 and baad mein all of this is passed to the SwitchState wali file
            { state=switchstatelabel1[index];}             //logfile made in io.cpp
            logSwitchState(CURRENT_TICK,switch_letter[index],switchmode[index],state);
}

int getSwitchStateForDirection(int index,int Entrydirection)
{                                                          //also used in agli files train wali for entry dir
    if (index>=0&&index<NumSwitches)                   //we check index<NumSwitches cuz what if num of switches are kam than 26
    {
      return(switchcurrentState[index]);           //fetches state atthe index
    }    
    else                                     //  
    {return -1; }                                      //we get -1 agar invalid ho index like outta range typa 
}
