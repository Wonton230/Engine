#include "DebugRenderSystem.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Renderer/Camera.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/VertexUtils.hpp"
#include "Engine/Core/Timer.hpp"
#include "Engine/Core/Clock.hpp"
#include "Engine/Renderer/BitmapFont.hpp"
#include "Engine/Renderer/Texture.hpp"
#include "Game/App.hpp"
#include "Game/Game.hpp"
#include <vector>

extern Clock* g_theSystemClock;
extern EventSystem* g_theEventSystem;
extern App* g_theApp;

struct DebugWorldObject
{
	Texture* m_texture;
	BillBoardType m_billboardType = BillBoardType::NONE;
	Mat44 m_transform;
	Vec3 m_billboardOrigin;
	Vec2 m_billboardScale;
	Rgba8 m_startColor;
	Rgba8 m_endColor;
	Rgba8 m_color;
	DebugRenderMode m_debugRenderMode;
	Timer* m_timer;
	float m_duration;
	std::string m_text;
	bool m_isVisible;
	bool m_isDead;
	bool m_isText = false;
	bool m_isWireFrame = false;
	std::vector<Vertex_PCU> m_vertexes;
};

struct DebugScreenObject
{
	std::vector<Vertex_PCU> m_vertexes;
	Texture* m_texture;
	BillBoardType m_billboardType = BillBoardType::NONE;
	AABB2 m_bounds;
	Rgba8 m_startColor;
	Rgba8 m_endColor;
	Rgba8 m_color;
	DebugRenderMode m_debugRenderMode;
	Timer* m_timer;
	float m_duration;
	std::string m_text;
	bool m_isVisible;
	bool m_isDead;
};

struct DebugMessage
{
	std::vector<Vertex_PCU> m_vertexes;
	BitmapFont* m_font;
	Texture* m_texture;
	std::string m_text;
	float m_duration;
	Rgba8 m_startColor;
	Rgba8 m_endColor;
	Rgba8 m_color;
	Timer* m_timer;
	float m_lineHeight;
};

static Clock* s_debugClock;
static std::vector<DebugWorldObject*> s_debugWorldObjects;
static std::vector<DebugScreenObject*> s_debugScreenObjects;
static std::vector<DebugMessage*> s_debugMessages;
static int s_maxMessagesOnScreen = 45;
static float s_messageLineHeight;
static DebugRenderConfig s_config;
static bool s_visible = true;
static BitmapFont* s_debugRenderFont;
static Camera s_billboardTargetCam;

void DebugRenderSystemStartup(const DebugRenderConfig& config)
{
	s_debugClock = new Clock(*g_theSystemClock);
	s_config = config;

	SubscribeEventCallbackFunction("debug_clear", Command_DebugRenderClear);
	SubscribeEventCallbackFunction("debug_toggle", Command_DebugRenderToggle);

	std::string filepath = s_config.m_fontPath + s_config.m_fontName + ".png";
	s_debugRenderFont = s_config.m_renderer->CreateBitmapFontFromFile(filepath.c_str());
}

void DebugRenderSystemShutdown()
{
	delete s_debugClock;
	s_debugClock = nullptr;

	DebugRenderClear();

	UnsubscribeEventCallbackFunction("debug_clear", Command_DebugRenderClear);
	UnsubscribeEventCallbackFunction("debug_toggle", Command_DebugRenderToggle);
}

void DebugRenderSetVisible()
{
	s_visible = true;
}

void DebugRenderSetHidden()
{
	s_visible = false;
}

void DebugRenderClear()
{
	for (int entIndex = 0; entIndex < static_cast<int>(s_debugWorldObjects.size()); entIndex++)
	{
		if (s_debugWorldObjects[entIndex] != nullptr)
		{
			if (s_debugWorldObjects[entIndex]->m_duration > -1)
			{
				delete 	s_debugWorldObjects[entIndex]->m_timer;
			}
			delete s_debugWorldObjects[entIndex];
			s_debugWorldObjects[entIndex] = nullptr;
		}
	}

	for (int entIndex = 0; entIndex < static_cast<int>(s_debugScreenObjects.size()); entIndex++)
	{
		if (s_debugScreenObjects[entIndex] != nullptr)
		{
			if (s_debugScreenObjects[entIndex]->m_duration > -1)
			{
				delete s_debugScreenObjects[entIndex]->m_timer;
			}
			delete s_debugScreenObjects[entIndex];
			s_debugScreenObjects[entIndex] = nullptr;
		}
	}

	for (int entIndex = 0; entIndex < static_cast<int>(s_debugScreenObjects.size()); entIndex++)
	{
		if (s_debugScreenObjects[entIndex] != nullptr)
		{
			if (s_debugScreenObjects[entIndex]->m_duration > -1)
			{
				delete s_debugScreenObjects[entIndex]->m_timer;
			}
			delete s_debugScreenObjects[entIndex];
			s_debugScreenObjects[entIndex] = nullptr;
		}
	}
}

