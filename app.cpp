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

const unsigned int WINDOW_WIDTH = 1200;
const unsigned int WINDOW_HEIGHT = 800;
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
// SPRITE TEXTURES
// ============================================================================
sf::Texture trainUpTexture;
sf::Texture trainDownTexture;
sf::Texture trainLeftTexture;
sf::Texture trainRightTexture;

sf::Texture signalGreenTexture;
sf::Texture signalYellowTexture;
sf::Texture signalRedTexture;

sf::Texture spawnBlueTexture;
sf::Texture spawnRedTexture;
sf::Texture destinationRedTexture;
sf::Texture destinationBlueTexture;
sf::Texture safetyTexture;

sf::Texture sourceTilesTexture;
sf::Texture destinationTilesTexture;

sf::Texture switchState1Texture;
sf::Texture switchState2Texture;

sf::Texture trackHorizontalTexture;
sf::Texture trackVerticalTexture;
sf::Texture trackDiagLeftTexture;
sf::Texture trackDiagRightTexture;
sf::Texture trackCrossTexture;

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
// SPRITE LOADING
// ============================================================================
bool loadAllSprites()
{
    // Load train sprites
    if (!trainUpTexture.loadFromFile("sprites/train_up.png"))
    {
        cout << "Failed to load train_up.png" << endl;
        return false;
    }
    if (!trainDownTexture.loadFromFile("sprites/train_down.png"))
    {
        cout << "Failed to load train_down.png" << endl;
        return false;
    }
    if (!trainLeftTexture.loadFromFile("sprites/train_left.png"))
    {
        cout << "Failed to load train_left.png" << endl;
        return false;
    }
    if (!trainRightTexture.loadFromFile("sprites/train_right.png"))
    {
        cout << "Failed to load train_right.png" << endl;
        return false;
    }

    // Load signal light sprites
    if (!signalGreenTexture.loadFromFile("sprites/signal_green.png"))
    {
        cout << "Failed to load signal_green.png" << endl;
        return false;
    }
    if (!signalYellowTexture.loadFromFile("sprites/signal_yellow.png"))
    {
        cout << "Failed to load signal_yellow.png" << endl;
        return false;
    }
    if (!signalRedTexture.loadFromFile("sprites/signal_red.png"))
    {
        cout << "Failed to load signal_red.png" << endl;
        return false;
    }

    // Load spawn and destination sprites
    if (!spawnBlueTexture.loadFromFile("sprites/spawn_blue.png"))
    {
        cout << "Failed to load spawn_blue.png" << endl;
        return false;
    }
    if (!destinationRedTexture.loadFromFile("sprites/destination_red.png"))
    {
        cout << "Failed to load destination_red.png" << endl;
        return false;
    }
    if (!safetyTexture.loadFromFile("sprites/safety.png"))
    {
        cout << "Failed to load safety.png" << endl;
        return false;
    }

    // Load tile sprites
    if (!sourceTilesTexture.loadFromFile("sprites/source_tiles.png"))
    {
        cout << "Failed to load source_tiles.png" << endl;
        return false;
    }
    if (!destinationTilesTexture.loadFromFile("sprites/destination_tiles.png"))
    {
        cout << "Failed to load destination_tiles.png" << endl;
        return false;
    }

    // Load switch sprites
    if (!switchState1Texture.loadFromFile("sprites/switch_state1.png"))
    {
        cout << "Failed to load switch_state1.png" << endl;
        return false;
    }
    if (!switchState2Texture.loadFromFile("sprites/switch_state2.png"))
    {
        cout << "Failed to load switch_state2.png" << endl;
        return false;
    }

    // Load track sprites
    if (!trackHorizontalTexture.loadFromFile("sprites/track_horizontal.png"))
    {
        cout << "Failed to load track_horizontal.png" << endl;
        return false;
    }
    if (!trackVerticalTexture.loadFromFile("sprites/track_vertical.png"))
    {
        cout << "Failed to load track_vertical.png" << endl;
        return false;
    }
    if (!trackDiagLeftTexture.loadFromFile("sprites/track_diag_left.png"))
    {
        cout << "Failed to load track_diag_left.png" << endl;
        return false;
    }
    if (!trackDiagRightTexture.loadFromFile("sprites/track_diag_right.png"))
    {
        cout << "Failed to load track_diag_right.png" << endl;
        return false;
    }
    if (!trackCrossTexture.loadFromFile("sprites/track_cross.png"))
    {
        cout << "Failed to load track_cross.png" << endl;
        return false;
    }

    return true;
}

