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

// ============================================================================
// GLOBAL WINDOW AND RENDERING STATE
// ============================================================================
static sf::RenderWindow* g_window = nullptr;
static sf::Font g_font;

const unsigned int WINDOW_WIDTH = 900;
const unsigned int WINDOW_HEIGHT =900;
const float SIMULATION_TICK_TIME = 0.5f;

bool g_isPaused = true;
bool g_isStepMode = false;
bool g_isDragging = false;
int g_lastMouseX = 0;
int g_lastMouseY = 0;
float g_cellSize = 40.0f;
float g_gridOffsetX = 50.0f;
float g_gridOffsetY = 50.0f;

// ============================================================================
// SPRITE SHEET TEXTURES (32x32 sprites in 3x3 grids)
// ============================================================================
sf::Texture signalSpriteSheet; // 1.png: signals (3x3 grid, 32x32 each)
sf::Texture trainSpriteSheet; // 2.png: trains (3x3 grid, 32x32 each)
sf::Texture tilesSpriteSheet; // 3.png: tiles (3x3 grid, 32x32 each)
sf::Texture switchSpriteSheet; // 4.png: switches (3x3 grid, 32x32 each)
sf::Texture trackSpriteSheet; // 5.png: tracks (3x3 grid, 32x32 each)

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
// SPRITE SHEET LOADING
// ============================================================================
bool loadAllSprites()
{
 if (!signalSpriteSheet.loadFromFile("1.png"))
 {
 cout << "Failed to load 1.png (signal sprites)" << endl;
 return false;
 }

 if (!trainSpriteSheet.loadFromFile("2.png"))
 {
 cout << "Failed to load 2.png (train sprites)" << endl;
 return false;
 }

 if (!tilesSpriteSheet.loadFromFile("3.png"))
 {
 cout << "Failed to load 3.png (tile sprites)" << endl;
 return false;
 }

 if (!switchSpriteSheet.loadFromFile("4.png"))
 {
 cout << "Failed to load 4.png (switch sprites)" << endl;
 return false;
 }

 if (!trackSpriteSheet.loadFromFile("5.png"))
 {
 cout << "Failed to load 5.png (track sprites)" << endl;
 return false;
 }

 cout << "All sprite sheets loaded successfully!" << endl;
 return true;
}


void drawSpriteFromSheet(sf::Texture* texture, int spriteIndex, float x, float y, float scale)
{
 if (texture == nullptr || texture->getSize().x == 0)
 {
 return;
 }

 sf::Sprite sprite;
 sprite.setTexture(*texture);


 int col = spriteIndex % 3;
 int row = spriteIndex / 3;


 sf::IntRect rect;
 rect.left = col * 32;
 rect.top = row * 32;
 rect.width = 32;
 rect.height = 32;

 sprite.setTextureRect(rect);
 sprite.setPosition(x, y);
 sprite.setScale(scale, scale);
 g_window->draw(sprite);
}

void drawLine(float x1, float y1, float x2, float y2, int r, int g, int b)
{
 sf::Vertex line[2];
 line[0].position.x = x1;
 line[0].position.y = y1;
 line[0].color.r = r;
 line[0].color.g = g;
 line[0].color.b = b;
 line[0].color.a = 255;

 line[1].position.x = x2;
 line[1].position.y = y2;
 line[1].color.r = r;
 line[1].color.g = g;
 line[1].color.b = b;
 line[1].color.a = 255;

 g_window->draw(line, 2, sf::Lines);
}