void DebugRenderBeginFrame()
{
	//Message Management
	for (int entIndex = 0; entIndex < static_cast<int>(s_debugMessages.size());)
	{
		if (s_debugMessages[entIndex] != nullptr)
		{
			if (s_debugMessages[entIndex]->m_duration == 0)
			{
				delete s_debugMessages[entIndex];
				s_debugMessages.erase(s_debugMessages.begin() + entIndex);
			}
			else if (s_debugMessages[entIndex]->m_duration > -1 && s_debugMessages[entIndex]->m_timer->HasPeriodElapsed())
			{
				delete s_debugMessages[entIndex];
				s_debugMessages.erase(s_debugMessages.begin() + entIndex);
			}
			else
			{
				s_debugMessages[entIndex]->m_color = InterpolateColor(s_debugMessages[entIndex]->m_startColor, s_debugMessages[entIndex]->m_endColor, (float)s_debugMessages[entIndex]->m_timer->GetElapsedFraction());
				entIndex++;
			}
		}
		else
		{
			entIndex++;
		}
	}

	for (int entIndex = 0; entIndex < static_cast<int>(s_debugWorldObjects.size()); entIndex++)
	{
		if (s_debugWorldObjects[entIndex] != nullptr)
		{
			if (s_debugWorldObjects[entIndex]->m_duration > -1)
			{
				if (s_debugWorldObjects[entIndex]->m_timer->HasPeriodElapsed())
				{
					delete s_debugWorldObjects[entIndex];
					s_debugWorldObjects[entIndex] = nullptr;
				}
				else
				{
					s_debugWorldObjects[entIndex]->m_color = InterpolateColor(s_debugWorldObjects[entIndex]->m_startColor, s_debugWorldObjects[entIndex]->m_endColor, (float)s_debugWorldObjects[entIndex]->m_timer->GetElapsedFraction());
				}
			}
		}
	}

	for (int entIndex = 0; entIndex < static_cast<int>(s_debugScreenObjects.size()); entIndex++)
	{
		if (s_debugScreenObjects[entIndex] != nullptr)
		{
			if (s_debugScreenObjects[entIndex]->m_duration > -1 && s_debugScreenObjects[entIndex]->m_timer->HasPeriodElapsed())
			{
				delete s_debugScreenObjects[entIndex];
				s_debugScreenObjects[entIndex] = nullptr;
			}
		}
	}
}

