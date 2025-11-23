#ifndef SWITCHES_H
#define SWITCHES_H


void updateSwitchCounters(int index,int direction);
void queueSwitchFlips();
void applyDeferredFlips();
// ----------------------------------------------------------------------------
// SIGNAL CALCULATION
// ----------------------------------------------------------------------------
// Update switch signal colors.
void updateSignalLights();

void toggleSwitchState(int index);
int getSwitchStateForDirection(int index,int Entrydirection);
#endif
