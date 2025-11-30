#include "app.h"
#include "../core/simulation_state.h"
#include "../core/simulation.h"
#include "../core/grid.h"
#include "../core/switches.h"
#include "../core/trains.h"
#include "../core/io.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdio>

using namespace std;

static sf::RenderWindow* g_window = nullptr;
static sf::Font g_font;

const unsigned int WINDOW_WIDTH = 1200;
const unsigned int WINDOW_HEIGHT = 900;
const float SIMULATION_TICK_TIME = 0.5f;

bool g_isPaused = true;
bool g_isStepMode = false;
bool g_isDragging = false;
int g_lastMouseX = 0;
int g_lastMouseY = 0;
float g_cellSize = 40.0f;
float g_gridOffsetX = 100.0f;
float g_gridOffsetY = 100.0f;

// ============================================================================
// SPRITE SHEET TEXTURES
// ============================================================================
sf::Texture signalSpriteSheet;
sf::Texture trainSpriteSheet;
sf::Texture tilesSpriteSheet;
sf::Texture switchSpriteSheet;
sf::Texture trackSpriteSheet;
sf::Sprite signalSprite;
sf::Sprite trainSprite;
sf::Sprite tilesSprite;
sf::Sprite switchSprite;
sf::Sprite trackSprite;

// ============================================================================
// HELPER FUNCTIONS
// ============================================================================
void gridToScreen(int gridX, int gridY, float* screenX, float* screenY)
{
    *screenX = g_gridOffsetX + gridY * g_cellSize;
    *screenY = g_gridOffsetY + gridX * g_cellSize;
}

void screenToGrid(float screenX, float screenY, int* gridX, int* gridY)
{
    *gridX = (int)((screenY - g_gridOffsetY) / g_cellSize);
    *gridY = (int)((screenX - g_gridOffsetX) / g_cellSize);
}

// ============================================================================
// SPRITE LOADING (optional - works without sprites)
// ============================================================================
bool loadAllSprites()
{
 
 if(!signalSpriteSheet.loadFromFile("1.png"))
     cout<<"error";
  else 
    {signalSprite.setTexture(signalSpriteSheet);
     signalSprite.setPosition(2,4);}
     
if (!trainSpriteSheet.loadFromFile("2.png"))
     cout<<"error 2";
else
   {trainSprite.setTexture(trainSpriteSheet);
   trainSprite.setPosition(3,4);}
   
if (!tilesSpriteSheet.loadFromFile("3.png"))
    cout<<"error 3";
 else
    {tilesSprite.setTexture(tilesSpriteSheet);
    tilesSprite.setPosition(5,7);}
    
if (!switchSpriteSheet.loadFromFile("4.png"))
  cout<<"error 4";
  else
    {switchSprite.setTexture(switchSpriteSheet);
    switchSprite.setPosition(3,5);}
    
if(!trackSpriteSheet.loadFromFile("5.png"))
   cout<<"error 5";
 else
    {trackSprite.setTexture(trackSpriteSheet);
    trackSprite.setPosition(3,6);}
      
   
    cout << "Graphics initialized (using geometric shapes)" << endl;
    return true;
}
 
void drawLine(float x1, float y1, float x2, float y2, int r, int g, int b)
{
    sf::Vertex line[2];
    line[0].position.x = x1;
    line[0].position.y = y1;
    line[0].color = sf::Color(r, g, b, 255);

    line[1].position.x = x2;
    line[1].position.y = y2;
    line[1].color = sf::Color(r, g, b, 255);

    g_window->draw(line, 2, sf::Lines);
}

void drawRectangle(float x, float y, float width, float height, int r, int g, int b, int a)
{
    sf::RectangleShape rect;
    rect.setPosition(x, y);
    rect.setSize(sf::Vector2f(width, height));
    rect.setFillColor(sf::Color(r, g, b, a));
    g_window->draw(rect);
}

void drawCircle(float x, float y, float radius, int r, int g, int b)
{
    sf::CircleShape circle(radius);
    circle.setPosition(x, y);
    circle.setFillColor(sf::Color(r, g, b, 255));
    g_window->draw(circle);
}

void drawText(const char* text, float x, float y, int size, int r, int g, int b)
{
    sf::Text sfText;
    sfText.setFont(g_font);
    sfText.setString(text);
    sfText.setCharacterSize(size);
    sfText.setPosition(x, y);
    sfText.setFillColor(sf::Color(r, g, b, 255));
    g_window->draw(sfText);
}

