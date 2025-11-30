#include "io.h"
#include "simulation_state.h"
#include "grid.h"
#include "simulation.h"
#include "switches.h"
#include "trains.h"
#include <fstream>
#include <iostream>
#include <cstring>
#include <sstream>
#include <string>

using namespace std;
const char equalsign = '=';
const char underscore = '|';
const char s_char='S';          //these consts will be unifrom poori jaga se like same so we 
const char d_char='D';
char grid[max_rows][max_cols];
int train_status[max_trains];
char originalGrid[max_rows][max_cols];
int train_direction[max_trains];
int NumTrains=100;
int train_waitticks[max_trains];
char switch_letter[max_switches];
int mode[max_switches];
int switch_x[max_switches];
char tile;
int spawnPointCount, destPointCount;
int destx[MAX_DESTINATION], desty[MAX_DESTINATION], spawnx[MAX_SPAWN], spawny[MAX_SPAWN];
int switch_y[max_switches];
int currentTick;
int sdirection[MAX_SPAWN];
int destid[MAX_DESTINATION];
int switch_currentState[max_switches];
char switch_statelabel1[max_switches][20];
char switch_statelabel0[max_switches][20];
int switch_kvalues[max_switches][4];
int switch_counters[max_switches][4];
int metric_delivered;
int metric_crashed;
int Train_ids[max_trains];
int train_spawnticks[max_trains];
int train_colorindex[max_trains]; 
int train_x[max_trains];
int train_y[max_trains]; 
char levelName[100];
int safetytiles;
int NumSwitches;
int weatherMode;
int metric_collisions;
int metric_totaltrains;
string weather;
int seed;
int rows, cols;

string trim(const string& str)
{
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == string::npos)      //this is an additional func it will truncate unnecessary spaces removed value mein
        return "";
    size_t last=str.find_last_not_of(" \t\r\n");
    return str.substr(first, last-first + 1);
}

bool loadLevelFile(const string& filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "Error opening level file:" << filename << endl;
        return false;
    }

    string Line;
    bool header_reading=true;
    bool map_reading=false;       //moving thru each one by onr
    bool switches_reading=false;
    bool trains_reading=false;
    int map_ind=0;

    while (getline(file, Line))
    {
        if (Line.empty()||Line[0])
        {                   //skipping empty lines n first line
            continue;
        }

        if (header_reading==true)
        {  
          if (Line.find("ROWS:")!=string::npos)
            {
                if (getline(file, Line))
                {
                    rows=stoi(trim(Line));  }
            }
            else if (Line.find("COLS:")!=string::npos)
            {
                if (getline(file,Line))
                {
                    cols=stoi(trim(Line)); }
            }
            else if (Line.find("SEED:")!=string::npos)
            {
                if (getline(file, Line))
                {
                    seed = stoi(trim(Line));  }
            }
            else if (Line.find("WEATHER:") != string::npos)
            {
                if (getline(file, Line))
                {
                    weather=trim(Line);
                    if (weather=="RAIN")
                        weatherMode=WEATHER_RAIN;
                    else if (weather=="FOG")
                        weatherMode=WEATHER_FOG;
                    else
                        weatherMode=WEATHER_NORMAL; }
            }
            else if (Line.find("MAP:")!=string::npos)
            {
                header_reading = false;
                map_reading = true;
                continue;  }
        }
        else if (map_reading==true)
        {
            if (Line.find("SWITCHES:")!=string::npos)
            {
                map_reading=false;
                switches_reading=true;
                continue;
            }

            if (map_ind<rows)
            {
                int len=Line.length();
                int C=0;
                while (C<cols)
                {
                    char tile;
                    if (C<len)
                        tile=Line[C];
                    else
                    {
                        tile=' ';
                    }
                    grid[map_ind][C]=tile;
                    originalGrid[map_ind][C]=tile;
                    C++;
                }
                map_ind++;
            }
        }
        else if (switches_reading==true)
        {
            if (Line.find("TRAINS:")!=string::npos)
            {
                switches_reading=false;
                trains_reading=true;
                continue;
            }

            istringstream iss(Line);
            char letter;
            string modeStr;
            char currentState;
            int k1, k2, k3, k4;
            string state0;
            string state1;

            if (iss>>letter>>modeStr>>currentState>>k1>>k2>>k3>>k4>> state0 >> state1)
            {
                int index=letter-'A';
                if (index>=0&&index<=25)
                {
                    switch_letter[index]=letter;
                    switch_currentState[index] = currentState - '0';
                    
                    if (modeStr == "PER_DIR")       //we check state n phir parsing starts
                       { mode[index]=SWITCH_MODE_PER_DIR;}
                    else if (modeStr=="GLOBAL")
                       { mode[index]=SWITCH_MODE_GLOBAL;}
                    else
                       { mode[index]=0;}
                    
                    switch_kvalues[index][0]=k1;
                    switch_kvalues[index][1]=k2;
                    switch_kvalues[index][2]=k3;
                    switch_kvalues[index][3]=k4;
                    
                    if (state0 == "STRAIGHT")          //is point pe upgradation of the movement n which direct theyll go in
                        {switch_statelabel0[index][0]='1';}
                    else if (state0=="TURN")
                        {switch_statelabel0[index][0]='2';}
                    else
                       { switch_statelabel0[index][0]=state0[0];}
                    
                    if (state1=="STRAIGHT")
                        {switch_statelabel1[index][0]='1';}
                    else if (state1=="TURN")
                        {switch_statelabel1[index][0]='2';}
                    else
                        {switch_statelabel1[index][0]=state1[0];}
                    
                    switch_statelabel0[index][1]= '\0';
                    switch_statelabel1[index][1]= '\0';
                    
                    NumSwitches++;            }
            }
        }
        else if (trains_reading==true)
        {
            istringstream iss(Line);
            int tick,x,y,direction, Sigcolor;

            if (iss>>tick>>x>>y>>direction>>Sigcolor)
            {
                if (NumTrains<100)
                {
                    train_spawnticks[NumTrains]=tick;
                    train_x[NumTrains]=x;
                    train_y[NumTrains]=y;
                    train_direction[NumTrains]=direction;
                    Train_ids[NumTrains]=NumTrains;
                    train_colorindex[NumTrains]=Sigcolor;
                    NumTrains++;     }
            }
        }
    }


    int r = 0;    // we need to find dest n spawn points too now cuz there are 2 types of Ss m Ds we implimentn checks
    while (r<rows)
    {
        int c=0;
        while (c<cols)
        {
            char tile=grid[r][c];
            if (tile==s_char&&spawnPointCount<=49)
            {
                bool is_isolated=true;

                if (r>0&&grid[r - 1][c]==equalsign)
                {
                    is_isolated=false;
                }
                else if (r<rows-1&&grid[r + 1][c]==equalsign)
                {
                    is_isolated=false;
                }
                else if (c>0&&grid[r][c - 1]==underscore)
                {
                    is_isolated=false;   }
                else if (c<cols-1&&grid[r][c + 1]==underscore)
                {
                    is_isolated=false;       }

                if (is_isolated==true)
                {
                    spawnx[spawnPointCount]=r;
                    spawny[spawnPointCount]=c;
                    sdirection[spawnPointCount]=0;
                    spawnPointCount++;
                }
            }
            else if (tile==d_char&&destPointCount<=49)
            {
                bool is_isolated=true;

                if (r>0&&grid[r - 1][c]==equalsign)
                {
                    is_isolated=false;
                }
                else if (r<rows-1&& grid[r + 1][c] == equalsign)
                {
                    is_isolated=false;    }
                else if (c>0&&grid[r][c - 1]==underscore)
                {
                    is_isolated=false; }
                else if (c<cols-1 && grid[r][c + 1]==underscore)
                {
                    is_isolated=false;   }

                if (is_isolated==true)
                {
                    destx[destPointCount]=r;
                    desty[destPointCount]=c;
                    destPointCount++; }
            }
            c++;
        }
        r++; }

    file.close();
    return true;}