void drawRectangle(float x, float y, float width, float height, int r, int g, int b, int a)
{
 sf::RectangleShape rect;
 rect.setPosition(x, y);
 sf::Vector2f size;
 size.x = width;
 size.y = height;
 rect.setSize(size);
 rect.setFillColor(sf::Color(r, g, b, a));
 g_window->draw(rect);
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
void renderGrid()
{
    // Draw grid lines
    int i = 0;
    while (i <= rows)
    {
        float x1, y1, x2, y2;
        gridToScreen(i, 0, &x1, &y1);
        gridToScreen(i, cols, &x2, &y2);
        drawLine(x1, y1, x2, y2, 80, 80, 80);
        i++;
    }

    int j = 0;
    while (j <= cols)
    {
        float x1, y1, x2, y2;
        gridToScreen(0, j, &x1, &y1);
        gridToScreen(rows, j, &x2, &y2);
        drawLine(x1, y1, x2, y2, 80, 80, 80);
        j++;
    }

    // Draw tiles - CHECK EVERY SINGLE TILE TYPE
    i = 0;
    while (i < rows)
    {
        j = 0;
        while (j < cols)
        {
            char tile = grid[i][j];
           
            if (tile == ' ' || tile == '.')
            {
                j++;
                continue;
            }

            float posX, posY;
            gridToScreen(i, j, &posX, &posY);
            float scale = g_cellSize / 32.0f;

            // Draw based on tile type - SIMPLIFIED, NO SPRITES
            if (tile == '=')
            {
                // Horizontal track - WHITE (like in your screenshot)
                drawRectangle(posX + 5, posY + g_cellSize * 0.35f,
                              g_cellSize - 10, g_cellSize * 0.3f, 255, 255, 255, 255);
            }
            else if (tile == '|')
            {
                // Vertical track - WHITE
                drawRectangle(posX + g_cellSize * 0.35f, posY + 5,
                              g_cellSize * 0.3f, g_cellSize - 10, 255, 255, 255, 255);
            }
            else if (tile == '+')
            {
                // Cross track - WHITE both ways
                drawRectangle(posX + 5, posY + g_cellSize * 0.35f,
                              g_cellSize - 10, g_cellSize * 0.3f, 255, 255, 255, 255);
                drawRectangle(posX + g_cellSize * 0.35f, posY + 5,
                              g_cellSize * 0.3f, g_cellSize - 10, 255, 255, 255, 255);
            }
            else if (tile == 'S')
            {
                // Spawn - BRIGHT BLUE RECTANGLE + TEXT
                drawRectangle(posX + 2, posY + 2, g_cellSize - 4, g_cellSize - 4, 50, 150, 255, 255);
                drawText("S", posX + g_cellSize * 0.3f, posY + g_cellSize * 0.2f,
                         (int)(g_cellSize * 0.6f), 255, 255, 255);
            }
            else if (tile == 'D')
            {
                // Destination - BRIGHT RED RECTANGLE + TEXT
                drawRectangle(posX + 2, posY + 2, g_cellSize - 4, g_cellSize - 4, 255, 50, 50, 255);
                drawText("D", posX + g_cellSize * 0.25f, posY + g_cellSize * 0.2f,
                         (int)(g_cellSize * 0.6f), 255, 255, 255);
            }
            else if (tile >= 'A' && tile <= 'Z')
            {
                // Switches - YELLOW RECTANGLE + LETTER
                drawRectangle(posX + 2, posY + 2, g_cellSize - 4, g_cellSize - 4, 255, 220, 0, 255);
               
                char switchText[2];
                switchText[0] = tile;
                switchText[1] = '\0';
                drawText(switchText, posX + g_cellSize * 0.3f, posY + g_cellSize * 0.2f,
                         (int)(g_cellSize * 0.5f), 0, 0, 0);
            }
            else if (tile == '-')
            {
                // Alternative horizontal track
                drawRectangle(posX + 5, posY + g_cellSize * 0.35f,
                              g_cellSize - 10, g_cellSize * 0.3f, 255, 255, 255, 255);
            }
            else if (tile == '/')
            {
                // Diagonal track
                drawLine(posX, posY + g_cellSize, posX + g_cellSize, posY, 255, 255, 255);
                drawLine(posX + 3, posY + g_cellSize, posX + g_cellSize, posY + 3, 255, 255, 255);
            }
            else if (tile == '\\')
            {
                // Diagonal track
                drawLine(posX, posY, posX + g_cellSize, posY + g_cellSize, 255, 255, 255);
                drawLine(posX + 3, posY, posX + g_cellSize, posY + g_cellSize - 3, 255, 255, 255);
            }
            else if (tile >= '0' && tile <= '9')
            {
                // Numbers (like 0, 2, 3, 4 in your map) - CYAN
                drawRectangle(posX + 2, posY + 2, g_cellSize - 4, g_cellSize - 4, 0, 200, 200, 255);
               
                char numText[2];
                numText[0] = tile;
                numText[1] = '\0';
                drawText(numText, posX + g_cellSize * 0.3f, posY + g_cellSize * 0.2f,
                         (int)(g_cellSize * 0.5f), 255, 255, 255);
            }

            j++;
        }
        i++;
    }
}

void renderTrains()
{
 int i = 0;
 while (i < NumTrains)
 {
 if (train_status[i] == TRAIN_MOVING ||
 train_status[i] == TRAIN_WAITING ||
 train_status[i] == TRAIN_DELAYED)
 {
 int x = train_x[i];
 int y = train_y[i];

 if (!isInBounds(x, y))
 {
 i++;
 continue;
 }

 float posX, posY;
 gridToScreen(x, y, &posX, &posY);

 // Scale to fit cell (sprites are 32x32)
 float scale = g_cellSize / 32.0f;

 // 2.png TRAINS - 3x3 grid layout:
 // Row 0: [0]up-left [1]up-mid [2]up-right
 // Row 1: [3]left [4]mid [5]right
 // Row 2: [6]down-left [7]down-mid [8]down-right

 int spriteIdx = 4; // Default middle
 if (train_direction[i] == DIR_UP)
 {
 spriteIdx = 1; // Top middle
 }
 else if (train_direction[i] == DIR_RIGHT)
 {
 spriteIdx = 5; // Middle right
 }
 else if (train_direction[i] == DIR_DOWN)
 {
 spriteIdx = 7; // Bottom middle
 }
 else if (train_direction[i] == DIR_LEFT)
 {
 spriteIdx = 3; // Middle left
 }

 drawSpriteFromSheet(&trainSpriteSheet, spriteIdx, posX, posY, scale);

 // Draw train ID
 char idText[10];
 sprintf(idText, "%d", i);
 drawText(idText, posX + g_cellSize * 0.35f, posY + g_cellSize * 0.25f,
 (int)(g_cellSize * 0.3f), 255, 255, 255);
 }

 i++;
 }
}

void renderSignalLights()
{
 int i = 0;
 while (i < NumSwitches)
 {
 if (switch_letter[i] >= 'A' && switch_letter[i] <= 'Z')
 {
 // Find switch position in grid
 bool found = false;
 int sx = 0;
 int sy = 0;

 int r = 0;
 while (r < rows && !found)
 {
 int c = 0;
 while (c < cols && !found)
 {
 if (grid[r][c] == switch_letter[i])
 {
 sx = r;
 sy = c;
 found = true;
 }
 c++;
 }
 r++;
 }

 if (found)
 {
 float posX, posY;
 gridToScreen(sx, sy, &posX, &posY);

 // Scale for signal light (smaller)
 float scale = g_cellSize / 64.0f;

 // Draw signal in top-left corner
 float signalX = posX + g_cellSize * 0.05f;
 float signalY = posY + g_cellSize * 0.05f;

 // 1.png SIGNALS - 3x3 grid layout:
 // Row 0: [0]green-full [1]yellow-full [2]red-full
 // Row 1: [3]green-half [4]yellow-half [5]red-half
 // Row 2: [6]unused [7]unused [8]unused

 int spriteIdx = 2; // Default red
 if (switch_signalcolour[i] == GREEN)
 {
 spriteIdx = 0; // Green
 }
 else if (switch_signalcolour[i] == YELLOW)
 {
 spriteIdx = 1; // Yellow
 }

 drawSpriteFromSheet(&signalSpriteSheet, spriteIdx, signalX, signalY, scale);
 }
 }

 i++;
 }
}

void renderUI()
{
 // Semi-transparent background
 drawRectangle(0, 0, WINDOW_WIDTH, 80, 0, 0, 0, 180);

 // Tick counter
 char tickText[50];
 sprintf(tickText, "Tick: %d", currentTick);
 drawText(tickText, 10, 10, 20, 255, 255, 255);

 // Metrics
 int active = NumTrains - metric_delivered - metric_crashed;
 char metricsText[200];
 sprintf(metricsText, "Trains: %d | Active: %d | Delivered: %d | Crashed: %d",
 NumTrains, active, metric_delivered, metric_crashed);
 drawText(metricsText, 10, 35, 18, 0, 255, 255);

 // Controls
 const char* controlsText = " Controls *SPACE*=Pause *PERIOD*=Step  *LEFT*=Safety *RIGHT*=Switch SAFETY TILES *SCROLL*=Zoom in/out *MIDDLE DRAG*= Camera Panning *ESC*=Exit Game";
 drawText(controlsText, 10, 60, 14, 200, 200, 200);

 // Pause indicator
 if (g_isPaused)
 {
 drawRectangle(WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 - 30, 300, 60, 255, 0, 0, 200);
 drawText("PAUSED", WINDOW_WIDTH / 2 - 60, WINDOW_HEIGHT / 2 - 20, 40, 255, 255, 255);
 }

 // Complete indicator
 if (isSimulationComplete())
 {
 drawRectangle(WINDOW_WIDTH / 2 - 250, WINDOW_HEIGHT / 2 - 40, 500, 80, 0, 255, 0, 220);
 drawText("SIMULATION COMPLETE!", WINDOW_WIDTH / 2 - 180, WINDOW_HEIGHT / 2 - 20, 35, 0, 0, 0);
 }
}

void renderSimulationState()
{
 renderGrid();
 renderSignalLights();
 renderTrains();
 renderUI();
}

// ============================================================================
// APPLICATION INITIALIZATION AND MAIN LOOP
// ============================================================================
bool initializeApp()
{
 const unsigned int FRAME_RATE = 60;

 sf::VideoMode videoMode;
 videoMode.width = WINDOW_WIDTH;
 videoMode.height = WINDOW_HEIGHT;

 g_window = new sf::RenderWindow(videoMode, "Train Simulation - PF Final Project");

 if (g_window == nullptr)
 {
 cout << "Error creating SFML window." << endl;
 return false;
 }

 g_window->setFramerateLimit(FRAME_RATE);

 // Load font
 if (!g_font.loadFromFile("/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf"))
 {
 if (!g_font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"))
 {
 cout << "Warning: Could not load font." << endl;
 }
 }


 if (!loadAllSprites())
 {
 cout << "Error"<< endl;
 delete g_window;
 g_window = nullptr;
 return false;
 }

 return true;
}

void runApp()
{
cout<<"running app"<<endl;
 if (!initializeApp())
 {
 cout << "Failed to initialize application." << endl;
 return;
 }
 cout<<"debugging lines"<<endl;
 cout<<"rows :"<<rows<<endl;
 cout<<"cols:"<< cols<<endl;

 sf::Clock clock;
 float lag = 0.0f;

 g_isPaused = true;
 g_isStepMode = false;
 g_isDragging = false;

 while (g_window->isOpen())
 {
 float deltaTime = clock.restart().asSeconds();
 lag = lag + deltaTime;

 sf::Event event;
 while (g_window->pollEvent(event))
 {
 if (event.type == sf::Event::Closed)
 {
 writeMetrics();
 g_window->close();
 }
 else if (event.type == sf::Event::KeyPressed)
 {
 if (event.key.code == sf::Keyboard::Escape)
 {
 writeMetrics();
 g_window->close();
 }
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
 toggleSafetyTile(gridX, gridY);
 }
 else if (event.mouseButton.button == sf::Mouse::Right)
 {
 int gridX, gridY;
 screenToGrid((float)event.mouseButton.x, (float)event.mouseButton.y, &gridX, &gridY);

 if (isInBounds(gridX, gridY))
 {
 char tile = grid[gridX][gridY];
 if (isSwitchTile(tile))
 {
 int switchIndex = getSwitchIndex(tile);
 if (switchIndex >= 0)
 {
 toggleSwitchState(switchIndex);
 }
 }
 }
 }
 else if (event.mouseButton.button == sf::Mouse::Middle)
 {
 g_isDragging = true;
 g_lastMouseX = event.mouseButton.x;
 g_lastMouseY = event.mouseButton.y;
 }
 }
 else if (event.type == sf::Event::MouseButtonReleased)
 {
 if (event.mouseButton.button == sf::Mouse::Middle)
 {
 g_isDragging = false;
 }
 }
 else if (event.type == sf::Event::MouseMoved)
 {
 if (g_isDragging)
 {
 int deltaX = g_lastMouseX - event.mouseMove.x;
 int deltaY = g_lastMouseY - event.mouseMove.y;

 g_gridOffsetX = g_gridOffsetX - deltaX;
 g_gridOffsetY = g_gridOffsetY - deltaY;

 g_lastMouseX = event.mouseMove.x;
 g_lastMouseY = event.mouseMove.y;
 }
 }
 else if (event.type == sf::Event::MouseWheelScrolled)
 {
 if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
 {
 float zoomFactor;
 if (event.mouseWheelScroll.delta > 0)
 zoomFactor = 1.1f;
 else
 zoomFactor = 0.9f;

 g_cellSize = g_cellSize * zoomFactor;
 }
 }
 }

 // Simulation step
 if (g_isStepMode)
 {
 simulateOneTick();
 g_isStepMode = false;
 lag = 0.0f;
 }
 else if (!g_isPaused)
 {
 while (lag >= SIMULATION_TICK_TIME)
 {
 simulateOneTick();
 lag = lag - SIMULATION_TICK_TIME;
 }
 }


 g_window->clear(sf::Color(0,0,0));
 renderSimulationState();
 g_window->display();
 }

 writeMetrics();
}

void cleanupApp()
{
 if (g_window)
 {
 delete g_window;
 g_window = nullptr;
 }
}