// ============================================================================
// RENDERING FUNCTIONS
// ============================================================================
void renderGrid()
{
    // Draw grid lines
    for (int i = 0; i <= rows; i++)
    {
        float x1, y1, x2, y2;
        gridToScreen(i, 0, &x1, &y1);
        gridToScreen(i, cols, &x2, &y2);
        drawLine(x1, y1, x2, y2, 60, 60, 60);
    }

    for (int j = 0; j <= cols; j++)
    {
        float x1, y1, x2, y2;
        gridToScreen(0, j, &x1, &y1);
        gridToScreen(rows, j, &x2, &y2);
        drawLine(x1, y1, x2, y2, 60, 60, 60);
    }

    // Draw tiles
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            char tile = grid[i][j];
           
            if (tile == ' ' || tile == '.')
            {
                continue;
            }

            float posX, posY;
            gridToScreen(i, j, &posX, &posY);

            // HORIZONTAL TRACKS
            if (tile == '=' || tile == '-')
            {
                drawRectangle(posX + 5, posY + g_cellSize * 0.35f,
                              g_cellSize - 10, g_cellSize * 0.3f, 255, 255, 255, 255);
            }
            // VERTICAL TRACKS
            else if (tile == '|')
            {
                drawRectangle(posX + g_cellSize * 0.35f, posY + 5,
                              g_cellSize * 0.3f, g_cellSize - 10, 255, 255, 255, 255);
            }
            // CROSSING
            else if (tile == '+')
            {
                drawRectangle(posX + 5, posY + g_cellSize * 0.35f,
                              g_cellSize - 10, g_cellSize * 0.3f, 255, 255, 255, 255);
                drawRectangle(posX + g_cellSize * 0.35f, posY + 5,
                              g_cellSize * 0.3f, g_cellSize - 10, 255, 255, 255, 255);
            }
            // SPAWN POINT
            else if (tile == 'S')
            {
                drawRectangle(posX + 2, posY + 2, g_cellSize - 4, g_cellSize - 4, 50, 150, 255, 255);
                drawText("S", posX + g_cellSize * 0.3f, posY + g_cellSize * 0.2f,
                         (int)(g_cellSize * 0.6f), 255, 255, 255);
            }
            // DESTINATION
            else if (tile == 'D')
            {
                drawRectangle(posX + 2, posY + 2, g_cellSize - 4, g_cellSize - 4, 255, 50, 50, 255);
                drawText("D", posX + g_cellSize * 0.25f, posY + g_cellSize * 0.2f,
                         (int)(g_cellSize * 0.6f), 255, 255, 255);
            }
            // SWITCHES
            else if (tile >= 'A' && tile <= 'Z')
            {
                drawRectangle(posX + 2, posY + 2, g_cellSize - 4, g_cellSize - 4, 255, 220, 0, 255);
               
                char switchText[2];
                switchText[0] = tile;
                switchText[1] = '\0';
                drawText(switchText, posX + g_cellSize * 0.3f, posY + g_cellSize * 0.2f,
                         (int)(g_cellSize * 0.5f), 0, 0, 0);
            }
            // DIAGONAL TRACKS
            else if (tile == '/')
            {
                drawLine(posX, posY + g_cellSize, posX + g_cellSize, posY, 255, 255, 255);
                drawLine(posX + 3, posY + g_cellSize, posX + g_cellSize, posY + 3, 255, 255, 255);
            }
            else if (tile == '\\')
            {
                drawLine(posX, posY, posX + g_cellSize, posY + g_cellSize, 255, 255, 255);
                drawLine(posX + 3, posY, posX + g_cellSize, posY + g_cellSize - 3, 255, 255, 255);
            }
            // NUMBERS
            else if (tile >= '0' && tile <= '9')
            {
                drawRectangle(posX + 2, posY + 2, g_cellSize - 4, g_cellSize - 4, 0, 200, 200, 255);
               
                char numText[2];
                numText[0] = tile;
                numText[1] = '\0';
                drawText(numText, posX + g_cellSize * 0.3f, posY + g_cellSize * 0.2f,
                         (int)(g_cellSize * 0.5f), 255, 255, 255);
            }
        }
    }
}


