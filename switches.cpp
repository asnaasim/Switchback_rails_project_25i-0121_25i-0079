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
          switch_currentState[i]=(1-(switch_currentState[i]));   
          int currentstate_output;                                 //ismein we flip the current state
          if(switch_currentState[i]==0)
          {
            currentstate_output=switchstatelabel0[i];}    //ismein you sirf check ke current state kiya hogi agar 0 then assign state0 to it
           else                                           //agar 1 hogi tou j assign state 1 and baad mein all of this is passed to the SwitchState wali file
            {  currentstate_output=switchstatelabel1[i];}             //logfile made in io.cpp
            logSwitchState(CURRENT_TICK,switch_letter[i],switchmode[i],currentstate_output);
            switch_queuedtoflip[i]=false;} 
        i++;}  }
          

void updateSignalLights()
{    int indxx=0;
    while(indxx<NumSwitches)
    {
        if (switch_letter[indxx]!='\0')
        {
            signal_lights[indxx]=GREEN;  
            }
        else 
            signal_lights[indxx]='\0';
        indxx++;
    }
   
    int i =0;
    while(i<NumSwitches)
    {
        if (switch_letter[i]=='\0')
        {
            continue;  // Skip empty switches ki jaga cuz shuru mein we have fixed size tou empty things will be encountered
           }
       
        int switch_row=switch_positions[i][0];
        int switch_col=switch_positions[i][1];
       
        int next_row=switch_row;
        int next_col=switch_col;
       
        int current_state=switch_currentState[i];
        int exit_direction;
               if (current_state==0)
        {
            exit_direction=switchstatelabel0[i];   //checks the value of current state fro, current state ka arr and then assigns exit val fro, switch takay it stays same path pe agar it encounters koye aur trains phir dir changes
        }
        else if (current_state==1)
        {
            exit_direction = switchstatelabel1[i];  }
       
        switch (exit_direction)
        {
          case 0:
            {next_row=next_row-1;
            break;}
        
          case 1:   
            {next_col=next_col+1;
            break;}
        
          case 2:
        {
            next_row=next_row+1;
            break;        }
          case 3: 
        {
            next_col=next_col-1;
            break;   }
          default:
             cout<<"unexpected haulting!! reboot system"<<endl;  }
       
        if (next_row<0||next_row >=GRID_ROWS||next_col<0||next_col>=GRID_COLS)
        {
            signal_lights[i]=RED;  
            continue;
        }
       
        for (int t=0;t<NumTrains;t++)
        {
            if (train_active[t]==false)
            {      continue;           }  //cont cuz yahan inactive wahan se terminate and move to jahan its active
           else if (train_active[t]==true){
            int train_row = train_positions[t][0];
            int train_col = train_positions[t][1];
           
            int row_diff=train_row-switch_row;
            int col_diff=train_col-switch_col;
            int distance=abs(row_diff)+abs(col_diff);  // Manhattan distance cuz distance switch se hve to calc takay we can identigfy konsa colour to assign agay
           
            if (distance>0&&distance<=2)   //greater than 0 cuz 0 pe collision 2 pe yellow n us se kam pe we get red
            {
                int train_dir=train_direction[t];
                bool train_approaching=false;
               
                if ((train_dir==0&&train_row > switch_row && train_col==switch_col)||(train_dir==1&&train_col <switch_col&&train_row==switch_row)|| (train_dir==2&&train_row < switch_row && train_col==switch_col)||(train_dir==3&&train_col >switch_col&&train_row==switch_row))    
                  {
                    train_approaching=true;        //checks agar trains coming tab we do yellow
                     }


                if (train_approaching&&signal_lights[i]==GREEN)
                {
                    signal_lights[i]=YELLOW;   }
            }
           
            if (train_row==next_row&&train_col==next_col)   //checks agar we assign red cuz agar sab occupied ho
            {
                signal_lights[i]=RED;  //we assign red to it cuz agar next tile occupied n wahan alr a trains there
            }
           
            int train_next_row=train_row;
            int train_next_col=train_col;
            int train_dir=train_direction[t];
           
            switch(train_dir)                  // is point pe we calc where will our trains move agay and decide phir how do we go abt it
            {                                          //this will check the direction agar 0 1 2 3 after which we make choice ke we move ooper, down left or right
              case 0: 
                     {train_next_row=train_next_row-1;
                     break;}
            case 1:
                     {train_next_col=train_next_col+1;
                     break;}
            case 2: 
                     {train_next_row=train_next_row+1;
                     break;}
            case 3: 
                     {train_next_col=train_next_col-1;
                     break;}
            default:
                             {cout<<"unexpected error!! system hault"<<endl;
                             break;}
            }
           
                                                                             // we will check yahan agar train ki next pos like x n y coords match next x n y coords movement specturm pe
            if (train_next_row==next_row&&train_next_col==next_col)
            {
                signal_lights[i]=RED;   //when sig light get 2 matlab assign red cuz collision matlab train is viciniity mein hi
            } }
    }
    i++;
}
}

void toggleSwitchState(int index)
{                                                           //gottamake sure ke index pe null charactr na ho and out of bounds na ho
          switch_currentState[index]=1-switch_currentState[index]; 
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
      return(switch_currentState[index]);           //fetches state atthe index
    }    
    else                                     //  
    {return -1; }                                      //we get -1 agar invalid ho index like outta range typa 
}
