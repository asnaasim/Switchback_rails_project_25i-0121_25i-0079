#ifndef SWITCHES_H
#define SWITCHES_H


void updateSwitchCounters(int index,int direction);
void queueSwitchFlips();
void applyDeferredFlips();

void updateSignalLights();

void toggleSwitchState(int index);
int getSwitchStateForDirection(int index,int Entrydirection);
#endif