void renderTrains()
{
    for (int i = 0; i < NumTrains; i++)
    {

        if (train_status[i] == TRAIN_INACTIVE || train_status[i] == TRAIN_ARRIVED)
            continue;

        int x = train_x[i];
        int y = train_y[i];

        if (!isInBounds(x, y))
        {
            cout << "WARNING: Train " << i << " at invalid position (" << x << "," << y << ")" << endl;
            continue;
        }

        float posX, posY;
        gridToScreen(x, y, &posX, &posY);

        
        int R = 0, G = 200, B = 0; // GREEN = MOVING
       
        if (train_status[i] == TRAIN_CRASHED)
        {
            R = 255; G = 0; B = 0; 
        }
        else if (train_status[i] == TRAIN_WAITING)
        {
            R = 255; G = 165; B = 0; 
        }
        else if (train_status[i] == TRAIN_DELAYED)
        {
            R = 255; G = 255; B = 0; 
        }


        drawRectangle(
            posX + 4,
            posY + 4,
            g_cellSize - 8,
            g_cellSize - 8,
            R, G, B, 255
        );

        // Draw train ID
        char idText[3];
        sprintf(idText, "%d", i);
        drawText(
            idText,
            posX + g_cellSize * 0.35f,
            posY + g_cellSize * 0.25f,
            (int)(g_cellSize * 0.4f),
            255, 255, 255
        );
    }
}


void renderSignalLights()
{
    for (int i = 0; i < NumSwitches; i++)
    {
        char letter = switch_letter[i];
        if (letter < 'A' || letter > 'Z')
            continue;


        int sx = switch_x[i];
        int sy = switch_y[i];
       
        if (!isInBounds(sx, sy))
            continue;

        float posX, posY;
        gridToScreen(sx, sy, &posX, &posY);


        int R = 0, G = 255, B = 0; // GREEN
        if (signallights[i] == RED)
        {
            R = 255; G = 0; B = 0;
        }
        else if (signallights[i] == YELLOW)
        {
            R = 255; G = 255; B = 0;
        }

        float radius = g_cellSize * 0.15f;
       
        // Draw signal in top-right corner
        drawCircle(posX + g_cellSize * 0.7f, posY + g_cellSize * 0.1f, radius, R, G, B);
    }
}

void renderUI()
{
    // Background bar
    drawRectangle(0, 0, WINDOW_WIDTH, 85, 0, 0, 0, 200);

    // Tick
    char tickText[50];
    sprintf(tickText, "Tick: %d", currentTick);
    drawText(tickText, 10, 10, 22, 255, 255, 255);

    // Calculate active trains correctly
    int active = 0;
    for (int i = 0; i < NumTrains; i++)
    {
        if (train_status[i]==TRAIN_MOVING||train_status[i]==TRAIN_WAITING||train_status[i]==TRAIN_DELAYED)
        { active++;}     }
   
    char metricsText[250];
    sprintf(metricsText,"Trains: %d | Active: %d | Delivered: %d | Crashed: %d",
            NumTrains, active, metric_delivered, metric_crashed);
    drawText(metricsText,10,38,18,100,255,100);
//all of these will be shown ooper extereme top
    const char* controlsText = "SPACE = Pause,  . = Step,  LEFT = Safety,  RIGHT = Switch,  SCROLL = Zoom, MIDDLE = Camera Panning,  ESC= Exit Window";
    drawText(controlsText, 10, 63, 14, 200, 200, 200);

    if (g_isPaused)
    {
        drawRectangle(WINDOW_WIDTH/2-150,WINDOW_HEIGHT/2-30,300,60,255,0,0,220);
        drawText("Game Paused",WINDOW_WIDTH/ 2-70,WINDOW_HEIGHT/2-15,40,255,255,255);
    }

    if (isSimulationComplete())
    {
        drawRectangle(WINDOW_WIDTH/2-280,WINDOW_HEIGHT/2-50,560,100,0,200,0,240);
        drawText("Simulation Is Complete!",WINDOW_WIDTH/2-200,WINDOW_HEIGHT/2-20,38,255,255, 255);}
}

void renderSimulationState()
{
    renderGrid();
    renderSignalLights();             //extra funcs to make the UI things and for signals n trains n making grids waghaira
    renderTrains();
    renderUI();
}

