#include "switches.h"
#include "simulation_state.h"
#include "grid.h"
#include "io.h"
#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;

void updateSwitchCounters(int index, int direction)
{
    if ((index >= 0 && index < NumSwitches) && (direction >= 0 && direction <= 3))
    {
        switch_counters[index][direction] += 1;
        switch_globalcounter[index] += 1;
    }
}

void queueSwitchFlips()
{
    int i = 0;
    while (i < NumSwitches)
    {
        if (switch_letter[i] == '\0')
        {
            i++;
            continue;
        }

        bool if_flip = false;

        // Yahan we checl if any direction counter has reached its k-value threshold agar yes tou phir flip and 0 assign reset
        if (switch_kvalues[i][0] > 0 && switch_counters[i][0] >= switch_kvalues[i][0])
        {
            if_flip = true;
            switch_counters[i][0] = 0;
        }
        else if (switch_kvalues[i][1] > 0 && switch_counters[i][1] >= switch_kvalues[i][1])
        {
            if_flip = true;
            switch_counters[i][1] = 0;
        }
        else if (switch_kvalues[i][2] > 0 && switch_counters[i][2] >= switch_kvalues[i][2])
        {
            if_flip = true;
            switch_counters[i][2] = 0;
        }
        else if (switch_kvalues[i][3] > 0 && switch_counters[i][3] >= switch_kvalues[i][3])
        {
            if_flip = true;
            switch_counters[i][3] = 0;
        }

        if (if_flip == true)
        {
            switch_queuedtoflip[i] = true;
        }
        i++;
    }
}

void applyDeferredFlips()
{
    int i = 0;
    while (i < NumSwitches)
    {
        if (switch_queuedtoflip[i] == true)
        {
            switch_currentState[i] = 1 - switch_currentState[i];
            
            char currentstate_output;
            if (switch_currentState[i] == 0)
            {
                currentstate_output = switch_statelabel0[i][0];
            }
            else
            {
                currentstate_output = switch_statelabel1[i][0];
            }
            
            logSwitchState(currentTick, switch_letter[i], mode[i], currentstate_output);
            switch_queuedtoflip[i] = false;
        }
        i++;
    }
}

void updateSignalLights()
{
    // Initialize all signal lights to GREEN or null
    int indxx = 0;
    while (indxx < max_switches)
    {
        if (switch_letter[indxx] != '\0')
        {
            signallights[indxx] = GREEN;
        }
        else
        {
            signallights[indxx] = 0;
        }
        indxx++;
    }

    int i = 0;
    while (i < NumSwitches)
    {
        if (switch_letter[i] == '\0')
        {
            i++;
            continue;
        }

        int switch_row = switch_x[i];
        int switch_col = switch_y[i];


        int current_state = switch_currentState[i];
        char exit_direction;
        
        if (current_state == 0)
        {
            exit_direction = switch_statelabel0[i][0];
        }
        else
        {
            exit_direction = switch_statelabel1[i][0];
        }

                                  // exit direction ko we check n then signal changings
        int next_row = switch_row;         //agar too close tou red warna yellow if still close n far would be green
        int next_col = switch_col;

        switch (exit_direction)
        {
            case '0':            // for up
            {
                next_row = next_row - 1;
                break;
            }
            case '1':             //  for right
            {
                next_col = next_col + 1;
                break;
            }
            case '2':            //for diwn
            {
                next_row = next_row + 1;
                break;
            }
            case '3':       // for left
            {
                next_col = next_col - 1;
                break;
            }
            default:
            {
                cout << "Invalid exit direction for switch " << switch_letter[i] << endl;
                break;
            }
        }


        if (next_row < 0 || next_row >= rows || next_col < 0 || next_col >= cols)
        {
            signallights[i] = RED;
            i++;
            continue;
        }
                                  //yahan we check collision and for that red signal and
        int t = 0;
        while (t < NumTrains)
        {
            if (train_status[t] == TRAIN_INACTIVE || train_status[t] == TRAIN_CRASHED || train_status[t] == TRAIN_ARRIVED)
            {
                t++;
                continue;
            }

            int train_row=train_x[t];
            int train_col=train_y[t];

                                  // Calculate Manhattan distance from switch
            int row_diff=train_row-switch_row;
            int col_diff=train_col-switch_col;
            int distance=abs(row_diff)+abs(col_diff);

            if (distance>0&&distance<=2)
            {
                int train_dir=train_direction[t];
                bool train_approaching=false;

                if ((train_dir==0&&train_row>switch_row&&train_col==switch_col)||    (train_dir==1 &&train_col<switch_col&&train_row==switch_row)||(train_dir==2 &&train_row<switch_row&&train_col==switch_col)||(train_dir==3&&train_col>switch_col&& train_row==switch_row))
                {
                    train_approaching=true;
                }

                if (train_approaching==true&&signallights[i]==GREEN)
                {
                    signallights[i]=YELLOW;
                }
            }


            if (train_row == next_row && train_col == next_col)
            {
                signallights[i] = RED;       //red jab train is v close and almost collision
            }         
            int train_next_row = train_row;
            int train_next_col = train_col;
            int train_dir = train_direction[t];

            switch (train_dir)
            {
                case 0:          // upward direction
                {
                    train_next_row=train_next_row-1;
                    break;
                }
                case 1:         // rightward dir
                {
                    train_next_col=train_next_col+1;
                    break;
                }
                case 2:      //downward dir 
                {
                    train_next_row=train_next_row+1;
                    break;
                }
                case 3:         //leftwards
                {
                    train_next_col=train_next_col-1;
                    break;
                }
                default:
                {
                    break;
                }
            }

            if (train_next_row==next_row && train_next_col == next_col)
            {
                signallights[i] = RED;
            }
            
            t++;
        }
        i++;
    }}

void toggleSwitchState(int index)
{
    if (index >= 0 && index < NumSwitches && switch_letter[index] != '\0')
    {
        switch_currentState[index] = 1 - switch_currentState[index];
        
        char state;
        if (switch_currentState[index] == 0)
        {
            state = switch_statelabel0[index][0];
        }
        else
        {
            state = switch_statelabel1[index][0];
        }
        
        logSwitchState(currentTick, switch_letter[index], mode[index], state);
    }
}
int getSwitchStateForDirection(int index, int direction)
{
    if (index >= 0 && index < NumSwitches)
    {
        return switch_currentState[index];
    }
    else
    {
        return -1;
    }
}