void initializeLogFiles()
{
    ofstream Trace("out/trace.csv");
    ofstream Switches("out/switches.csv");
    ofstream Signals("out/signals.csv");

    if (Trace.is_open())
    {
        Trace<<"Tick,TrainID,X,Y,Direction,State"<<endl;
        Trace.close(); }
        
    if (Switches.is_open())
    {
        Switches<<"Tick,Switch,Mode,State"<<endl;
        Switches.close();  }
        
    if (Signals.is_open())
    {
        Signals<<"Tick,Switch,Signal"<<endl;
        Signals.close(); }
}

void logTrainTrace(int tick, int train_id, int x, int y, int dir, char state)
{
    ofstream file("out/trace.csv",ios::app);
    if (!file.is_open())
    {
        cout<<"Failed to open trace.csv"<<endl;}
    else
    {
        file<<tick<<","<<train_id<<","<<x<<","<<y<<","<<dir<<","<<state<<endl;
        file.close();  }
}

void logSwitchState(int tick, char Switch, int mode, char state)
{
    ofstream file("out/switches.csv", ios::app);
    if (!file.is_open())
    {
        cout<<"Failed to open switches.csv"<<endl;
    }
    else
    {
        file<<tick<<","<<Switch<<","<<mode<<","<<state<< endl;
        file.close();
    }
}

void logSignalState(int tick, char Switch, const string& SigColor)
{
    ofstream file("out/signals.csv", ios::app);
    if (!file.is_open())
    {
        cout << "Failed to open signals.csv" << endl;
    }
    else
    {
        file<<tick<<","<<Switch<<","<<SigColor<< endl;
        file.close();
    }
}

void writeMetrics()
{
    ofstream file("out/metrics.txt");
    if (!file.is_open())
    {
        cout<<"Failed to open metrics.txt" << endl;
    }
    else
    {
        file<<"Train Simulation Metrics"<<endl;

        file<<"Level: " << levelName<<endl;
        file<<"Total Ticks: " <<currentTick<< endl;
        file<<"Total Trains: " <<NumTrains<< endl;
        file<<"Trains Delivered: "<<metric_delivered << endl;
        file<<"Trains Crashed: "<<metric_crashed << endl;
        int collisions=metric_crashed/2;
        file<<"Total Train Collisions: "<<collisions<<endl;
        float throughput=0.0;
        if (currentTick > 0)
        {
            throughput = (metric_delivered * 100.0) / currentTick;
        }
        file<<"Throughput: "<<throughput<<" per 100 ticks" << endl;
        int totalWait=0;
        int activeTrains=0;
        int i=0;
        while (i<NumTrains)
        {
            if (train_status[i]==1)
            {
                totalWait=totalWait+train_waitticks[i];
                activeTrains++;
            }
            i++;
        }

        float avgWait;
        if (activeTrains>0)
        {
            avgWait = (float)totalWait/activeTrains;
        }
        else
        {
            avgWait=0.0;
        }
        file<<"Average Wait: "<<avgWait<<"ticks"<<endl;
        file<<"Safety Tiles Used: "<<safetytiles<<endl;
        file.close();
    }
}