// ============================================================================
// RENDERING FUNCTIONS
// ============================================================================
void drawSprite(sf::Texture* texture, float x, float y, float scale)
{
    if (texture == nullptr)
        return;

    sf::Sprite sprite;
    sprite.setTexture(*texture);
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
        drawLine(x1, y1, x2, y2, 60, 60, 60);
        i++;
    }

    int j = 0;
    while (j <= cols)
    {
        float x1, y1, x2, y2;
        gridToScreen(0, j, &x1, &y1);
        gridToScreen(rows, j, &x2, &y2);
        drawLine(x1, y1, x2, y2, 60, 60, 60);
        j++;
    }

    // Draw tiles
    i = 0;
    while (i < rows)
    {
        j = 0;
        while (j < cols)
        {
            char tile = grid[i][j];
            float posX, posY;
            gridToScreen(i, j, &posX, &posY);

            // Calculate scale to fit sprite in cell
            float scale = g_cellSize / 64.0f; // Assuming sprites are 64x64

            // Skip empty tiles
            if (tile == ' ' || tile == '.')
            {
                j++;
                continue;
            }

            if (tile == '=')
            {
                // Safety tile
                drawSprite(&safetyTexture, posX, posY, scale);
            }
            else if (tile == 'S')
            {
                // Spawn point - use blue spawn
                drawSprite(&spawnBlueTexture, posX, posY, scale);
            }
            else if (tile == 'D')
            {
                // Destination point - use red destination
                drawSprite(&destinationRedTexture, posX, posY, scale);
            }
            else if (tile >= 'A' && tile <= 'Z')
            {
                // Switch tile
                int switchIndex = getSwitchIndex(tile);
                if (switchIndex >= 0 && switchIndex < NumSwitches)
                {
                    if (switch_currentState[switchIndex] == 0)
                        drawSprite(&switchState1Texture, posX, posY, scale);
                    else
                        drawSprite(&switchState2Texture, posX, posY, scale);

                    // Draw switch letter
                    char switchText[2];
                    switchText[0] = tile;
                    switchText[1] = '\0';
                    drawText(switchText, posX + g_cellSize * 0.35f, posY + g_cellSize * 0.25f,
                             (int)(g_cellSize * 0.4f), 255, 255, 255);
                }
            }
            else if (tile == '|')
            {
                // Vertical track
                drawSprite(&trackVerticalTexture, posX, posY, scale);
            }
            else if (tile == '-')
            {
                // Horizontal track
                drawSprite(&trackHorizontalTexture, posX, posY, scale);
            }
            else if (tile == '/')
            {
                // Diagonal left track
                drawSprite(&trackDiagLeftTexture, posX, posY, scale);
            }
            else if (tile == '\\')
            {
                // Diagonal right track
                drawSprite(&trackDiagRightTexture, posX, posY, scale);
            }
            else if (tile == '+')
            {
                // Cross track
                drawSprite(&trackCrossTexture, posX, posY, scale);
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

            // Calculate scale to fit sprite in cell
            float scale = g_cellSize / 64.0f; // Assuming sprites are 64x64

            // Draw train sprite based on direction
            if (train_direction[i] == DIR_UP)
            {
                drawSprite(&trainUpTexture, posX, posY, scale);
            }
            else if (train_direction[i] == DIR_DOWN)
            {
                drawSprite(&trainDownTexture, posX, posY, scale);
            }
            else if (train_direction[i] == DIR_LEFT)
            {
                drawSprite(&trainLeftTexture, posX, posY, scale);
            }
            else if (train_direction[i] == DIR_RIGHT)
            {
                drawSprite(&trainRightTexture, posX, posY, scale);
            }

            // Draw train ID on top
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

                // Calculate scale for signal light (smaller than cell)
                float scale = g_cellSize / 128.0f; // Half the size of cell

                // Draw signal light in top-left corner
                float signalX = posX + g_cellSize * 0.05f;
                float signalY = posY + g_cellSize * 0.05f;

                if (switch_signalcolour[i] == GREEN)
                {
                    drawSprite(&signalGreenTexture, signalX, signalY, scale);
                }
                else if (switch_signalcolour[i] == YELLOW)
                {
                    drawSprite(&signalYellowTexture, signalX, signalY, scale);
                }
                else // RED
                {
                    drawSprite(&signalRedTexture, signalX, signalY, scale);
                }
            }
        }

        i++;
    }
}

