#include "switches.h"
#include "simulation_state.h"
#include "grid.h"
#include "io.h"


// This will sirf Increment counters for trains entering switches jo bhi direction hui.
void updateSwitchCounters(int index, int direction) 
{
    if ((index>=0&&index<NumSwitches)&&(direction>=0&&direction<=3))   //if entered index and dir are within limits so the switchcounter array will inc warna waisa hi stays the same
     {
        if (direction==0)
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


void queueSwitchFlips() {
}

// ----------------------------------------------------------------------------
// APPLY DEFERRED FLIPS
// ----------------------------------------------------------------------------
// Apply queued flips after movement.
// ----------------------------------------------------------------------------
void applyDeferredFlips() {
}

// ----------------------------------------------------------------------------
// UPDATE SIGNAL LIGHTS
// ----------------------------------------------------------------------------
// Update signal colors for switches.
// ----------------------------------------------------------------------------
void updateSignalLights() {
}

// ----------------------------------------------------------------------------
// TOGGLE SWITCH STATE (Manual)
// ----------------------------------------------------------------------------
// Manually toggle a switch state.
// ----------------------------------------------------------------------------
void toggleSwitchState() {
}

// ----------------------------------------------------------------------------
// GET SWITCH STATE FOR DIRECTION
// ----------------------------------------------------------------------------
// Return the state for a given direction.
// ----------------------------------------------------------------------------

int getSwitchStateForDirection(int index, int Entrydirection)
{                                                          //also used in agli files train wali for entry dir
    if (index>=0&& index<NumSwitches)                   //we check index<NumSwitches cuz what if num of switches are kam than 26
    {
        return Switch[index].currentState;           //fetches state atthe index
    }    
    else
    {
        return -1;
    }
}
