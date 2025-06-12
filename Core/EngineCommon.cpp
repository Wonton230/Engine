#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/EventSystem.hpp"
#include "Engine/Core/DevConsole.hpp"
#include "Engine/Core/Clock.hpp"

float SCREEN_SIZE_X = 1600.f;
float SCREEN_SIZE_Y = 800.f;
float SCREEN_CENTER_X;
float SCREEN_CENTER_Y;

float WORLD_TO_SCREEN_SCALE;
NamedStrings* g_gameConfigBlackboard = new NamedStrings();

EventSystemConfig config;
EventSystem* g_theEventSystem = new EventSystem(config);

DevConsole* g_theDevConsole = nullptr;
InputSystem* g_theInputSystem = nullptr;

Clock* g_theSystemClock = new Clock();