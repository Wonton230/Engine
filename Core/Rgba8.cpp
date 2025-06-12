#include "Engine/Core/Rgba8.hpp"
#include "Engine//Core/ErrorWarningAssert.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Math/MathUtils.hpp"

const Rgba8 Rgba8::WHITE = Rgba8(255, 255, 255, 255);
const Rgba8 Rgba8::BLACK =	Rgba8(0, 0, 0, 255);
const Rgba8 Rgba8::RED =	Rgba8(255, 0, 0, 255);
const Rgba8 Rgba8::GREEN =	Rgba8(0, 255, 0, 255);
const Rgba8 Rgba8::BLUE =	Rgba8(0, 0, 255, 255);
const Rgba8 Rgba8::CYAN =	Rgba8(0, 255, 255, 255);
const Rgba8 Rgba8::PINK =	Rgba8(255, 0, 255, 255);
const Rgba8 Rgba8::YELLOW = Rgba8(255, 255, 0, 255);
const Rgba8 Rgba8::ORANGE = Rgba8(255, 150, 0, 255);
const Rgba8 Rgba8::GRAY =	Rgba8(127, 127, 127, 255);

Rgba8::
Rgba8::Rgba8(unsigned char initialR, unsigned char initialG, unsigned char initialB, unsigned char initialA)
{
	r = initialR;
	g = initialG;
	b = initialB;
	a = initialA;
}

bool Rgba8::operator==(const Rgba8& rhs) const
{
	if (r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a)
	{
		return true;
	}
	return false;
}

void Rgba8::SetFromText(char const* text)
{
	Strings variables = SplitStringOnDelimiter(text, ',');
	if ((variables.size() != 3 && variables.size() != 4) || variables[0].c_str() == nullptr || variables[1].c_str() == nullptr || variables[2].c_str() == nullptr)
	{
		ERROR_AND_DIE("Invalid arguments passed into RGBA8 String");
	}
	r = static_cast<unsigned char>(atoi(variables[0].c_str()));
	g = static_cast<unsigned char>(atoi(variables[1].c_str()));
	b = static_cast<unsigned char>(atoi(variables[2].c_str()));

	if (variables.size() == 4 && variables[3].c_str() != nullptr)
	{
		a = static_cast<unsigned char>(atoi(variables[3].c_str()));
	}
	else
	{
		a = 255;
	}
}

void Rgba8::GetAsFloats(float* colorAsFloats) const
{
	colorAsFloats[0] = RangeMap(r, 0.f, 255.f, 0.f, 1.f);
	colorAsFloats[1] = RangeMap(g, 0.f, 255.f, 0.f, 1.f);
	colorAsFloats[2] = RangeMap(b, 0.f, 255.f, 0.f, 1.f);
	colorAsFloats[3] = RangeMap(a, 0.f, 255.f, 0.f, 1.f);
}

Rgba8 InterpolateColor(Rgba8 start, Rgba8 end, float fraction)
{
	float lerpR = Interpolate(NormalizeByte(start.r), NormalizeByte(end.r), fraction);
	float lerpG = Interpolate(NormalizeByte(start.g), NormalizeByte(end.g), fraction);
	float lerpB = Interpolate(NormalizeByte(start.b), NormalizeByte(end.b), fraction);
	float lerpA = Interpolate(NormalizeByte(start.a), NormalizeByte(end.a), fraction);
	//r = DenormailzeByte(lerpR);
	//g = DenormailzeByte(lerpG);
	//b = DenormailzeByte(lerpB);
	//a = DenormailzeByte(lerpA);
	return Rgba8(DenormalizeByte(lerpR), DenormalizeByte(lerpG), DenormalizeByte(lerpB), DenormalizeByte(lerpA));
}
