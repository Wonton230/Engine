#pragma once

struct Rgba8
{
public:
	unsigned char r = 255;
	unsigned char g = 255;
	unsigned char b = 255;
	unsigned char a = 255;
public:
	// Construction/Destruction
	~Rgba8() {}												// destructor (do nothing)
	Rgba8() {}												// default constructor (do nothing)	
	explicit Rgba8(unsigned char initialR, unsigned char initialG, unsigned char initialB, unsigned char initialA);		// explicit constructor (from r,g,b,a)

	void SetFromText(char const* text);
	void GetAsFloats(float* colorAsFloats) const;
	bool operator==(const Rgba8& rhs) const;

	//constants
	static const Rgba8 WHITE;
	static const Rgba8 BLACK;
	static const Rgba8 RED;
	static const Rgba8 GREEN;
	static const Rgba8 BLUE;
	static const Rgba8 CYAN;
	static const Rgba8 PINK;
	static const Rgba8 YELLOW;
	static const Rgba8 ORANGE;
	static const Rgba8 GRAY;
}; 

Rgba8 InterpolateColor(Rgba8 start, Rgba8 end, float fraction);