#include "Engine/Core/NamedStrings.hpp"
#pragma once

#define UNUSED(x) (void)(x);

constexpr float G_SCREEN_SIZE_X = 1600.f;
constexpr float G_SCREEN_SIZE_Y = 800.f;

constexpr float WINDOWS_BAR_HEIGHT = 64.f;

extern float SCREEN_SIZE_X;
extern float SCREEN_SIZE_Y;
extern float SCREEN_CENTER_X;
extern float SCREEN_CENTER_Y;

extern float WORLD_TO_SCREEN_SCALE;

class NamedStrings;
class DevConsole;
class EventSystem;
class InputSystem;
class Clock;

extern NamedStrings* g_gameConfigBlackboard;
extern DevConsole* g_theDevConsole;
extern EventSystem* g_theEventSystem;
extern InputSystem* g_theInputSystem;
extern Clock* g_theSystemClock;