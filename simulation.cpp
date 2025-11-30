#include "simulation.h"
#include "simulation_state.h"
#include "trains.h"
#include "switches.h"
#include "io.h"
#include "grid.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;

// ============================================================================
// SIMULATION.CPP - Implementation of main simulation logic
// ============================================================================

void printGrid()
{
    cout << "Tick: " << currentTick << endl;

    //originalgrid will be used background kei liye
    char display[max_rows][max_cols];
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {display[i][j] = originalGrid[i][j];}
    }

    //this is to put trains track kei oopar
    for (int i = 0; i < NumTrains; i++)
    {
        if (train_status[i]==TRAIN_MOVING || train_status[i]==TRAIN_WAITING || train_status[i]==TRAIN_DELAYED)
        {
            int x=train_x[i];
            int y=train_y[i];
            if (isInBounds(x, y))
            {
                //train id display kei liye
                display[x][y] = '0' + (i % 10);
            }
        }
    }

    //printin the actual wali grid
    for (int i=0; i<rows; i++)
    {
        for (int j=0; j<cols; j++)
        {
            cout<<display[i][j];
        }
        cout<<endl;
    }
    
    //printing train kei proper statistics
    cout << "\nActive Trains: \n";
    for (int i=0; i<NumTrains; i++)
    {
        if (train_status[i]==TRAIN_MOVING || train_status[i]==TRAIN_DELAYED || train_status[i]==TRAIN_WAITING)
        {
            const string dirNames[] = {"UP","RIGHT","DOWN","LEFT"};
            const string stateNames[] = {"INACTIVE","WAITING","MOVING","CRASHED", "DELAYED","ARRIVED"};
            // Check direction bounds to prevent crashing dirNames array
            int dir = (train_direction[i] >= 0 && train_direction[i] <= 3) ? train_direction[i] : DIR_RIGHT;
            cout << " Train " << i << " at (" << train_x[i] << "," << train_y[i] << ") moving " 
                 << dirNames[dir] << " state: " << stateNames[train_status[i]] << endl;
        }
    }
    cout << "Delivered: " << metric_delivered << " | Crashed: " << metric_crashed << "\n";
}

// ----------------------------------------------------------------------------
// INITIALIZE SIMULATION
// ----------------------------------------------------------------------------

void initializeSimulation()
{
    initializeSimulationState();
    cout << "Simulation Initialized!" << endl;
    cout << "Your level: " << levelName << endl;
    cout << "Your Trains: " << NumTrains << endl;
    cout << "Grid: " << rows << " x " << cols << endl;
    cout << "Switches: " << NumSwitches << endl;
    cout << "seed: " << seed << endl;

    currentTick = 0;

    initializeLogFiles();
}

// ----------------------------------------------------------------------------
// SIMULATE ONE TICK (Phase structure for smooth movement)
// ----------------------------------------------------------------------------
void simulateOneTick()
{
    // PHASE 1: Time advancement and Spawning
    currentTick++;
    cout << "---------------------------------------" << endl;
    cout << "TICK: " << currentTick << endl;
    
    spawnTrainsForTick();
    updateEmergencyHalt(); // Manage the halt timer

    // PHASE 2: Route Determination (Plan next position for all moving/delayed trains)
    cout << "Route determination underway (Phase 2)..." << endl;
    determineAllRoutes();

    // PHASE 3: Switch & Signal Logic
    cout << "Switch and signal logic (Phase 3)..." << endl;
    
    // Update switch counters based on trains currently on switch tiles
    for (int i = 0; i < NumTrains; i++)
    {
        if (train_status[i] == TRAIN_MOVING || train_status[i] == TRAIN_WAITING || train_status[i] == TRAIN_DELAYED)
        {
            int x = train_x[i];
            int y = train_y[i];
            if (isInBounds(x, y))
            {
                char tile = grid[x][y];
                if (isSwitchTile(tile))
                {
                    int switchIndex = getSwitchIndex(tile);
                    if (switchIndex >= 0)
                    {
                        updateSwitchCounters(switchIndex, train_direction[i]);
                    }
                }
            }
        }
    }
    
    // Queue and apply flips based on counter updates
    queueSwitchFlips();
    applyDeferredFlips();
    
    // Update signals based on current positions and planned next positions
    updateSignalLights();

    // PHASE 4: Collision & Halt Application (Handle conflicts based on planned routes)
    cout << "Collision and Halt Application (Phase 4)..." << endl;
    detectCollisions();
    applyEmergencyHalt();

    // PHASE 5: Movement (Apply finalized moves)
    cout << "Moving trains (Phase 5)..." << endl;
    moveAllTrains();
    
    // Print the state
    printGrid();
}

// ----------------------------------------------------------------------------
// CHECK IF SIMULATION IS COMPLETE
// ----------------------------------------------------------------------------
bool isSimulationComplete()
{
    int active = 0;
    int unspawned = 0;
    
    for (int i = 0; i < NumTrains; i++)
    {
        // Count active trains (moving, waiting, or delayed)
        if (train_status[i]!=TRAIN_INACTIVE && 
            train_status[i]!=TRAIN_ARRIVED && 
            train_status[i] != TRAIN_CRASHED)
        {
            active++;
        }
        
        // Count trains that haven't spawned yet (spawn tick is in the future)
        // Note: TRAIN_INACTIVE is the only state for unspawned trains.
        if (train_status[i] == TRAIN_INACTIVE && train_spawnticks[i] > currentTick)
        {
            unspawned++;
        }
    }
    
    // Simulation complete only when:
    // 1. No active trains AND
    // 2. No unspawned trains waiting to spawn
    return (active == 0 && unspawned == 0);
}
