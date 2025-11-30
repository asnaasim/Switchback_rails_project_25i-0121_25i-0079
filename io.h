#ifndef IO_H
#define IO_H
#include <string>
using std::string;
 
extern int rows, cols;

extern int mode[26];
extern int seed;
extern int weatherMode;

bool loadLevelFile(const string& filename);
void initializeLogFiles();
void logTrainTrace(int tick, int train_id, int x, int y, int direction, char state);
void logSwitchState(int tick, char Switch, int mode, char state);
void logSignalState(int tick, char Switch, int SigColour);
void writeMetrics();

#endif
