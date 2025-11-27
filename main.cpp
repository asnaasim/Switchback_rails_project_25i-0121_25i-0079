#include "app.h"
#include "sprites"
#include "../core/simulation_state.h"
#include "../core/simulation.h"
#include "../core/io.h"
#include <iostream>
#include "sprites.h"
#include <string>
using namespace std;

int main() 
{
    int level;
  cout<<"----------Welcome to Asna and Zainab's Train simulation game-------------"<<endl;
  cout<<"Choose between 1,2,3,4 for game level complexity"<<endl;
  cout<<"1. Easy Level"<<endl;
  cout<<"2. Medium Level"<<endl;
  cout<<"3. Hard Level"<<endl;
  cout<<"4. Complex Level"<<endl;
  cin>>level;
  bool done=false;
  switch (level)
  {
    case 4:
      {                       //use of switches to check konsi file user wants to load uske liya we use switch cuz 4 options n 5th is default
        done=loadLevelFile("complex_network.lvl");
        break;
      }
    case 2:
    {
        done=loadLevelFile("medium_level.lvl");
        break;
    }  
    case 3:
    {
        done=loadLevelFile("hard_level.lvl");
        break;
    }                                                      //bas loads files onto func
    case 1:
    {
        done=loadLevelFile("easy_level.lvl");
        break;
    }
    default:
      {
        cout<<"Invalid level entered :("<<endl;
        return 1;
        break;
      }
  }
  if (done==false)
  {
    cout<<"error file not loaded"<<endl;
    return 1;
  }
  else if (done==true)
  {
      initializeSimulation();    //yahan we initialise the simulation
      initializeApp();                   //yahan se sfml is activated
      cout<<"IMPORTANT!!"<<endl;
      cout <<"=+=+=+=+=+=+=+=+=+=CONTROLS=+=+=+=+=+=+=+=+"<<endl;         //we cout all the commands to the user
      cout<<"press *SPACE* to pause or resume"<<endl;
      cout<<"press *ESC* to end the simulation"<<endl;
      cout<<"press *.* to make move forward one tick"<<endl;
      cout<<"use *Left-click* to Toggle safety tile (=)"<<endl;
     cout<<"use *Right-click* to Toggle switch state"<<endl;
     cout<<"use *Middle-drag* to Pan camera"<<endl;
      cout<<"use *Mouse wheel* to Zoom in/out"<<endl;
      cout <<"=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+"<<endl;
      runApp();
      cleanupApp();

      cout <<"=+=+=Simulation Complete=+=+="<<endl;         //this is displayed once simulation is completed
      cout<<"end of game reached!!"<<endl;

      return 0;
  }     

}
