#ifndef IO_H
#define IO_H

bool loadLevelFile(const string& filename);
void initializeLogFiles();
void logTrainTrace(int tick,int train_id,int x,int y,int direction,int state);
void logSwitchState(int tick,char Switch,const string& mode,const string& state);
void logSignalState(int tick,char Switch,const string& SigColor);
void writeMetrics();
#endif
