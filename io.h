#ifndef IO_H
#define IO_H

bool loadLevelFile(filename);
void initializeLogFiles();
void logTrainTrace(int tick, int train_id, int x, int y, int dir, int state);
void logSwitchState(int tick, char Switch, const string& mode, const string& state);
void logSignalState(int tick, char Switch, const string& SigColor);
void writeMetrics(int deliver,int crash);
#endif
