#include "io.h"
#include "simulation_state.h"
#include "grid.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

const char a_char='A';
const char b_char='B';
const char c_char='C';
const char d_char='D';
const char e_char='E';
const char f_char='F';
const char g_char='G';
const char h_char='H';
const char i_char='I';
const char j_char='J';
const char k_char='K';
const char l_char='L';
const char m_char='M';
const char n_char='N';
const char o_char='O';
const char p_char='P';
const char q_char='Q';
const char r_char='R';
const char s_char='S';
const char t_char='T';
const char u_Char ='U';
const char v_char='V';
const char w_char='W';
const char x_char='X';
const char y_char='Y';
const char z_char='Z';
const char equal='=';             //these are used to check agar D destpoint hai ya j another tile
const char underscore='|';  
  string Line;
    string weather;
    char type;
    int indx; 
    int i=0;
    int r=0;
    int c_rows=0;  
    int temprow=0; 
    int tempcol=0; 
    int map_ind=0;
     bool header_reading = true;
     bool map_reading = false;
     bool switches_reading = false;
     bool trains_reading = false;
    int seed;
    int c_row=1;   //acts as row ki index as when we iterate thru the file  we start 1 se cuz 0 pos pe it j has NAME 
    bool map_found=false;    //ye help to iterate through file once map is found 
                                                  // extern accesses them external source se
extern SimulationState simulation; 
extern char** grids;
extern int rows; 
extern struct metrics metric;
int row,col;
extern int cols; 
extern int CURRENT_TICK;
extern struct trains Train[100]; 
extern int traincount; 
extern struct switches Switch[26]; 
extern struct metrics metric;
extern struct destPoint DestPoints[50]; 
extern int destPointCount;
extern struct spawnPoint SpawnPoints[50]; 
extern int spawnPointCount;
extern bool isInBounds(int x, int y); 
extern bool istracktile(int x, int y);

bool loadLevelFile(const string& filename)
{
    ifstream file(filename);
    if (!file.is_open())
     {     cout << "Error opening level file:"<<filename<<endl;
        return false;
    }

    while (getline(file, Line))
    {
        if (Line.empty()) 
        { 
         continue;       //cont taka we skip khali lines
       }

        if (header_reading==true)
        {
            if (Line.find("ROWS:")!=string::npos)
            {   
               continue;
                if (getline(file, Line)) 
                { 
                  rows=stoi(Line);
                }
            }
            else if (Line.find("COLS:")!=string::npos)
            {
               continue;
                if (getline(file,Line))
                 { 
                  cols=stoi(Line); 
               }
            }
            else if (Line.find("SEED:")!=string::npos)
            {   
                continue;
                if (getline(file, Line)) 
                { 
                  seed=stoi(Line); 
               }
            }
            else if (Line.find("WEATHER:") != string::npos)
            {   
                continue;
                if (getline(file, Line)) 
                { 
                  weather=trim(Line); }
            }
            else if (Line.find("MAP:")!=string::npos)
            {
                header_reading=false;
                map_reading=true;
                continue;                              //continue used cuz we wanna skip the line yahan keyword use hua ho
            }
        }
        else if (map_reading)
        {
            if (Line.find("SWITCHES:") != string::npos)       //when keyword switch comes baki sab vals become false
            {
                map_reading = false;
                switches_reading = true;
                continue;
            }
            if (map_ind<rows)
            {
                for (int c=0;c<cols;c++)
                {
                    if (c<Line.length())
                     {
                        grids[map_ind][c]=Line[c];} 
                }
                map_ind++;}
        }
         else if (switches_reading)                   
        {
            if (Line.find("TRAINS:")!= string::npos)             //when they keyword train is found baki sab vals become false
            {
                switches_reading=false;
                trains_reading=true;
                continue;
            }
                                              // ismein we follow the format for switch ka struct A PER_DIR 0 3 3 3 3 STRAIGHT TURN)
            istringstream iss(Line);
            char letter;
            string currentState;
            int initialState, state0, state1;
            int init, k1, k2, k3, k4;
            if (iss>>letter>>currentState>>initialState>>k1>>k2>>k3>>k4>>state0>>state1)
            {
                int index=letter-'A';
                if (index>=0&&index<=25)
                {                                              //after reading sab vals are eneterd in switch ka struct one by one in order
                    Switch[index].letter = letter;
                    Switch[index].currentState=currentState ;
                    Switch[index].initialState=initialState;
                    Switch[index].state0 = state0;
                    Switch[index].state1 = state1;
                    Switch[index].kUp = k1;
                    Switch[index].kRight = k2; 
                    Switch[index].kDown = k3; 
                    Switch[index].kLeft = k4;}}
        }

        else if (trains_reading)            //yahan se reading for train val starts
        {
            istringstream iss(Line);
            int tick;
            int x, y, direction;
            int Sigcolor;        // we assign int vals to colors

            // Tismein we j follow yeeh wala format jisme we take vals and then enter train ke struct mei(<tick><x><y><direction><Sigcolor>)
            if (iss>>tick>>x>>y>>direction>>Sigcolor)
            {
                if (traincount<100)
                { 
                    Train[traincount].spawnTicks = tick;
                    Train[traincount].currentx = x;
                    Train[traincount].currenty = y;
                    Train[traincount].direction = direction;
                    Train[traincount].train_id = traincount;
                    metric.totaltrains = traincount + 1;
                    traincount++;}}
              }
          }
      file.close();

       for (int r=0;r<rows;r++)
        {
         for (int c=0;c<cols;c++)
          {
            char tile = grids[r][c];                  //checks dest points n spawn points
                                                  //spawn points shud be bas spart ke which means no = sign at r-1
            if (tile==s_char&&spawnPointCount<=49)&&(grids[r-1][c]!= equal&&grids[r+1][c]!=equal)||(grids[r-1][c]!= equal)||(grids[r][c+1]!=underscore&& grids[r][c-1]!=underscore&& grids[r+1][c]!=equal &&grids[r-1][c]!=equal))
            {
                SpawnPoints[spawnPointCount].x=r;
                SpawnPoints[spawnPointCount].y=c;             
                spawnPointCount++;}
           else if ((grids[r][c]==d_char&&destPointCount<=49)&&(grids[r+1][c]!= equal&&grids[r-1][c]!=equal)||(grids[r+1][c]!= equal)||(grids[r][c+1]!=underscore&& grids[r][c-1]!=underscore&& grids[r+1][c]!=equal &&grids[r-1][c]!=equal))
                    {
                        DestPoints[destPointCount].x=r;              //dest points shud be bas spart ke which means n0 = sign at r+1 wala krke
                        DestPoints[destPointCount].y=c;              //issi tarha we do diff checks and then get the value that is bilkul end pe
                        DestPoints[destPointCount].id=d_char;              // we take reference from 3 files hard, complex and mediym
                        destPointCount++;
                    }
                }
            }      
    file.close();
    return true;
}