void DebugRenderWorld(const Camera& camera)
{
	s_config.m_renderer->BeginCamera(camera);
	s_billboardTargetCam = camera;

	if (s_visible)
	{
		for (int entIndex = 0; entIndex < static_cast<int>(s_debugWorldObjects.size()); entIndex++)
		{
			if (s_debugWorldObjects[entIndex] != nullptr)
			{
				s_config.m_renderer->SetModelConstants(s_debugWorldObjects[entIndex]->m_transform, s_debugWorldObjects[entIndex]->m_color);
				s_config.m_renderer->BindTexture(s_debugWorldObjects[entIndex]->m_texture);
				switch (s_debugWorldObjects[entIndex]->m_debugRenderMode)
				{
				case DebugRenderMode::ALWAYS:
					s_config.m_renderer->SetBlendMode(BlendMode::ALPHA);
					s_config.m_renderer->SetRasterizerMode(RasterizerMode::SOLID_CULL_BACK);
					s_config.m_renderer->SetDepthMode(DepthMode::DISABLED);
					break;
				case DebugRenderMode::USE_DEPTH:
					s_config.m_renderer->SetBlendMode(BlendMode::ALPHA);
					s_config.m_renderer->SetRasterizerMode(RasterizerMode::SOLID_CULL_BACK);
					s_config.m_renderer->SetDepthMode(DepthMode::READ_WRITE_LESS_EQUAL);
					break;
				case DebugRenderMode::X_RAY:
					s_config.m_renderer->SetBlendMode(BlendMode::ALPHA);
					s_config.m_renderer->SetRasterizerMode(RasterizerMode::SOLID_CULL_BACK);
					s_config.m_renderer->SetDepthMode(DepthMode::READ_ONLY_ALWAYS);
					for (int i = 0; i < s_debugWorldObjects[entIndex]->m_vertexes.size(); i++)
					{
						unsigned char a = DenormalizeByte(NormalizeByte(s_debugWorldObjects[entIndex]->m_vertexes[i].m_color.a) * 0.5f);
						s_debugWorldObjects[entIndex]->m_vertexes[i].m_color = Rgba8(s_debugWorldObjects[entIndex]->m_vertexes[i].m_color.r, 
							s_debugWorldObjects[entIndex]->m_vertexes[i].m_color.g, 
							s_debugWorldObjects[entIndex]->m_vertexes[i].m_color.b,
							a);
					}
					break;
				}
				if (s_debugWorldObjects[entIndex]->m_isText)
				{
					s_config.m_renderer->SetRasterizerMode(RasterizerMode::SOLID_CULL_NONE);
					if (s_debugWorldObjects[entIndex]->m_billboardType != BillBoardType::NONE)
					{
						s_debugWorldObjects[entIndex]->m_transform = GetBillboardTransform(BillBoardType::FULL_OPPOSING, s_billboardTargetCam.GetModelToWorldTransform(), s_debugWorldObjects[entIndex]->m_billboardOrigin, s_debugWorldObjects[entIndex]->m_billboardScale);
					}
				}
				else if (s_debugWorldObjects[entIndex]->m_isWireFrame)
				{
					s_config.m_renderer->SetRasterizerMode(RasterizerMode::WIREFRAME_CULL_BACK);
				}
				s_config.m_renderer->BindShader(nullptr);
				s_config.m_renderer->SetStatesIfChanged();
				s_config.m_renderer->DrawVertexArray(s_debugWorldObjects[entIndex]->m_vertexes);

				if (s_debugWorldObjects[entIndex]->m_debugRenderMode == DebugRenderMode::X_RAY)
				{
					s_config.m_renderer->SetBlendMode(BlendMode::OPAQUE);
					s_config.m_renderer->SetDepthMode(DepthMode::READ_WRITE_LESS_EQUAL);
					s_config.m_renderer->SetStatesIfChanged();
					for (int i = 0; i < s_debugWorldObjects[entIndex]->m_vertexes.size(); i++)
					{
						float aF = NormalizeByte(s_debugWorldObjects[entIndex]->m_vertexes[i].m_color.a) * 2.f;
						unsigned char a = DenormalizeByte(aF);
						s_debugWorldObjects[entIndex]->m_vertexes[i].m_color = Rgba8(s_debugWorldObjects[entIndex]->m_vertexes[i].m_color.r,
							s_debugWorldObjects[entIndex]->m_vertexes[i].m_color.g,
							s_debugWorldObjects[entIndex]->m_vertexes[i].m_color.b,
							a);
					}
					s_config.m_renderer->DrawVertexArray(s_debugWorldObjects[entIndex]->m_vertexes);
				}
			}
		}
	}

	s_config.m_renderer->EndCamera(camera);
}

