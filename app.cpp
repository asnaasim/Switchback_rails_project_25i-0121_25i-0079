#include "app.h"
#include "../core/simulation_state.h"
#include "../core/simulation.h"
#include "../core/grid.h"
#include "../core/switches.h"
#include "../core/io.h"
#include "sprites.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdio>
using namespace std;

static sf::RenderWindow* g_window=nullptr;
static sf::Font g_font;

const unsigned int WINDOW_WIDTH=400;
const unsigned int WINDOW_HEIGHT=400;
const float SIMULATION_TICK_TIME=0.5f;

static sf::View g_camera;

static bool g_isPaused = false;
static bool g_isStepMode = false;

static bool g_isDragging = false;
static int g_lastMouseX = 0;
static int g_lastMouseY = 0;

static float g_cellSize = 40.0f;
static float g_gridOffsetX = 45.0f;
static float g_gridOffsetY = 45.0f;


bool initializeApp()
{
    const unsigned int Frame_rate=60;  //unsigned cuz framerate cant be neg
                                          //this sets vals for video ki sizing
    g_window=new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH,WINDOW_HEIGHT),"Train Simulation - PF Final Project");
    if (g_window==nullptr)
    {                                         
        cout<<"Error creating SFML window."<<endl;
        return false;
    }

    g_window->setFramerateLimit(Frame_rate);                         //sets up camview epecification
    sf::View cameraView(sf::FloatRect(0.f, 0.f,(float)WINDOW_WIDTH,(float)WINDOW_HEIGHT));
    g_window->setView(cameraView);

    if (!g_font.loadFromFile("arial.ttf"))            //this fulfills req ke agar na ho fontfile tou ret false
    {
        cout<<"Error loading the font file try again :("<<endl;
        delete g_window;
        g_window=nullptr; 
        return false;                //delete ptr agar load na ho cuz we wanna try again phir
    }                                 //null ptr to avoid mem overflow

    return true; 
} 

void runApp() 
{
    sf::Clock clock;   
    g_isPaused=true;
    g_isStepMode=false;
    g_running=false;
    g_isDragging=false;
    g_isStepMode=false;
    float lag = 0.0f;    
    g_window->setView(g_camera); 
        
    if (initializeApp()==true)
    {
        while (g_window->isOpen())
        {
            float deltaTime =clock.restart().asSeconds();
            lag=lag+deltaTime;
            sf::Event event;
            while (g_window->pollEvent(event)) 
            {
                sf::Vector2i pixelPos=sf::Mouse::getPosition(*g_window);
                sf::Vector2f pos=g_window->mapPixelToCoords(pixelPos, g_camera);

             if (event.type==sf::Event::Closed)
                {
                    writeMetrics();
                    g_window->close();
                }
                else if(event.type==sf::Event::KeyPressed)
                {
                    switch(event.key.code)
                    {
                        case sf::keyboard::Escape :
                    {
                        writeMetrics();                //need to write matrics jab window closed in matrics.txt wali output directory
                        g_window->close();
                        break;                          
                    }
                        case sf::keyboard::Space :
                    {                            //space means pause ya start agar its paused pehla tou on pressing space it starts moving
                        g_isPaused=!g_isPaused;           //so cant set it true ya false but as negation of pichli val of it
                        g_isStepMode=false;
                        break;
                    }
                        case sf::keyboard::Period :     
                    {                                //pressing period to tell agay step
                        if (g_isPaused==true)
                            g_isStepMode = true; 
                            break;
                    }
                    default:            
                    {   cout<<"invalid keyboard key pressed"<<endl;
                        break;}
                    }
                }
                else if(event.type==sf::Event::MouseButtonPressed)
                  {
                    switch(event.mouseButton.button)             //yahan mouse commands start for zooming in zooming out cam panning
                    {
                        case sf::Mouse::Right :
                        {
                           toggleSwitchState(pos);
                           break;
                        }
                        case sf::Mouse::Left :
                        {
                           toggleSafetyTile(pos.x,pos.y);  
                           break;
                        }
                        case sf::Mouse::Middle :
                        {
                            g_isDragging=true;
                            g_lastMouseX=event.mouseButton.x;
                            g_lastMouseY=event.mouseButton.y;
                            break;
                        }
                        default:
                        {cout<<"invalid command!!"<<endl;
                        break;}
                    }
                  }
                  else if (event.type==sf::Event::MouseButtonReleased)
                 {
                     if (event.mouseButton.button==sf::Mouse::Middle)
                    {
                        g_isDragging=false;
                    }
                  }
            
                 else if (event.type==sf::Event::MouseMoved)
                 {
                   if (g_isDragging==true) 
                  {
                    float dx=(float)(event.mouseMove.x-g_lastMouseX);
                    float dy=(float)(event.mouseMove.y-g_lastMouseY);   //this moves
                    
                    g_camera.move(-dx*0.5f,-dy*0.5f);    
                    g_window->setView(g_camera);
                    g_lastMouseX =event.mouseMove.x;
                    g_lastMouseY =event.mouseMove.y;
                   }                                     
                  }
               else if (event.type==sf::Event::MouseWheelScrolled) //mouse scrolling command starts yahan se
                {
                   if (event.mouseWheelScroll.wheel==sf::Mouse::VerticalWheel)
                  {
                     float zoomFactor;
                     if (event.mouseWheelScroll.delta>0)
                       zoomFactor=1.4f;
                     else if(event.mouseWheelScroll.delta<0) 
                       zoomFactor=0.8f;                        //zooming wala factor fullfilled inc in size and dec in size wali chez happns yahn
                     g_camera.zoom(zoomFactor);
                     g_window->setView(g_camera);
                   }
-                }
            if (g_isStepMode==true)             //this means it wud move and so we will call simulateonetick takay movement
            {
                simulateOneTick(); 
                g_isStepMode=false;
                lag=0.0f;                    //have to reset lag takay its zero for agli games ki turns
            }

            if (!g_isPaused)
            {
                while (lag>=SIMULATION_TICK_TIME)
                {
                    simulateOneTick();                      //hv to call the func takay keeps on going n movement occurs
                    lag=lag-SIMULATION_TICK_TIME;
                }
            }
               }
              g_window->clear(sf::Color(0xFF8800FF));
             
              renderSimulationState();     //this comes from dosri demo spries sfml wali folder

            if (g_isPaused==true)    //this means pause hui we cuz space bar pressed tou we display ke paused n press pace bar to restart
            {
                sf::Text statusText("GAME PAUSED PRESS *SPACE BAR* TO CONTINUE",g_font,20);
                statusText.setFillColor(sf::Color::Purple);
                g_window->draw(statusText);
                                }                               //this sets the text to be display jab game is paused text ka font to be 20
            if (isSimulationComplete()==true)
            {   
                sf::Text completeText("SIMULATION COMPLETE, GAME COMPLETE!", g_font,30);
                completeText.setFillColor(sf::Color::Red);              //helps user to know game is complete
                g_window->draw(completeText);                        //this sets the text to be display jab game is paused text ka font to be 30
                                     }
              g_window->display();
        }
    }  
    if (g_window->isClosed())
        writeMetrics();}               //we exit game is point pw

void cleanupApp()
 {
     if (g_window)                 //this js clears pooro screen n resets 
      {
        delete g_window;
        g_window = nullptr;}}

