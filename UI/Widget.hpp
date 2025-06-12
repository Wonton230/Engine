#pragma once
#include <vector>
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/Vertex_PCUTBN.hpp"
#include "Engine/Core/Vertex_PCU.hpp"

class Widget
{
public:
	Widget();
	~Widget();

	void Update();
	void Render() const;

public:
	Vec2 m_pivot;
	Vec2 m_minUV;
	Vec2 m_maxUV;
	Rgba8 m_tintColor;

	bool m_isFocused;
	Widget* m_parentWidget;
	std::vector<Widget*> m_childWidgets;
};