void DebugRenderScreen(const Camera& camera)
{
	s_messageLineHeight = camera.GetOrthographicTopRight().y / s_maxMessagesOnScreen;
	s_config.m_renderer->BeginCamera(camera);

	if (s_visible)
	{
		Vec3 topLeft = Vec3(camera.GetOrthographicBottomLeft().x, camera.GetOrthographicTopRight().y, 0);

		for (int i = 0; i < static_cast<int>(s_debugMessages.size()); i++)
		{
			DebugMessage* message = s_debugMessages[i];
			if (message != nullptr)
			{
				float yOffset = -(i * s_messageLineHeight);
				message->m_vertexes.clear();
				message->m_font->AddVertsForTextInBox2D(
					message->m_vertexes,
					message->m_text,
					AABB2(Vec2(topLeft.x, topLeft.y + yOffset - s_messageLineHeight),
						Vec2(800, topLeft.y + yOffset)),
					s_messageLineHeight,
					message->m_color,
					0.75f,
					Vec2(0, 1)
				);
				s_config.m_renderer->SetRasterizerMode(RasterizerMode::SOLID_CULL_NONE);
				s_config.m_renderer->BindTexture(message->m_texture);
				s_config.m_renderer->BindShader(nullptr);
				s_config.m_renderer->SetStatesIfChanged();
				s_config.m_renderer->DrawVertexArray(message->m_vertexes);
			}
		}

		for (int entIndex = 0; entIndex < static_cast<int>(s_debugScreenObjects.size()); entIndex++)
		{
			if (s_debugScreenObjects[entIndex] != nullptr)
			{
				s_config.m_renderer->SetRasterizerMode(RasterizerMode::SOLID_CULL_NONE);
				s_config.m_renderer->BindTexture(s_debugScreenObjects[entIndex]->m_texture);
				s_config.m_renderer->BindShader(nullptr);
				s_config.m_renderer->SetStatesIfChanged();
 				s_config.m_renderer->DrawVertexArray(s_debugScreenObjects[entIndex]->m_vertexes);
			}
		}
	}

	s_config.m_renderer->EndCamera(camera);
}

void DebugRenderEndFrame()
{
}

void DebugAddWorldSphere(const Vec3& center, float radius, float duration, const Rgba8& startColor, const Rgba8& endColor, DebugRenderMode mode)
{
	DebugWorldObject* sphere = new DebugWorldObject;
	AddVertsForSphere(sphere->m_vertexes, Vec3(), radius, startColor);
	if (duration >= 0)
	{
		sphere->m_timer = new Timer(duration, s_debugClock);
		sphere->m_timer->Start();
	}

	sphere->m_duration = duration;
	Mat44 translation = Mat44::MakeTranslation3D(center);
	sphere->m_transform = translation;
	sphere->m_texture = nullptr;
	sphere->m_startColor = startColor;
	sphere->m_endColor = endColor;
	sphere->m_debugRenderMode = mode;
	s_debugWorldObjects.push_back(sphere);
}

void DebugAddWorldWireSphere(const Vec3& center, float radius, float duration, const Rgba8& startColor, const Rgba8& endColor, DebugRenderMode mode)
{
	DebugWorldObject* sphere = new DebugWorldObject;
	AddVertsForSphere(sphere->m_vertexes, Vec3(), radius, startColor);
	if (duration >= 0)
	{
		sphere->m_timer = new Timer(duration, s_debugClock);
		sphere->m_timer->Start();
	}

	sphere->m_duration = duration;
	Mat44 translation = Mat44::MakeTranslation3D(center);
	sphere->m_transform = translation;
	sphere->m_texture = nullptr;
	sphere->m_startColor = startColor;
	sphere->m_endColor = endColor;
	sphere->m_debugRenderMode = mode;
	sphere->m_isWireFrame = true;
	s_debugWorldObjects.push_back(sphere);
}

void DebugAddWorldCylinder(const Vec3& start, const Vec3& end, float radius, float duration, const Rgba8& startColor, const Rgba8& endColor, DebugRenderMode mode)
{
	DebugWorldObject* cylinder = new DebugWorldObject;
	AddVertsForCylinder3D(cylinder->m_vertexes, start, end, radius, startColor);
	if (duration >= 0)
	{
		cylinder->m_timer = new Timer(duration, s_debugClock);
		cylinder->m_timer->Start();
	}

	cylinder->m_duration = duration;
	Mat44 translation = Mat44();
	cylinder->m_transform = translation;
	cylinder->m_texture = nullptr;
	cylinder->m_startColor = startColor;
	cylinder->m_endColor = endColor;
	cylinder->m_debugRenderMode = mode;
	s_debugWorldObjects.push_back(cylinder);
}