bool initializeApp()
{
    currentTick = 0;     //agar its not zero tou the tick time starts v randomly n messes up the thing
    g_window=new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),  //makes ovrall
                                    "Train Simulation - PF Final Project");          //window 

    if (g_window == nullptr)
    {
        cout << "Error creating window." << endl;
        return false;
    }

    g_window->setFramerateLimit(60);//framerate 60 since thats whats the most sued typa

    if (!g_font.loadFromFile("/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf"))
    {
        if (!g_font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"))
        {
            if (!g_font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
            {
                cout<<"Could not load font"<<endl;}   //we load all our fonts files yahan taka theres one font thats used poori game mein
        }
    }

    loadAllSprites();
    return true;
}

void runApp()
{
    cout<<"Running application in 3...2...1" << endl;
     if (!initializeApp())
    {
        cout<<"Failed to initialize"<<endl;
        return; }
   
    cout<<"Grid: "<<rows<< " x "<<cols<<endl;
    cout<<"Trains: "<<NumTrains<<endl;

    sf::Clock clock;
    float lag = 0.0f;
    g_isPaused = true;
    while (g_window->isOpen())
    {
        float deltaTime = clock.restart().asSeconds();
        lag += deltaTime;
        sf::Event event;
        while (g_window->pollEvent(event))
        {
          cout<<"working!"<<endl;
            if (event.type == sf::Event::Closed)
            {
                writeMetrics();    //yahan we check ke konsi key from keyboard is pressed
                g_window->close();}                //each key has its alag func
            else if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {                             //esc ends the ga,e n eberything saved in the metrics
                    writeMetrics();
                    g_window->close();}
                else if (event.key.code == sf::Keyboard::Space)
                {
                    g_isPaused = !g_isPaused;
                    g_isStepMode = false;
                }
                else if (event.key.code == sf::Keyboard::Period)
                {
                    if (g_isPaused)
                    {
                        g_isStepMode = true;
                    }
                }
            }
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    int gridX, gridY;
                    screenToGrid((float)event.mouseButton.x, (float)event.mouseButton.y, &gridX, &gridY);
                    toggleSafetyTile(gridX, gridY);               }
                else if (event.mouseButton.button == sf::Mouse::Right)
                {
                    int gridX, gridY;
                    screenToGrid((float)event.mouseButton.x, (float)event.mouseButton.y, &gridX, &gridY);

                    if (isInBounds(gridX, gridY))
                    {
                       trainSprite.setPosition(gridX,gridY);
                       cout<<trainSprite.getPosition().x<<endl;
                       cout<<trainSprite.getPosition().y<<endl;
                        char tile = grid[gridX][gridY];
                        if (isSwitchTile(tile))
                        {
                            
                            int switchIndex = getSwitchIndex(tile);
                            if (switchIndex >= 0)
                            {
                                
                                toggleSwitchState(switchIndex);  }  }
                    }
                }
                else if (event.mouseButton.button == sf::Mouse::Middle)
                {
                    g_isDragging = true;
                    g_lastMouseX = event.mouseButton.x;
                    g_lastMouseY = event.mouseButton.y;  }
            }
            else if (event.type == sf::Event::MouseButtonReleased)
            {
                if (event.mouseButton.button == sf::Mouse::Middle)
                {
                    g_isDragging = false;}
            }
            else if (event.type == sf::Event::MouseMoved)
            {
                if (g_isDragging)    //yahan we update screen ka size when mouse pad moved
                {
                    int deltaX=g_lastMouseX-event.mouseMove.x;
                    int deltaY=g_lastMouseY-event.mouseMove.y;
                    g_gridOffsetX-= deltaX;
                    g_gridOffsetY-= deltaY;     
                    g_lastMouseX=event.mouseMove.x;
                    g_lastMouseY=event.mouseMove.y;      
                }
            }
            else if (event.type == sf::Event::MouseWheelScrolled)
            {
                if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
                {
                    float zoomFactor = (event.mouseWheelScroll.delta > 0) ? 1.1f : 0.9f;
                    g_cellSize *= zoomFactor;   //zooming in pe size inc
                                               //zoom out pe dec
                    if (g_cellSize<10.0f) 
                        {g_cellSize=10.0f;}
                   else if (g_cellSize>100.0f) 
                        { g_cellSize=100.0f;}           }
            }
        }

        if (g_isStepMode)
        {
            simulateOneTick();
            g_isStepMode = false;
            lag = 0.0f; }
        else if (!g_isPaused)
        {
            while (lag >= SIMULATION_TICK_TIME)
            {
                simulateOneTick();
                lag -= SIMULATION_TICK_TIME;}
        }

        g_window->clear(sf::Color(30, 30, 30));
        renderSimulationState();
        g_window->display(); }               

    writeMetrics();   //all metrics wali things will be written in the txt metrics ki after its closed in the out dir
}

void cleanupApp()
{                      //this clears window n sets the val to null ptr cuz we made it dynamic
    if (g_window)
    {
        delete g_window;
        g_window = nullptr;
    }
}
