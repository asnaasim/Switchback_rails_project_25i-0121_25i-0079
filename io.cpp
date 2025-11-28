#include "io.h"
#include "simulation_state.h"
#include "grid.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

const char equal='=';
const char underscore = '|';
const char s_char='S';          //these consts will be unifrom poori jaga se like same so we declare as global
const char d_char='D';

string trim(const string& str)
{                                                        //yahan we make trim to cut extra spaces in a a line
 size_t first = str.find_first_not_of(" \t\r\n");
 if (first == string::npos)
 return "";
 size_t last = str.find_last_not_of(" \t\r\n");
 return str.substr(first,last-first+1);
}

bool loadLevelFile(const string& filename)      //pass file ka name
{
 ifstream file(filename);
 if (!file.is_open())       //we do file hangling that is checking file availaboe hai ya nae
 {
 cout<<"Error opening level file:"<< filename << endl;
 return false;
 }

 string Line;
 bool header_reading = true;
 bool map_reading = false;
 bool switches_reading = false;
 bool trains_reading = false;
 int map_ind = 0;

 while (getline(file, Line))      //we start file reading yahan se start 
 {
 if (Line.empty())              // we skip all empty lines cuz no point of reading those
 {
 continue;
 }

 if (header_reading==true)             //we start yahan se cuz rows, columns, seeds waghaira are here
 {
 if (Line.find("ROWS:")!= string::npos)
 {
 if (getline(file, Line))
 {
 rows = stoi(trim(Line));
 }
 }
 else if (Line.find("COLS:")!= string::npos)
 {
 if (getline(file,Line))
 {                                   //yahan we store cols rows waghiara sab
 cols = stoi(trim(Line));
 }
 }
 else if (Line.find("SEED:") != string::npos)
 {
 if (getline(file, Line))
 {
 seed = stoi(trim(Line));
 }
 }
 else if (Line.find("WEATHER:") != string::npos)
 {
 if (getline(file, Line))
 {
   weather=trim(Line);                   // these function for the weather checks agar rain normal ya fog
     if (weather=="RAIN")
        weatherMode= WEATHER_RAIN;
      else if (weather=="FOG")
        weatherMode= WEATHER_FOG;
          else
            weatherMode= WEATHER_NORMAL;
            }
 }
 else if (Line.find("MAP:") != string::npos)
 {
 header_reading=false;
 map_reading=true;

 grid = new char*[rows];
 originalGrid = new char*[rows];
 for (int r=0; r<rows;r++)
 {
 grid[r]=new char[cols];
 originalGrid[r]=new char[cols];
 }

   getline(file, Line);
   continue;
   }
  }
 else if (map_reading==true)
 {
 if (Line.find("SWITCHES:")!= string::npos)
 {
    map_reading=false;
    switches_reading=true;
    continue;
          }

 if (map_ind<rows)
 {
      int len=Line.length();
      int C=0;                      //we check the length of the line those will be the column limit
     while(C<cols)
    {
      char tile;          //accord to that we assign value to tile ya agar line ke char se ya empty string se
         if (C<len)
           tile=Line[C];
         else
            tile = ' ';
         grid[map_ind][C]=tile;
         originalGrid[map_ind][C]=tile;
      }
    map_ind++;
   }
  }
 else if (switches_reading==true)
   {
   if (Line.find("TRAINS:")!= string::npos)
  {
   switches_reading = false;
   trains_reading = true;
   continue;
     }

 istringstream iss(Line);
 char letter;
 string currentState;
 int mode;
 string state0, state1;
 int k1,k2,k3,k4;

 if (iss>>letter>>currentState>>mode>>k1>>k2>>k3>>k4>> state0 >> state1)
 {
 int index=letter-'A';
 if (index>=0&&index<=25)
 {
     switch_letter[index]=letter;                 //ye pooray saray arrays for switch rel things these are what we store
     switch_currentState[index]=currentState;
     switchmode[index] mode;
      switchstatelabel0[index]=state0;
     switchstatelabel1[index]=state1;
     switch_kvalues[index][0]=k1;
     switch_kvalues[index][1]=k2;
      switch_kvalues[index][2]=k3;
      switch_kvalues[index][3]=k4;
         }
     }
   }
 else if (trains_reading==true)
 {
    istringstream iss(Line);
    int tick,x,y,direction,Sigcolor;

    if (iss>>tick>>x>>y>>direction>>Sigcolor)
       {
        if (traincount<100)
         {
          train_spawnticks[traincount]=tick;                //ye sab arrs for train rel sab kuch we read we add yahan pe to keep a track and map them index to index
          train_x[traincount]=x;
          train_y[traincount]=y;
          train_direction[traincount]=direction;
          Train_ids[traincount]=traincount;
          train_colorindex[traincount]=Sigcolor;
           traincount++;
             }
         }
     }
   }
 int r=0; 
 int c=0;
 while(r<rows)
 {
   while(c<cols)
   {
    char tile=grid[r][c];
     if (grid[r][c]==s_char&&spawnPointCount<50)
      {
         bool is_isolated=true;

        if (r>0 &&grid[r-1][c]==equal) 
            {is_isolated=false;}
        else if (r<rows-1&&grid[r+1][c]==equal) 
             {is_isolated=false;}                    //gthis is spawn points ka check ke agar un se pehlay theres no = sign cuz we assume un ne pichla x is none
        else if (c>0 &&grid[r][c-1]==underscore) 
             {is_isolated=false;}
        else if (c<cols-1&&grid[r][c+1]==underscore) 
             {is_isolated=false;}

     if (is_isolated)
      {
        spawnx[spawnPointCount]=r;
        spawny[spawnPointCount]=c;
        sdirection[spawnPointCount]=0;
        spawnPointCount++;
     }
   }
 else if (tile==d_char && destPointCount<50)   //this checks destination points cuz at some lvls we have 2Ds
 {
 bool is_isolated = true;

 if (r>0 && grid[r-1][c]==equal)  //so to check if its isolated n a dest point there must be no equal (=) after it like x+1 pe and issi tarha a mix of other conditions
   {is_isolated=false;}
 else if (r<rows-1&&grid[r+1][c]==equal) 
   {is_isolated=false;}
 else if (c>0&&grid[r][c-1]==underscore) 
    {is_isolated=false;}
 else if (c<cols-1&&grid[r][c+1]==underscore) 
    {is_isolated=false;}

 if (is_isolated==true)              //ye checks agar isolates tou its a dest point warna it aint and its j a tile
 {
    Destx[destPointCount]=r;
    Desty[destPointCount]=c;
     destPointCount++; }
        }
    }
    c++;
 }
 r++;
 file.close();
 return true;
}

