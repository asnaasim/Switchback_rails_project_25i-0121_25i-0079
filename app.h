#ifndef APP_H
#define APP_H

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
// CONSTANTS (extern declarations)
// ============================================================================
extern const unsigned int WINDOW_WIDTH;
extern const unsigned int WINDOW_HEIGHT;
extern const float SIMULATION_TICK_TIME;

// ============================================================================
// GLOBAL STATE VARIABLES (extern declarations)
// ============================================================================
extern sf::Texture trainUpTexture;
extern sf::Texture trainDownTexture;
extern sf::Texture trainLeftTexture;
extern sf::Texture trainRightTexture;
extern sf::Texture signalGreenTexture;
extern sf::Texture signalYellowTexture;
extern sf::Texture signalRedTexture;
extern sf::Texture spawnBlueTexture;
extern sf::Texture spawnRedTexture;
extern sf::Texture destinationRedTexture;
extern sf::Texture destinationBlueTexture;
extern sf::Texture safetyTexture;
extern sf::Texture sourceTilesTexture;
extern sf::Texture destinationTilesTexture;
extern sf::Texture switchState1Texture;
extern sf::Texture switchState2Texture;
extern sf::Texture trackHorizontalTexture;
extern sf::Texture trackVerticalTexture;
extern sf::Texture trackDiagLeftTexture;
extern sf::Texture trackDiagRightTexture;
extern sf::Texture trackCrossTexture;

extern bool g_isPaused;
extern bool g_isStepMode;
extern bool g_isDragging;
extern int g_lastMouseX;
extern int g_lastMouseY;
extern float g_cellSize;
extern float g_gridOffsetX;
extern float g_gridOffsetY;

// ============================================================================
// FUNCTION DECLARATIONS
// ============================================================================
void gridToScreen(int gridX, int gridY, float* screenX, float* screenY);
void screenToGrid(float screenX, float screenY, int* gridX, int* gridY);
bool loadAllSprites();
void drawSprite(sf::Texture* texture, float x, float y, float scale);
void drawLine(float x1, float y1, float x2, float y2, int r, int g, int b);
void drawRectangle(float x, float y, float width, float height, int r, int g, int b, int a);
void drawText(const char* text, float x, float y, int size, int r, int g, int b);
void renderTrains();
void renderSignalLights();
void renderUI();
void renderGrid();
void renderSimulationState();
bool initializeApp();
void runApp();
void cleanupApp();

extern void cleanupApp();
extern void runApp();
extern bool initializeApp();
extern void renderSimulationState();

#endif
