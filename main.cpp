#include "app.h"

#include "../core/simulation_state.h"
#include "../core/simulation.h"
#include "../core/io.h"
#include <iostream>

#include <string>
using namespace std;

int main()
{
    int level;
    cout << "----------Welcome to Asna and Zainab's Train simulation game-------------" << endl;
    cout << "Choose between 1,2,3,4 for game level complexity" << endl;
    cout << "1. Easy Level" << endl;
    cout << "2. Medium Level" << endl;
    cout << "3. Hard Level" << endl;
    cout << "4. Complex Level" << endl;
    cout << "Enter your choice: ";
    cin >> level;
   

    if (cin.fail()) {
        cout << "Invalid input. Please enter a number between 1 and 4." << endl;
        return 1;
    }
   
    bool done = false;
    string levelFile;
   
    switch (level)
    {
        case 1:
        {
            levelFile = "easy_level.lvl";
            break;
        }
        case 2:
        {
            levelFile = "medium_level.lvl";
            break;
        }
        case 3:
        {
            levelFile = "hard_level.lvl";
            break;
        }
        case 4:
        {
            levelFile = "complex_network.lvl";
            break;
        }
        default:
        {
            cout<<"Invalid level entered :("<<endl;
            cout<<"Please choose a number between 1 and 4."<<endl;
            return 1;
        }
    }
   
    done=loadLevelFile(levelFile);
   
    if (done==false)
    {
        cout << "Error: Failed to load level file "<< endl;
        cout << "Please ensure the file is in the correct directory." << endl;
        return 1;
    }
   
    else
    {
    cout << "Level loaded successfully!" << endl;
        cout << "Initializing simulation..." << endl;
   
   
    if (!initializeApp())
    {    
        cout << "Error: Failed to initialize application window" << endl;
        return 1;
    }
    initializeApp();    

    cout<<"=+=+=+=+=+=+=+=+=+=CONTROLS=+=+=+=+=+=+=+=+" << endl;
    cout<<"IMPORTANT!!" << endl;
    cout<<" press *SPACE*....... Pause or resume simulation" << endl;
    cout<<" press *ESC*  ....... End the simulation" << endl;
    cout<<" press *.*    ....... Step forward one tick (when paused)" << endl;
    cout<<" use *Left-click*.... Toggle safety tile (=)" << endl;
    cout<<" use *Right-click*... Toggle switch state" << endl;
    cout<<" use *Middle-drag*... Pan camera view" << endl;
    cout<<" use *Mouse wheel*... Zoom in/out" << endl;
    cout<<"=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+" << endl;

    cout<<"Starting simulation..." << endl;
    cout<<"The game begins PAUSED. Press SPACE to start!" << endl;
    cout<<endl;
    runApp();
   
    renderSimulationState();
   
    cleanupApp();

   
    cout<<endl;
    cout<<"=+=+=+=+=+=+=Simulation Complete=+=+=+=+=+="<<endl;
    cout<<"End of game reached!!"<<endl;
    cout<<"Thank you for playing!"<<endl;
}
    return 0;
}