void initializeLogFiles()
{
 ofstream Trace("out/trace.csv");    //ismein all files are intialised like their names waghaira
 ofstream Switches("out/switches.csv");
 ofstream Signals("out/signals.csv");

 if (Trace.is_open())
 {
    Trace<<"Tick,TrainID,X,Y,Direction,State"<<endl;   //ismein their headings are passed that tells basically hai kiya in the file
    Trace.close();
      }
 if (Switches.is_open())
 {
    Switches<<"Tick,Switch,Mode,State"<<endl;
    Switches.close();
       }
 if (Signals.is_open())
 {
    Signals<<"Tick,Switch,Signal"<<endl;
    Signals.close();
      }
}

void logTrainTrace(int tick,int train_id,int x,int y,int dir,int state)
{
   ofstream file("out/trace.csv",ios::app);
   if (!file.is_open())
     {
      cout<<"Failed to open trace.csv"<<endl;       //log of train trace is csv jsime we write ye sab cheezain again separated by commas and then we add values jo we have read from lines of file level wali
      }
    else
    {
      file<<tick<<","<<train_id<<","<<x<<","<<y<<","<<dir<<","<<state<<endl;
      file.close();
       }
}

void logSwitchState(int tick,char Switch,const string& mode,const string& state)
 { 
  ofstream file("out/switches.csv",ios::app);              //all these are passed in switch state wali csv pehla we opem and then write contents in the alr made ones
   if (!file.is_open())
    {
      cout<<"Failed to open switches.csv"<<endl;
      }
    else
    {
       file<<tick<<","<<Switch<<","<<mode<<","<<state<<endl;
       file.close();
      }
}

void logSignalState(int tick,char Switch,const string& SigColor)
{
   ofstream file("out/signals.csv", ios::app);
   if (!file.is_open())
    {
      cout<<"Failed to open signals.csv"<<endl;            //all these things are written in the file signal state wali and csv file mein variables are used separated by commas
      }                              //csv is made in output dir
   else
     {
       file<<tick<<","<<Switch<<","<<SigColor<<endl;
       file.close();             //cheezain are passes one by one file mein hi
      }
}

void writeMetrics()
{
   ofstream file("out/metrics.txt");
   if (!file.is_open())
    {
     cout << "Failed to open metrics.txt" << endl;
       }
else if (file,is_open())
{
    file<<"Train Simulation Metrics"<<endl;            //final matric making starts yahan se
    file<<"Total Ticks :"<<currentTick<<endl;               //we output total ticks, total trains jo deliver hui n crashed ones
    file<<"Total Trains :"<<NumTrains<<endl;
    file<<"Trains Delivered :"<<metric_delivered<<endl;
    file<<"Trains Crashed :" <<trains_crashed<<endl;

 int collisions=trains_crashed/2;                //also mentions jitni coll hui
 file<<"Total Train Collisions That Happened :"<<collisions<<endl;

 float throughput=0.0;
 if (currentTick>0)
   {
     throughput=(metric_delivered*100.0)/currentTick;
       }
 file<<"Throughput :" << throughput<<" per 100 ticks"<<endl;

 int totalWait = 0;
 int activeTrains = 0;
 int i=0;                                 //adding dets of average wait n baki cheezain one by one in the matrics file to be made in the output dir
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
       avgWait=(float)totalWait / activeTrains;
    }
 else
  {
      avgWait = 0.0;
      }
 file<<"Average Wait :"<<avgWait<<endl;
 file<<"Total Flips :"<<totalSwitchFlips<<endl;
 file<<"Safety Tiles Used: "<<safetytiles<<endl;
 file.close();
}
}
