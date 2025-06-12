#pragma once
#include <string>
#include "Engine/Core/Rgba8.hpp"
#include "Engine/Core/EventSystem.hpp"

class Renderer;
class Camera;
struct Vec2;
struct Vec3;
struct Mat44;
struct AABB2;

enum class DebugRenderMode
{
	ALWAYS,
	USE_DEPTH,
	X_RAY,
};

struct DebugRenderConfig
{
	Renderer* m_renderer = nullptr;
	std::string m_fontPath = "Data/Fonts/";
	std::string m_fontName = "SquirrelFixedFont";
};

//Setup
void DebugRenderSystemStartup(const DebugRenderConfig& config);
void DebugRenderSystemShutdown();

//Control
void DebugRenderSetVisible();
void DebugRenderSetHidden();
void DebugRenderClear();

//Output
void DebugRenderBeginFrame();
void DebugRenderWorld(const Camera& camera);
void DebugRenderScreen(const Camera& camera);
void DebugRenderEndFrame();

//Geometry
void DebugAddWorldSphere(const Vec3& center, float radius, float duration,
	const Rgba8& startColor = Rgba8::WHITE, const Rgba8& endColor = Rgba8::WHITE, 
	DebugRenderMode mode = DebugRenderMode::USE_DEPTH);
void DebugAddWorldWireSphere(const Vec3& center, float radius, float duration,
	const Rgba8& startColor = Rgba8::WHITE, const Rgba8& endColor = Rgba8::WHITE, 
	DebugRenderMode mode = DebugRenderMode::USE_DEPTH);
void DebugAddWorldCylinder(const Vec3& start, const Vec3& end, float radius, float duration,
	const Rgba8& startColor = Rgba8::WHITE, const Rgba8& endColor = Rgba8::WHITE, 
	DebugRenderMode mode = DebugRenderMode::USE_DEPTH);
void DebugAddWorldWireCylinder(const Vec3& start, const Vec3& end, float radius, float duration,
	const Rgba8& startColor = Rgba8::WHITE, const Rgba8& endColor = Rgba8::WHITE, 
	DebugRenderMode mode = DebugRenderMode::USE_DEPTH);
void DebugAddWorldArrow(const Vec3& start, const Vec3& end, float radius, float duration,
	const Rgba8& startColor = Rgba8::WHITE, const Rgba8& endColor = Rgba8::WHITE, 
	DebugRenderMode mode = DebugRenderMode::USE_DEPTH);
void DebugAddWorldWireArrow(const Vec3& start, const Vec3& end, float radius, float duration,
	const Rgba8& startColor = Rgba8::WHITE, const Rgba8& endColor = Rgba8::WHITE, 
	DebugRenderMode mode = DebugRenderMode::USE_DEPTH);
void DebugAddBasis(const Mat44& transform, float duration, float length, float radius,
	float colorScale = 1.0f, float alphaScale = 1.0f, 
	DebugRenderMode mode = DebugRenderMode::USE_DEPTH);
void DebugAddWorldBasis(const Mat44& transform, float duration, 
	DebugRenderMode mode = DebugRenderMode::USE_DEPTH);
void DebugAddWorldText(const std::string& text, const Mat44& transform, float textheight,
	const Vec2& alignment, float duration, 
	const Rgba8& startColor = Rgba8::WHITE, const Rgba8& endColor = Rgba8::WHITE, 
	DebugRenderMode mode = DebugRenderMode::USE_DEPTH);
void DebugAddWorldBillboardText(const std::string& text, const Vec3& origin, float textheight,
	const Vec2& alignment, float duration,
	const Rgba8& startColor = Rgba8::WHITE, const Rgba8& endColor = Rgba8::WHITE,
	DebugRenderMode mode = DebugRenderMode::USE_DEPTH);

void DebugAddScreenText(const std::string& text, const AABB2& box, float cellHeight,
	const Vec2& alignment, float duration,
	const Rgba8& startColor = Rgba8::WHITE, const Rgba8& endColor = Rgba8::WHITE);
void DebugAddMessage(const std::string& text, float duration, 
	const Rgba8& startColor = Rgba8::WHITE, const Rgba8& endColor = Rgba8::WHITE);

//Console Commands
bool Command_DebugRenderClear(EventArgs& args);
bool Command_DebugRenderToggle(EventArgs& args);