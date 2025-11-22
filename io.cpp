#include "io.h"
#include "simulation_state.h"
#include "grid.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

extern Grid grid;       
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
const char equal='=';
const char underscore='_';   
                                                  // extern accesses them external source se
extern SimulationState simulation; 
extern char** grids;
extern int rows; 
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

    string Line;
    string weather;
    char type;
    int seed;
    int c_row=1;   //acts as row ki index as when we iterate thru the file  we start 1 se cuz 0 pos pe it j has NAME 
    bool map_found=false;    //changes its flag is the keyword of map is found

    while (getline(file, Line))
     {
        if (Line.empty()) 
        {continue;} 

         if (Line.find("ROWS:") != string::npos)
         {           
           if (getline(file, Line))             //stores the value in rows jo us line se agli mein ho
            {
              rows = stoi(Line); }
            }

          if (Line.find("COLS:") != string::npos)
          {
           if(getline(file, Line))           //checks COLS: kahan hai and hai bhi ya nah
           {                                  //stores the value in cols jo us line se agli mein ho
            cols = stoi(Line);  
             }
           }

        if (Line.find("SEED:") != string::npos)          //checks SEEDS: kahan hai finds its index
        {
            if (getline(file,Line))
            {
               seed= stoi(Line);                  //stores the value in seed jo us line se agli mein ho
            }
        }

         if (Line.find("WHEATHER:") != string::npos)          //checks WHEATHER: kahan hai finds its index
        {
            if(getline(file,Line))
            {                                                   //stores the value in wheather jo us line se agli mein ho
              weather=Line;                        
            }                
        }
        int indx; 
        int i=0;
        int r=2;
        int c_rows=0;  
        int temprow=0; 
        int tempcol=0;  

        if (currentRow<rows&&grids!=NULL)
        {
           if (Line.find("MAP:") != string::npos)
           { 
              int map_ind= Line.find("MAP:")+1;
              char tile;
              while (file.get(tile)){
              for (int i=0;((i<rows);i++)){
               for(int r=0;((r<cols)&&(r<Line.length());r++)) 
               {
                grids[i][r]=char; 
                if (istracktile(i,r)&&grid[i][r]==d_char&&destPointCount<=49)
                {
                    int x=i;
                    int y=j;          
                    bool isfound=true; 
                    if ((grid[indx][r]==d_char)&&(arr[indx][y+4]!=a_char&&arr[indx+4][y]!=a_char       //we create alag arr jisme we store saray Ds and then uss arr se we check agar
                       ||grid[i][r]!=b_char|| grid[i+4][r]!=b_char|| grid[i][r+4]!=c_char|| grid[i+4][r]!=c_char        //r+4 show kara then itll not b the dest cuz agay pir_dir ka p comes
                                                                                                                                                                    //ths shows endpoint nae hai ye so we do double check is waja se hi

                       ||grid[i][r]!=d_char|| grid[i+4][r]!=d_char|| grid[i][r+4]!=e_char|| grid[i+4][r]!=e_char||grid[i][r+4]!=f_char||grid[i+4][r]!=f_char
                       ||grid[i][r]!=g_char|| grid[i+4][r]!=g_char|| grid[i][r+4]!=h_char|| grid[i+4][r]!=h_char||grid[i][r+4]!=i_char||grid[i+4][r]!=i_char
                       ||grid[i][r]!=j_char||grid[i+4][r]!=k_char|| grid[i][r+4]!=l_char|| grid[i+4][r]!=l_char||grid[i][r+4]!=m_char||grid[i+4][r]!=m_char
                       ||grid[i][r]!=n_char|| grid[i+4][r]!=n_char|| grid[i][r+4]!=o_char|| grid[i+4][r]!=o_char||grid[i][r+4]!=p_char||grid[i+4][r]!=p_char
                       ||grid[i][r]!=q_char|| grid[i+4][r]!=q_char|| grid[i][r+4]!=r_char|| grid[i+4][r]!=r_char||grid[i][r+4]!=s_char||grid[i+4][r]!=s_char
                       ||grid[i][r]!=t_char|| grid[i+4][r]!=t_char|| grid[i][r+4]!=u_Char|| grid[i+4][r]!=u_Char||grid[i][r+4]!=v_char||grid[i+4][r]!=v_char
                       ||grid[i][r]!=w_char|| grid[i+4][r]!=w_char|| grid[i][r+4]!=x_char|| grid[i+4][r]!=x_char||grid[i][r+4]!=y_char||grid[i+4][r]!=y_char
                       ||grid[i][r]!=z_char|| grid[i+4][r]!=z_char|| grid[i][r+1]!=equal ||grid[i+1][r]!=wqual||grid[i][r+1]!=underscore||grid[i+1][r]!=underscore))
                        isfound=false;

                    if (isfound==false)
                     {
                         DestPoints[destPointCount].x =i; 
                         DestPoints[destPointCount].y =r;
                         DestPoints[destPointCount].id=d_char; 
                         destPointCount++;}
                 } 
                else if (grid[i][r]==s_char&&spawnPointCount<=49)
                {
                    SpawnPoints[spawnPointCount].x=i;
                    SpawnPoints[spawnPointCount].y=r;
                    spawnPointCount++;}
            }
            currentRow++; 
            continue;}
      }
    }
}
        
    


        int indx= stoi(Line.find("SWITCHES"))+1;            
            if (Line.find("SWITCHES:") != string::npos) 
               continue; 

            istringstream iss(Line);
            char alpha;                      //intialization for baki
            string mode;
            int init, k1, k2, k3, k4; 
            string state0;
            string state1;
            int letter;
            int index;
            // we follow ye wala format rough sa  we start A se         A PER_DIR 0 2 1 3 1 LEFT RIGHT
            if (iss>>letter>>mode>>init>>k1>>k2>>k3>>k4>>state0>>state1)
             {
                index=letter-'A';
                if (index>=0&&index<=25)
                 {
                    Switch[index].letter=letter;
                    Switch[index].currentState=init;
                    Switch[index].kUp=k1;
                    Switch[index].kRight=k2; 
                    Switch[index].kLeft=k4;                //ismein we assign up down left waghaira
                    Switch[index].kDown=k3;}}
        

        int indx= stoi(Line.find("TRAINS"))+1;
        traincount = 0;

            while (getline(file, Line))
             {
                if (!Line.empty()){

                istringstream iss(Line);
                int tick, x, y, direction;
                string SigColor;

                // isme we follow ye wala format        <tick> <x> <y> <direction> <color>
                if (iss >> tick >> x >> y >> direction >> color)
                 {
                    if (traincount<100)
                     { 
                        Train[traincount].spawnTicks=tick;
                        Train[traincount].currentx=x;
                        Train[traincount].currenty=y;
                        Train[traincount].direction=direction;
                        Train[traincount].train_id=traincount;
                        metric.totaltrains=traincount+1;
                        traincount++;}
                }
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

void writeMetrics(int deliver,int crash)    //couts the final matrix in matrix.txt poori details uski
 {
    ofstream file("metrics.txt");
    file<<"Crashed:"<<crash <<endl;
    file<<"Delivered:"<<deliver<<endl;}
    
