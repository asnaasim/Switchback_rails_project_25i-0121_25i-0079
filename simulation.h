#ifndef SIMULATION_H
#define SIMULATION_H
//simple header file as barely any constants used, just utilisation of an additional funtion

// ============================================================================
// SIMULATION.H - Simulation tick logic
// ============================================================================

// ----------------------------------------------------------------------------
// MAIN SIMULATION FUNCTION
// ----------------------------------------------------------------------------
// Run one simulation tick.
void simulateOneTick();

// ----------------------------------------------------------------------------
// INITIALIZATION
// ----------------------------------------------------------------------------
// Initialize the simulation after loading a level.
void initializeSimulation();

// ----------------------------------------------------------------------------
// UTILITY
// ----------------------------------------------------------------------------
// True if all trains are delivered or crashed.
bool isSimulationComplete();

//additional function for grid printing
void printGrid();

#endif