void renderUI()
{
    // Create semi-transparent background for UI
    drawRectangle(0, 0, WINDOW_WIDTH, 80, 0, 0, 0, 180);

    // Render tick counter
    char tickText[50];
    sprintf(tickText, "Tick: %d", currentTick);
    drawText(tickText, 10, 10, 20, 255, 255, 255);

    // Render metrics
    int active = NumTrains - metric_delivered - metric_crashed;
    char metricsText[200];
    sprintf(metricsText, "Trains: %d | Active: %d | Delivered: %d | Crashed: %d",
            NumTrains, active, metric_delivered, metric_crashed);
    drawText(metricsText, 10, 35, 18, 0, 255, 255);

    // Render controls hint
    const char* controlsText = "Controls: SPACE=Pause | PERIOD=Step | LEFT CLICK=Safety | RIGHT CLICK=Switch | SCROLL=Zoom | MIDDLE DRAG=Pan | ESC=Exit";
    drawText(controlsText, 10, 60, 14, 200, 200, 200);

    // Pause indicator
    if (g_isPaused)
    {
        drawRectangle(WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 - 30, 300, 60, 255, 0, 0, 200);
        drawText("PAUSED", WINDOW_WIDTH / 2 - 60, WINDOW_HEIGHT / 2 - 20, 40, 255, 255, 255);
    }

    // Simulation complete indicator
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
    if (!g_font.loadFromFile("arial.ttf"))
    {
        cout << "Error loading font file. Trying fallback..." << endl;
        if (!g_font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"))
        {
            cout << "Error loading fallback font." << endl;
            delete g_window;
            g_window = nullptr;
            return false;
        }
    }

    // Load all sprite textures
    if (!loadAllSprites())
    {
        cout << "Error loading sprite textures." << endl;
        delete g_window;
        g_window = nullptr;
        return false;
    }

    return true;
}

void runApp()
{
    if (!initializeApp())
    {
        cout << "Failed to initialize application." << endl;
        return;
    }

    sf::Clock clock;
    float lag = 0.0f;

    g_isPaused = true;
    g_isStepMode = false;
    g_isDragging = false;

    while (g_window->isOpen())
    {
        float deltaTime = clock.restart().asSeconds();
        lag = lag + deltaTime;

        // Event handling
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
                    // Toggle safety tile
                    int gridX, gridY;
                    screenToGrid((float)event.mouseButton.x, (float)event.mouseButton.y, &gridX, &gridY);
                    toggleSafetyTile(gridX, gridY);
                }
                else if (event.mouseButton.button == sf::Mouse::Right)
                {
                    // Toggle switch
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

        // Rendering
        g_window->clear(sf::Color(30, 30, 30, 255));
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
