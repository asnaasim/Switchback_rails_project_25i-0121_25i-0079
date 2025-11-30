#include "switches.h"
#include "simulation_state.h"
#include "grid.h"
#include "io.h"
#include "simulation.h"
#include "trains.h"
#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;

void updateSwitchCounters(int index, int direction)
{
    if ((index>=0 && index<NumSwitches) && (direction>=0 && direction<=3))
    {
        switch_counters[index][direction]+=1;
        switch_globalcounter[index]+=1;
    }
}

void queueSwitchFlips()
{
    int i=0;
    while (i<NumSwitches)
    {
        if (switch_letter[i]=='\0')
        {
            i++;
            continue;
        }

        bool if_flip = false;

        //ek ek individual counters ko check karney kei liye
        if (mode[i]==SWITCH_MODE_PER_DIR) {
            for (int d = 0; d < 4; d++) {
                if (switch_kvalues[i][d] > 0 && switch_counters[i][d] >= switch_kvalues[i][d]) {
                    if_flip = true;
                    switch_counters[i][d] = 0;
                    // Reset all other counters for this switch? (Depends on specific rule, 
                    // but we only reset the one that triggered the flip here).
                }
            }
        }
        
        //global counter ka check
        else if (mode[i] == SWITCH_MODE_GLOBAL) {
            // Assuming global flip happens when global counter reaches kvalue[0]
            if (switch_kvalues[i][0] > 0 && switch_globalcounter[i] >= switch_kvalues[i][0]) {
                if_flip = true;
                switch_globalcounter[i] = 0;
            }
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
            switch_globalcounter[i] += 1; // Increment metric
            
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

// ----------------------------------------------------------------------------
// UPDATE SIGNAL LIGHTS (Fixed signal conditions for better control)
// ----------------------------------------------------------------------------
void updateSignalLights()
{
    // Initialize all signal lights to GREEN or null
    for (int i = 0; i < NumSwitches; i++)
    {
        if (switch_letter[i] != '\0')
        {
            signallights[i] = GREEN;
        }
    }

    // Check for RED and YELLOW conditions
    for (int i = 0; i < NumSwitches; i++)
    {
        if (switch_letter[i] == '\0')
        {
            continue;
        }

        int switch_row = switch_x[i];
        int switch_col = switch_y[i];

        int current_state = switch_currentState[i];
        char exit_direction_char;
        
        if (current_state == 0)
        {
            exit_direction_char = switch_statelabel0[i][0];
        }
        else
        {
            exit_direction_char = switch_statelabel1[i][0];
        }

        // Determine the coordinates of the tile *immediately after* the switch (the protected tile)
        int protected_row = switch_row;
        int protected_col = switch_col;
        int exit_direction = exit_direction_char - '0';
        
        if (exit_direction == DIR_UP) protected_row--;
        else if (exit_direction == DIR_RIGHT) protected_col++;
        else if (exit_direction == DIR_DOWN) protected_row++;
        else if (exit_direction == DIR_LEFT) protected_col--;

        // 1.Check for Out-of-Bounds/End-of-Track Crash (Force RED)
        if (!isInBounds(protected_row, protected_col))
        {
            signallights[i] = RED;
            continue; 
        }

        // 2. Check for Train Conflict (Set to RED if danger)
        for (int t = 0; t < NumTrains; t++)
        {
            if (train_status[t] == TRAIN_INACTIVE || train_status[t] == TRAIN_CRASHED || train_status[t] == TRAIN_ARRIVED)
            {
                continue;
            }

            int train_row = train_x[t];
            int train_col = train_y[t];
            
            // A. RED if the protected tile is CURRENTLY occupied by ANY train
            if (train_row == protected_row && train_col == protected_col)
            {
                signallights[i] = RED;
                break; // Switch is red, no need to check other trains for RED
            }
            
            // B. RED if any train is PLANNING to move into the protected tile (handled by collision detection, but useful redundancy)
            if (train_plannedmove[t] && train_nextx[t] == protected_row && train_nexty[t] == protected_col)
            {
                signallights[i] = RED;
                break; //switch is red, no need to check other trains for RED
            }

            // 3. checking for warning zone
            
            //manhatan distance ko calculate karney kei liye
            int row_diff = abs(train_row - switch_row);
            int col_diff = abs(train_col - switch_col);
            int distance = row_diff + col_diff;

            //only consider trains that are close (distance 1 or 2)
            if (signallights[i] == GREEN && distance > 0 && distance <= 2)
            {
                //check if the train is appoaching sai destination
                //this is a simplified check based on location relative to the switch, assuming straight-line approach
                bool train_approaching = false;
                if ((train_direction[t] == DIR_UP && train_row > switch_row && train_col == switch_col) ||    
                    (train_direction[t] == DIR_LEFT && train_col > switch_col && train_row == switch_row) || 
                    (train_direction[t] == DIR_DOWN && train_row < switch_row && train_col == switch_col) ||
                    (train_direction[t] == DIR_RIGHT && train_col < switch_col && train_row == switch_row))
                {
                    train_approaching = true;
                }
                
                if (train_approaching)
                {
                    signallights[i] = YELLOW;
                    //keep checking other trains, a later check might force it to RED
                }
            }
        }
    }
    
    //logging aakhri stages
    int j = 0;
    while (j<NumSwitches)
    {
        if (switch_letter[j]>='A' && switch_letter[j]<='Z')
        {
            string color;
            if (signallights[j]==GREEN)
                color = "GREEN";
            else if (signallights[j]==YELLOW)
                color="YELLOW";
            else
                color="RED";
            
            logSignalState(currentTick, switch_letter[j], color);
        }
        j++;
    }
}

void toggleSwitchState(int index)
{
    if (index>=0 && index<NumSwitches && switch_letter[index]!='\0')
    {
        switch_currentState[index]=1-switch_currentState[index];
        
        char state;
        if (switch_currentState[index] ==0)
        {
            state=switch_statelabel0[index][0];
        }
        else
        {
            state=switch_statelabel1[index][0];
        }
        
        logSwitchState(currentTick, switch_letter[index], mode[index], state);
    }
}

int getSwitchStateForDirection(int index, int direction)
{
    if (index>=0 && index<NumSwitches)
    {
        return switch_currentState[index];
    }
    else
    {
        return -1;}
}
