#include "Engine/ThirdParty/TinyXML2/tinyxml2.hpp"
#include "Engine/Core/StringUtils.hpp"
#pragma once

struct Vec2;
struct Vec3;
struct IntVec2;
struct Rgba8;
struct EulerAngles;
struct FloatRange;

int			ParseXmlAttribute(XmlElement const& element, char const* attributename, int defaultValue);
char		ParseXmlAttribute(XmlElement const& element, char const* attributename, char defaultValue);
bool		ParseXmlAttribute(XmlElement const& element, char const* attributename, bool defaultValue);
float		ParseXmlAttribute(XmlElement const& element, char const* attributename, float defaultValue);
Rgba8		ParseXmlAttribute(XmlElement const& element, char const* attributename, Rgba8 const& defaultValue);
Vec2		ParseXmlAttribute(XmlElement const& element, char const* attributename, Vec2 const& defaultValue);
Vec3		ParseXmlAttribute(XmlElement const& element, char const* attributename, Vec3 const& defaultValue);
EulerAngles ParseXmlAttribute(XmlElement const& element, char const* attributename, EulerAngles const& defaultValue);
FloatRange	ParseXmlAttribute(XmlElement const& element, char const* attributename, FloatRange const& defaultValue);
IntVec2		ParseXmlAttribute(XmlElement const& element, char const* attributename, IntVec2 const& defaultValue);
std::string	ParseXmlAttribute(XmlElement const& element, char const* attributename, std::string const& defaultValue);
std::string ParseXmlAttribute(XmlElement const& element, char const* attributename, char const* defaultValue);
Strings		ParseXmlAttribute(XmlElement const& element, char const* attributename, Strings const& defaultValue);