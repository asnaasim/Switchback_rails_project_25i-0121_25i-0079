#ifndef SIMULATION_H
#define SIMULATION_H
#include <string>
using std::string;

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


