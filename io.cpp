#include "io.h"
#include "simulation_state.h"
#include "grid.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

const char equal='=';             //these are used to check agar D destpoint hai ya j another tile
const char underscore='|';  
  string Line;
    char type;
    int indx; 
    int i=0;
    int r=0;
    int temprow=0; 
    int tempcol=0; 
    int map_ind=0;
     bool header_reading = true;
     bool map_reading = false;
     bool switches_reading = false;
     bool trains_reading = false;
    int seed;  //acts as row ki index as when we iterate thru the file  we start 1 se cuz 0 pos pe it j has NAME 
    bool map_found=false;    //ye help to iterate through file once map is found 
 
string trim(const string& str) 
{                                                   //extra func that j trims extra spaces
    size_t first=str.find_first_not_of(" \t\r\n");
    if (first==string::npos)                                  //removes all the leading spaces so it avoids inaccuracy in func
        return ""; 
        size_t last=str.find_last_not_of(" \t\r\n");
    return str.substr(first, last-first+1);}

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
               
                if (getline(file,Line)) 
                { 
                  rows=stoi(Line);
                }
            }
            else if (Line.find("COLS:")!=string::npos)
            {
                if (getline(file,Line))
                 { 
                  cols=stoi(Line); 
               }
            }
            else if (Line.find("SEED:")!=string::npos)
            {   
                
                if (getline(file, Line)) 
                { 
                  seed=stoi(Line); 
               }
            }
            else if (Line.find("WEATHER:")!= string::npos)
            {   
                if (getline(file,Line)) 
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
               int map_ind=Line.find("MAP:")+2;
               int len=strlen(Line);
               else if (line=="MAP:")
            {
                readingHeader=false;
                readingMap=true;

                grid=new char*[rows];
                originalGrid=new char*[rows];
                for (int r=0;r<rows;r++)
                {
                    grid[r]=new char[cols];
                    originalGrid[r]=new char[cols];
                }

                continue;
            }
        }
                header_reading=false;
                map_reading=true;
                continue;                              //continue used cuz we wanna skip the line yahan keyword use hua ho
            }
        }
        else if (map_reading)
        {
            if (Line.find("SWITCHES:")!= string::npos)       //when keyword switch comes baki sab vals become false
            {
                map_reading = false;
                switches_reading = true;
                continue;
            }
            if (map_ind<rows)
            {
                                     
              for(int c=0;c<cols;c++) 
              {
                     
                   char tile;
                   if (c<len)
                      tile=Line[c];
                   else
                       tile= ' ';
                   grid[map_ind][c]=tile;
                   originalGrid[map_ind][c]=tile;
                }
                map_ind++;
        }
         else if (switches_reading)                   
        {
            if (Line.find("TRAINS:")!=string::npos)             //when they keyword train is found baki sab vals become false
            {
                switches_reading=false;
                trains_reading=true;
                continue;
            }
                                              // ismein we follow the format for switch ka struct A PER_DIR 0 3 3 3 3 STRAIGHT TURN)
            istringstream iss(Line);
            char letter;
            string currentState;
            int mode;
            char state0,state1;
            int init,k1,k2,k3,k4;
            if (iss>>letter>>currentState>>mode>>k1>>k2>>k3>>k4>>state0>>state1)
            {
                int index=letter-'A';
                if (index>=0&&index<=25)
                {                                              //after reading sab vals are eneterd in switch ka struct one by one in order
                    switch_letter[index]=letter;
                    switch_currentState[index]=currentState ;
                    switchmode[index]=mode;
                    switchstatelabel0[index]=state0;
                    switchstatelabel1[index]=state1;
                    switch_kvalues[index][0]=k1;
                    switch_kvalues[index][1]=k2;
                    switch_kvalues[index][2]=k3;
                    switch_kvalues[index][3]=k4;}}
        }

        else if (trains_reading)            //yahan se reading for train val starts
        {
            istringstream iss(Line);
            int tick;
            int x,y,direction;
            int Sigcolor;        // we assign int vals to colors
            // Tismein we j follow yeeh wala format jisme we take vals and then enter train ke struct mei(<tick><x><y><direction><Sigcolor>)
            if (iss>>tick>>x>>y>>direction>>Sigcolor)
            {
                if (traincount<100)
                { 
                    train_spawnticks[traincount]=tick;
                    train_x[traincount]= x;
                    train_y[traincount]= y;
                    train_direction[traincount]= direction;
                    Train_id[traincount]= traincount;
                    train_colorindex[traincount]=Sigcolor;
                    traincount++;}}
              }
          }
      file.close();
       for (int r=0;r<rows;r++)
        {
         for (int c=0;c<cols;c++)
          {
            char tile=grids[r][c];                  //checks dest points n spawn points
                                                  //spawn points shud be bas spart ke which means no = sign at r-1
            if (tile==s_char&&spawnPointCount<=49)&&(grids[r-1][c]!= equal&&grids[r+1][c]!=equal)||(grids[r-1][c]!= equal)||(grids[r][c+1]!=underscore&& grids[r][c-1]!=underscore&& grids[r+1][c]!=equal &&grids[r-1][c]!=equal))
              {
                Spawnx[spawnPointCount]=r;
                Spawny[spawnPointCount]=c; 
                sdirection[SpawnPointCount]=0;             
                spawnPointCount++;}
                
            else if ((grids[r][c]==d_char&&destPointCount<=49)&&(grids[r+1][c]!= equal&&grids[r-1][c]!=equal)||(grids[r+1][c]!= equal)||(grids[r][c+1]!=underscore&& grids[r][c-1]!=underscore&& grids[r+1][c]!=equal &&grids[r-1][c]!=equal))
                    {
                        Destx[destPointCount]=r;              //dest points shud be bas spart ke which means n0 = sign at r+1 wala krke
                        Desty[destPointCount]=c;              //issi tarha we do diff checks and then get the value that is bilkul end pe
                                                                // we take reference from 3 files hard, complex and mediym
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

void logSignalState(int tick,char Switch,const string& SigColor) 
{
    ofstream file("out/signals.csv",ios::app);
    if (!file.is_open())
    {
     cout<<"Failed to open signals.csv"<<endl;}
    else
    {
    file<<tick<<","<<Switch<<","<<SigColor<<endl;
    file.close();}
}

void writeMetrics()                 //we open matrics file yahan and agar it doesnt open tou error
{
    ofstream file("out/metrics.txt");           //this is metrics ki output file yahan all matric rel things are stroed
    if (!file.is_open()) 
    {
        cout<< "Failed to open metrics.txt"<<endl;
    }
    else if(file.is_open())
    {
      file <<"Train Simulation Metrics"<< endl;         //pehla we write matrics sara trains ke
      file <<"Total Ticks:"<<current_ticks<<endl;        //total ticks,trains crashed, trains delivered, crashed, total trains wai info
      file <<"Total Trains:"<<NumTrains<< endl;
      file <<"Trains Delivered:"<<trains_delivered<<endl;
      file <<"Trains Crashed:"<<trains_crashed<<endl;   

      int collisions=trainsCrashed/2;                 //cuz 2 trains hv 1 coll so / by 2
      file<<"Total Collisions:"<<collisions<< endl;            //uske baad we move on to extra info to be stored in matrics file
                                                          //throughput currentticks
      float throughput=0.0;
      if (currentTick>0)
      {
        throughput=(trainsDelivered*100.0)/currentTick;
      }
      file <<"Throughput:"<<throughput<<"per 100 ticks"<< endl;
        int totalWait=0;                       //we calc throughputs per 100 ticks cuz we've taken max trains as 100 vals we have taken
        int activeTrains=0;
       for (int i=0;i<NumTrains;i++) 
         {
          if (train_status[i]==1)      //trains shud be active tab ye condition works matlab if active tou total wait mein the value of array at that index is added   
            {
              totalWait=totalWait+train_waitticks[i];
              activeTrains++;     }
       }
       float avrgWait;
       if(activeTrains>0)
      {
       avrgWait=(float)totalWait/activeTrains;
          } 
       else
         {
            avegWait=0.0;
           }
        file<<"Average Wait:"<<avrgWait<<endl;
        file<<"Total Flips:"<<totalSwitchFlips<<endl;
        file<<"Safety tiles Used:"<<safetyTilesUsed<<endl;
        file.close();}
}