void DebugAddWorldWireCylinder(const Vec3& start, const Vec3& end, float radius, float duration, const Rgba8& startColor, const Rgba8& endColor, DebugRenderMode mode)
{
	DebugWorldObject* cylinder = new DebugWorldObject;
	AddVertsForCylinder3D(cylinder->m_vertexes, start, end, radius, startColor);
	if (duration >= 0)
	{
		cylinder->m_timer = new Timer(duration, s_debugClock);
		cylinder->m_timer->Start();
	}

	cylinder->m_duration = duration;
	Mat44 translation = Mat44();
	cylinder->m_transform = translation;
	cylinder->m_texture = nullptr;
	cylinder->m_startColor = startColor;
	cylinder->m_endColor = endColor;
	cylinder->m_debugRenderMode = mode;
	cylinder->m_isWireFrame = true;
	s_debugWorldObjects.push_back(cylinder);
}

void DebugAddWorldArrow(const Vec3& start, const Vec3& end, float radius, float duration, const Rgba8& startColor, const Rgba8& endColor, DebugRenderMode mode)
{
	DebugWorldObject* arrow = new DebugWorldObject;
	AddVertsForRoundArrow3D(arrow->m_vertexes, start, end, radius, startColor);
	if (duration >= 0)
	{
		arrow->m_timer = new Timer(duration, s_debugClock);
		arrow->m_timer->Start();
	}

	arrow->m_duration = duration;
	Mat44 translation = Mat44();
	arrow->m_transform = translation;
	arrow->m_texture = nullptr;
	arrow->m_startColor = startColor;
	arrow->m_endColor = endColor;
	arrow->m_debugRenderMode = mode;
	s_debugWorldObjects.push_back(arrow);
}

void DebugAddWorldWireArrow(const Vec3& start, const Vec3& end, float radius, float duration, const Rgba8& startColor, const Rgba8& endColor, DebugRenderMode mode)
{
	DebugWorldObject* arrow = new DebugWorldObject;
	AddVertsForRoundArrow3D(arrow->m_vertexes, start, end, radius, startColor);
	if (duration >= 0)
	{
		arrow->m_timer = new Timer(duration, s_debugClock);
		arrow->m_timer->Start();
	}

	arrow->m_duration = duration;
	Mat44 translation = Mat44();
	arrow->m_transform = translation;
	arrow->m_texture = nullptr;
	arrow->m_startColor = startColor;
	arrow->m_endColor = endColor;
	arrow->m_debugRenderMode = mode;
	arrow->m_isWireFrame = true;
	s_debugWorldObjects.push_back(arrow);
}

void DebugAddBasis(const Mat44& transform, float duration, float length, float radius, float colorScale, float alphaScale, DebugRenderMode mode)
{
	DebugWorldObject* basis = new DebugWorldObject;
	AddVertsForRoundArrow3D(basis->m_vertexes, Vec3(), Vec3(length,0,0), radius, Rgba8::RED);
	AddVertsForRoundArrow3D(basis->m_vertexes, Vec3(), Vec3(0, length, 0), radius, Rgba8::GREEN);
	AddVertsForRoundArrow3D(basis->m_vertexes, Vec3(), Vec3(0, 0, length), radius, Rgba8::BLUE);
	if (duration >= 0)
	{
		basis->m_timer = new Timer(duration, s_debugClock);
		basis->m_timer->Start();
	}

	basis->m_duration = duration;
	basis->m_transform = transform;
	basis->m_texture = nullptr;
	basis->m_debugRenderMode = mode;
	s_debugWorldObjects.push_back(basis);
	alphaScale;
	colorScale;
}

void DebugAddWorldBasis(const Mat44& transform, float duration, DebugRenderMode mode)
{
	DebugWorldObject* basis = new DebugWorldObject;
	AddVertsForRoundArrow3D(basis->m_vertexes, Vec3(), Vec3(2, 0, 0), .15f, Rgba8::RED);
	AddVertsForRoundArrow3D(basis->m_vertexes, Vec3(), Vec3(0, 2, 0), .15f, Rgba8::GREEN);
	AddVertsForRoundArrow3D(basis->m_vertexes, Vec3(), Vec3(0, 0, 2), .15f, Rgba8::BLUE);
	if (duration >= 0)
	{
		basis->m_timer = new Timer(duration, s_debugClock);
		basis->m_timer->Start();
	}

	basis->m_duration = duration;
	basis->m_transform = transform;
	basis->m_texture = nullptr;
	basis->m_debugRenderMode = mode;
	s_debugWorldObjects.push_back(basis);
}

