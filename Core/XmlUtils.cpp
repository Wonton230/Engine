#include "XmlUtils.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/IntVec2.hpp"
#include "Engine/Core/Rgba8.hpp"
#include "Engine/Math/EulerAngles.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/FloatRange.hpp"

int ParseXmlAttribute(XmlElement const& element, char const* attributename, int defaultValue)
{
	if (element.FindAttribute(attributename) == nullptr)
	{
		return defaultValue;
	}
    int var = element.FindAttribute(attributename)->IntValue();
	return var;
}

char ParseXmlAttribute(XmlElement const& element, char const* attributename, char defaultValue)
{
	if (element.FindAttribute(attributename) == nullptr)
	{
		return defaultValue;
	}
	char var = static_cast<char>(element.FindAttribute(attributename)->IntValue());
	return var;
}

bool ParseXmlAttribute(XmlElement const& element, char const* attributename, bool defaultValue)
{
	if (element.FindAttribute(attributename) == nullptr)
	{
		return defaultValue;
	}
	bool var = element.FindAttribute(attributename)->BoolValue();
	return var;
}

float ParseXmlAttribute(XmlElement const& element, char const* attributename, float defaultValue)
{
	if (element.FindAttribute(attributename) == nullptr)
	{
		return defaultValue;
	}
	float var = element.FindAttribute(attributename)->FloatValue();
	return var;
}

Rgba8 ParseXmlAttribute(XmlElement const& element, char const* attributename, Rgba8 const& defaultValue)
{
	if (element.FindAttribute(attributename) == nullptr)
	{
		return defaultValue;
	}
	Rgba8 var;
	var.SetFromText(element.FindAttribute(attributename)->Value());
	return var;
}

Vec2 ParseXmlAttribute(XmlElement const& element, char const* attributename, Vec2 const& defaultValue)
{
	if (element.FindAttribute(attributename) == nullptr)
	{
		return defaultValue;
	}
	Vec2 var;
	var.SetFromText(element.FindAttribute(attributename)->Value());
	return var;
}

Vec3 ParseXmlAttribute(XmlElement const& element, char const* attributename, Vec3 const& defaultValue)
{
	if (element.FindAttribute(attributename) == nullptr)
	{
		return defaultValue;
	}
	Vec3 var;
	var.SetFromText(element.FindAttribute(attributename)->Value());
	return var;
}

EulerAngles ParseXmlAttribute(XmlElement const& element, char const* attributename, EulerAngles const& defaultValue)
{
	if (element.FindAttribute(attributename) == nullptr)
	{
		return defaultValue;
	}
	EulerAngles var;
	var.SetFromText(element.FindAttribute(attributename)->Value());
	return var;
}

FloatRange ParseXmlAttribute(XmlElement const& element, char const* attributename, FloatRange const& defaultValue)
{
	if (element.FindAttribute(attributename) == nullptr)
	{
		return defaultValue;
	}
	FloatRange var;
	var.SetFromText(element.FindAttribute(attributename)->Value());
	return var;
}

IntVec2 ParseXmlAttribute(XmlElement const& element, char const* attributename, IntVec2 const& defaultValue)
{
	if (element.FindAttribute(attributename) == nullptr)
	{
		return defaultValue;
	}
	IntVec2 var;
	var.SetFromText(element.FindAttribute(attributename)->Value());
	return var;
}

std::string ParseXmlAttribute(XmlElement const& element, char const* attributename, std::string const& defaultValue)
{
	if (element.FindAttribute(attributename) == nullptr)
	{
		return defaultValue;
	}
	std::string var;
	var = element.FindAttribute(attributename)->Value();
	return var;
}

std::string ParseXmlAttribute(XmlElement const& element, char const* attributename, char const* defaultValue)
{
	if (element.FindAttribute(attributename) == nullptr)
	{
		return defaultValue;
	}
	std::string var;
	var = element.FindAttribute(attributename)->Value();
	return var;
}

Strings ParseXmlAttribute(XmlElement const& element, char const* attributename, Strings const& defaultValue)
{
	if (element.FindAttribute(attributename) == nullptr)
	{
		return defaultValue;
	}
	std::string var;
	var = element.FindAttribute(attributename)->Value();
	return SplitStringOnDelimiter(var, ',');
}