void initializeLogFiles()                   //no param pass cuz sirf initialization happens here
 {                                         //ofstream is for writing wali thing
    ofstream Trace("trace.csv");
    ofstream Switches("switches.csv");
    ofstream Signals("signals.csv");
    
    if (Trace.is_open())
    {
       Trace<<"Tick,TrainID,X,Y,Direction,State"<<endl;
       Trace.close();}             //we always close files after using takay like its part of the syntax and 
    if (Switches.is_open())
    {
       Switches<<"Tick,Switch,Mode,State"<<endl;
       Switches.close();}                      //these become headers cuz they tell hai kiya in the file

    if (Signals.is_open())                        //not doing file handling yahan cuz uske baghair bhi it works but baki har jaga its imp 
    {   
       Signals<<"Tick,Switch,Signal"<<endl;
       Signals.close();}

    }
void logTrainTrace(int tick, int train_id, int x, int y, int dir, int state)
 {
    ofstream file("out/trace.csv", ios::app);           //we do out/ cuz we want ye sab to go in output ki directory 
    if (!file.is_open())
    {
     cout<<"Failed to open trace.csv"<<endl;}            //yahan we do file handling to check agar files khulre

    else
    { file<<tick<<","<<train_id <<","<<x<<","<<y<<","<<dir<<","<<state<<endl;
      file.close();}
}

void logSwitchState(int tick,char Switch,const string& mode,const string& state)
{
     ofstream file("out/switches.csv",ios::app);
    if (!file.is_open())
    {
     cout<<"Failed to open switch.csv"<<endl;}
    else
    {
      file<<tick<<","<<Switch<<","<<mode<<","<<state<<endl;
      file.close();}
}

void logSignalState(int tick, char Switch, const string& SigColor) 
{
    ofstream file("out/signals.csv", ios::app);
    if (!file.is_open())
    {
     cout<<"Failed to open signals.csv"<<endl;}
    else
    {
    file<<tick<<","<<Switch<<","<<SigColor<<endl;
    file.close();}
}


void writeMetrics()    //couts the final matrix in matrix.txt poori details uski
 {
    ofstream file("out/metrics.txt");
    if (!file.is_open)                         //file handling to check agar file has been opened ya nah
    {
      cout <<"Failed to open out/metrics.txt"<<endl;
    } 
    else                                                                 //yahan we extern matric from the external file simulation_state jis waja se we use extern
     {
       file<<"Total Trains :"<<metric.totaltrains<<endl;
       file<<"Trains Delivered At Their Destination :"<<metric.delivered<<endl;
       file<<"Trains Crashed :"<<metric.crashed<<endl;
       file<<"Total Train Collisions :"<<metric.collisions<<endl;
       file.close();
    }
}
