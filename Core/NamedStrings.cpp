#include "Engine/Core/NamedStrings.hpp"
#include "Engine/Core/XmlUtils.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/IntVec2.hpp"
#include "Engine/Core/Rgba8.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/EulerAngles.hpp"
#include "Engine/Math/FloatRange.hpp"

void NamedStrings::PopulateFromXmlElementAttributes(XmlElement const& element)
{
	if (element.FirstAttribute() == nullptr)
	{
		return;
	}

	const XmlAttribute* attribute = element.FirstAttribute();
	while (attribute)
	{
		m_keyValuePairs[attribute->Name()] = attribute->Value();
		attribute = attribute->Next();
	}
}

void NamedStrings::SetValue(std::string const& keyName, std::string const& newValue)
{
	m_keyValuePairs[keyName] = newValue;
}

std::string NamedStrings::GetValue(std::string const& keyName, std::string const& defaultValue) const
{
	std::string result;
	if (m_keyValuePairs.find(keyName) == m_keyValuePairs.end())
	{
		return defaultValue;
	}
	result = m_keyValuePairs.at(keyName);
	return result;
}

bool NamedStrings::GetValue(std::string const& keyName, bool defaultValue) const
{
	std::string str;
	if (m_keyValuePairs.find(keyName) == m_keyValuePairs.end())
	{
		return defaultValue;
	}

	str = m_keyValuePairs.at(keyName);
	if (str.compare("true") == 0)
	{
		return true;
	}
	else if (str.compare("false") == 0)
	{
		return false;
	}
	else
	{
		return defaultValue;
	}
}

int NamedStrings::GetValue(std::string const& keyName, int defaultValue) const
{
	std::string str;
	if (m_keyValuePairs.find(keyName) == m_keyValuePairs.end())
	{
		return defaultValue;
	}

	str = m_keyValuePairs.at(keyName);
	return atoi(str.c_str());
}

float NamedStrings::GetValue(std::string const& keyName, float defaultValue) const
{
	std::string str;
	if (m_keyValuePairs.find(keyName) == m_keyValuePairs.end())
	{
		return defaultValue;
	}

	str = m_keyValuePairs.at(keyName);
	return static_cast<float>(atof(str.c_str()));
}

std::string NamedStrings::GetValue(std::string const& keyName, char const* defaultValue) const
{
	std::string result;
	if (m_keyValuePairs.find(keyName) == m_keyValuePairs.end())
	{
		return defaultValue;
	}
	result = m_keyValuePairs.at(keyName);
	return result;
}

Rgba8 NamedStrings::GetValue(std::string const& keyName, Rgba8 const& defaultValue) const
{
	std::string str;
	if (m_keyValuePairs.find(keyName) == m_keyValuePairs.end())
	{
		return defaultValue;
	}

	str = m_keyValuePairs.at(keyName);
	Rgba8 result = Rgba8();
	result.SetFromText(str.c_str());
	return result;
}

Vec2 NamedStrings::GetValue(std::string const& keyName, Vec2 const& defaultValue) const
{
	std::string str;
	if (m_keyValuePairs.find(keyName) == m_keyValuePairs.end())
	{
		return defaultValue;
	}

	str = m_keyValuePairs.at(keyName);
	Vec2 result = Vec2();
	result.SetFromText(str.c_str());
	return result;
}

Vec3 NamedStrings::GetValue(std::string const& keyName, Vec3 const& defaultValue) const
{
	std::string str;
	if (m_keyValuePairs.find(keyName) == m_keyValuePairs.end())
	{
		return defaultValue;
	}

	str = m_keyValuePairs.at(keyName);
	Vec3 result = Vec3();
	result.SetFromText(str.c_str());
	return result;
}

EulerAngles NamedStrings::GetValue(std::string const& keyName, EulerAngles const& defaultValue) const
{
	std::string str;
	if (m_keyValuePairs.find(keyName) == m_keyValuePairs.end())
	{
		return defaultValue;
	}

	str = m_keyValuePairs.at(keyName);
	EulerAngles result = EulerAngles();
	result.SetFromText(str.c_str());
	return result;
}

IntVec2 NamedStrings::GetValue(std::string const& keyName, IntVec2 const& defaultValue) const
{
	std::string str;
	if (m_keyValuePairs.find(keyName) == m_keyValuePairs.end())
	{
		return defaultValue;
	}

	str = m_keyValuePairs.at(keyName);
	IntVec2 result = IntVec2();
	result.SetFromText(str.c_str());
	return result;
}

FloatRange NamedStrings::GetValue(std::string const& keyName, FloatRange const& defaultValue) const
{
	std::string str;
	if (m_keyValuePairs.find(keyName) == m_keyValuePairs.end())
	{
		return defaultValue;
	}

	str = m_keyValuePairs.at(keyName);
	FloatRange result = FloatRange();
	result.SetFromText(str.c_str());
	return result;
}
