#include "io.h"
#include "simulation_state.h"
#include "grid.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

extern Grid grid;               // extern accesses them external source se
extern SimulationState simulation; 
int f; 
bool loadLevelFile(const string& filename)
{
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening level file: " << filename << endl;
        return false;
    }

    string Line;
    char type;
    while (getline(file, Line))
     {
        if (Line.empty()||Line[0]) 
        { continue;} 
        istringstream iss(Line);
        iss >> alphatype;

        if (alphatype=='S') 
        {
                int x, y;
                string mode;
                iss >> x >> y >> mode;
                grid.setSwitch(x, y, mode);      
                simulation.addSwitch(x, y, mode); }
        else if (alphatype=='T')
        {
                int x, y, dir;
                iss >> x >> y >> dir;
                simulation.addTrain(x, y, dir);    
                }
        else if(alphatype=='L')
        {    int x, y;
                string colour;
                iss >> x >> y >> colour;
                grid.setSignal(x, y, colour);     
                simulation.addSignal(x, y, colour);
            }
        else
            cout<<"error undefined line" <<line<<endl;}
    file.close();
    return true;
}

void initializeLogFiles()                   //no param pass cuz sirf initialization happens here
 {                                         //ofstream is for writing wali thing
    ofstream Trace("trace.csv");
    ofstream Switch("switch.csv");
    ofstream Signal("signal.csv");

    Trace<<"Tick,TrainID,X,Y,Direction,State"<<endl;
    Switche<<"Tick,Switch,Mode,State"<endl;                    //these become headers cuz they tell hai kiya in the file
    Signal<<"Tick,Switch,Signal"<<endl;}


void logTrainTrace(int tick, int train_id, int x, int y, int dir, int state)
 {
    ofstream file("trace.csv", ios::app);
    if (!file.is_open())
    {
     cout<<"Failed to open trace.csv"<<endl;}

    else
    { file<<tick<<","<<train_id <<","<<x<<","<<y<<","<<dir<<","<<state<<endl;
      file.close();}
}

void logSwitchState(int tick, char Switch, const string& mode, const string& state)
{
     ofstream file("switch.csv", ios::app);
    if (!file.is_open())
    {
     cout<<"Failed to open switch.csv"<<endl;}
    else
    {
      file<<tick<<","<< Switch<<","<<mode<<","<<state<<endl;
      file.close();}
}

void logSignalState(int tick, char Switch, const string& SigColor) 
{
    ofstream file("signals.csv", ios::app);
    if (!file.is_open())
    {
     cout<<"Failed to open signals.csv"<<endl;}
    else
    {
    file<<tick<<","<<Switch <<"," <<SigColor << endl;
    file.close();}
}


void writeMetrics(int deliver,int crash)    //couts the final wala matrix
 {
    ofstream file("metrics.txt");
    file<<"Crashed:"<<crash <<endl;
    file<<"Delivered:"<<deliver<<endl;}
    
