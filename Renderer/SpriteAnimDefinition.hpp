#pragma once
#include "Engine/Core/XmlUtils.hpp"

class SpriteSheet;
class SpriteDefinition;
class Shader;

enum class SpriteAnimPlaybackType
{
	ONCE,		// for 5-frame anim, plays 0,1,2,3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4...
	LOOP,		// for 5-frame anim, plays 0,1,2,3,4,0,1,2,3,4,0,1,2,3,4,0,1,2,3,4,0,1,2,3,4,0...
	PINGPONG,	// for 5-frame anim, plays 0,1,2,3,4,3,2,1,0,1,2,3,4,3,2,1,0,1,2,3,4,3,2,1,0,1...
};

//------------------------------------------------------------------------------------------------
class SpriteAnimDefinition
{
public:
	SpriteAnimDefinition(SpriteSheet const& sheet, int startSpriteIndex, int endSpriteIndex,
		float framesPerSecond, SpriteAnimPlaybackType playbackType = SpriteAnimPlaybackType::LOOP);

	SpriteDefinition const& GetSpriteDefAtTime(float seconds) const;
	std::string				GetName() const;
	float					GetLengthSeconds() const;
	bool					DidFinishPlayingOnce(float seconds) const;
	bool					LoadFromXmlElement(const XmlElement& element);

private:
	std::string				m_name = "Idle";
	Shader*					m_shader = nullptr;
	SpriteSheet const&		m_spriteSheet;
	int						m_startSpriteIndex = -1;
	int						m_endSpriteIndex = -1;
	float					m_framesPerSecond = 1.f;
	SpriteAnimPlaybackType	m_playbackType = SpriteAnimPlaybackType::LOOP;
};
