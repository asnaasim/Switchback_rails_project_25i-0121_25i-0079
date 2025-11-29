#ifndef SWITCHES_H
#define SWITCHES_H
#include <string>
#include "simulation_state.h"
using std::string;

void updateSwitchCounters(int index, int direction);
void queueSwitchFlips();
void applyDeferredFlips();
void updateSignalLights();
void toggleSwitchState(int index);
int getSwitchStateForDirection(int index, int direction);

#endif
