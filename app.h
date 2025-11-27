#ifndef APP_H
#define APP_H

extern void writeMetrics();      
extern void simulateOneTick();    
extern bool isSimulationComplete(); 
extern void toggleSwitchState();             // ye we access from dosri files in core cuz our game needs to proceed accordingly
extern bool toggleSafetyTile();   

bool initializeApp();


void runApp();


void cleanupApp();

#endif
