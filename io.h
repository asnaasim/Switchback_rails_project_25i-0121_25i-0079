#ifndef IO_H
#define IO_H
 
extern *grid;    // extern accesses them external source se 
extern int cols; 
extern int CURRENT_TICK;
extern int destPointCount;
 extern char switch_letter[26];
extern string switchcurrentState[26];
extern int switchmode[26];
extern int switchstatelabel0[26];
extern int switchstatelabel1[26];
extern int switch_kvalues[26][4];
extern int train_spawnticks[100];
extern int train_x[100];
extern int train_y[100];
extern int train_direction[100];
extern int train_ids[100];
extern int train_colorindex[100];
extern int spawnx[50]; 
extern int spawny[50]; 
extern int sdirection[50];
extern int Destx[50];
extern int Desty[50];
extern int weatherMode;
extern const int WEATHER_RAIN;
extern const int WEATHER_FOG;
extern const int WEATHER_NORMAL;
extern int spawnPointCount;
extern bool isInBounds(int x,int y); 
extern bool istracktile(int x,int y);
bool loadLevelFile(const string& filename);
void initializeLogFiles();
void logTrainTrace(int tick,int train_id,int x,int y,int direction,int state);
void logSwitchState(int tick,char Switch,const string& mode,const string& state);
void logSignalState(int tick,char Switch,const string& SigColor);
void writeMetrics();
#endif