void DebugAddWorldText(const std::string& text, const Mat44& transform, float textheight, const Vec2& alignment, float duration, const Rgba8& startColor, const Rgba8& endColor, DebugRenderMode mode)
{
	DebugWorldObject* textObject = new DebugWorldObject;
	BitmapFont* font = s_debugRenderFont;
	font->AddVertsForText3DAtOriginXForward(textObject->m_vertexes, textheight, text, startColor, 1.f, alignment);

	if (duration >= 0)
	{
		textObject->m_timer = new Timer(duration, s_debugClock);
		textObject->m_timer->Start();
	}

	textObject->m_duration = duration;
	textObject->m_transform = transform;
	textObject->m_texture = &font->GetTexture();
	textObject->m_startColor = startColor;
	textObject->m_endColor = endColor;
	textObject->m_debugRenderMode = mode;
	textObject->m_isText = true;
	s_debugWorldObjects.push_back(textObject);
}

void DebugAddWorldBillboardText(const std::string& text, const Vec3& origin, float textheight, const Vec2& alignment, float duration, const Rgba8& startColor, const Rgba8& endColor, DebugRenderMode mode)
{
	DebugWorldObject* textObject = new DebugWorldObject;
	BitmapFont* font = s_debugRenderFont;
	font->AddVertsForText3DAtOriginXForward(textObject->m_vertexes, textheight, text, startColor, 1.f, alignment);

	if (duration >= 0)
	{
		textObject->m_timer = new Timer(duration, s_debugClock);
		textObject->m_timer->Start();
	}

	textObject->m_duration = duration;
	textObject->m_billboardOrigin = origin;
	float textLength = text.length() * textheight;

	textObject->m_transform = GetBillboardTransform(BillBoardType::FULL_OPPOSING, s_billboardTargetCam.GetModelToWorldTransform(), origin, Vec2(textLength, textheight));
	textObject->m_billboardScale = Vec2(textLength, textheight);
	textObject->m_billboardType = BillBoardType::FULL_OPPOSING;
	textObject->m_texture = &font->GetTexture();
	textObject->m_startColor = startColor;
	textObject->m_endColor = endColor;
	textObject->m_debugRenderMode = mode;
	textObject->m_isText = true;
	s_debugWorldObjects.push_back(textObject);
}

void DebugAddScreenText(const std::string& text, const AABB2& box, float cellHeight, const Vec2& alignment, float duration, const Rgba8& startColor, const Rgba8& endColor)
{
	DebugScreenObject* screenText = new DebugScreenObject;
	BitmapFont* font = s_debugRenderFont;
	font->AddVertsForTextInBox2D(screenText->m_vertexes, text, box, cellHeight, startColor, .75f, alignment);

	if (duration >= 0)
	{
		screenText->m_timer = new Timer(duration, s_debugClock);
		screenText->m_timer->Start();
	}
	screenText->m_duration = duration;
	screenText->m_texture = &(font->GetTexture());
	screenText->m_startColor = startColor;
	screenText->m_endColor = endColor;
	s_debugScreenObjects.push_back(screenText);
}

void DebugAddMessage(const std::string& text, float duration, const Rgba8& startColor, const Rgba8& endColor)
{
	DebugMessage* message = new DebugMessage();
	BitmapFont* font = s_debugRenderFont;
	message->m_font = font;
	message->m_text = text;

	if (duration >= 0)
	{
		message->m_timer = new Timer(duration, s_debugClock);
		message->m_timer->Start();
	}
	message->m_duration = duration;
	message->m_texture = &(font->GetTexture());
	message->m_startColor = startColor;
	message->m_endColor = endColor;
	s_debugMessages.insert(s_debugMessages.begin(), message);
}

bool Command_DebugRenderClear(EventArgs& args)
{
	args;
	DebugRenderClear();
	return true;
}

bool Command_DebugRenderToggle(EventArgs& args)
{
	args;
	if (s_visible)
	{
		DebugRenderSetHidden();
	}
	else
	{
		DebugRenderSetVisible();
	}
	return false;
}
