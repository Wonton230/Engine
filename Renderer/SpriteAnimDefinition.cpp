#include "SpriteAnimDefinition.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Renderer/SpriteSheet.hpp"
#include "Engine/Renderer/SpriteDefinition.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/NamedStrings.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Renderer/Shader.hpp"

extern Renderer* g_theRenderer;

SpriteAnimDefinition::SpriteAnimDefinition(SpriteSheet const& sheet, int startSpriteIndex, int endSpriteIndex, float framesPerSecond, SpriteAnimPlaybackType playbackType)
	:m_spriteSheet(sheet)
{
	m_name = "NA";
	m_name = "default";
	m_startSpriteIndex = startSpriteIndex;
	m_endSpriteIndex = endSpriteIndex;
	m_framesPerSecond = framesPerSecond;
	m_playbackType = playbackType;
}

SpriteDefinition const& SpriteAnimDefinition::GetSpriteDefAtTime(float seconds) const
{
	int totalFrames = m_endSpriteIndex - m_startSpriteIndex + 1;
	if (totalFrames <= 0) {
		ERROR_AND_DIE("Invalid sprite animation range set");
	}

	switch (m_playbackType)
	{
	case SpriteAnimPlaybackType::ONCE:
	{
		int animationFrameIndex = static_cast<int>(seconds * m_framesPerSecond);
		return m_spriteSheet.GetSpriteDef(static_cast<int>(GetClamped((float)m_startSpriteIndex + (float)animationFrameIndex, (float)m_startSpriteIndex, (float)m_endSpriteIndex)));

		break;
	}

	case SpriteAnimPlaybackType::LOOP:
	{
		int animationFrameIndex = m_startSpriteIndex + (static_cast<int>(seconds * m_framesPerSecond) % totalFrames);
		return m_spriteSheet.GetSpriteDef(animationFrameIndex);
		break;
	}

	case SpriteAnimPlaybackType::PINGPONG:
	{
		int pingPongFrameIndex = static_cast<int>(seconds * m_framesPerSecond) % (totalFrames * 2 - 2);
		if (pingPongFrameIndex >= totalFrames)
		{
			pingPongFrameIndex = (totalFrames * 2 - 2) - pingPongFrameIndex; // reverse direction
		}
		int animationFrameIndex = m_startSpriteIndex + pingPongFrameIndex;
		return m_spriteSheet.GetSpriteDef(animationFrameIndex);
		break;
	}

	default:
		ERROR_AND_DIE("Invalid sprite animation playback type");
	}
}

std::string SpriteAnimDefinition::GetName() const
{
	return m_name;
}

float SpriteAnimDefinition::GetLengthSeconds() const
{
	float totalFrames = m_endSpriteIndex - m_startSpriteIndex + 1.f;
	return totalFrames * (1.f / m_framesPerSecond);
}

bool SpriteAnimDefinition::DidFinishPlayingOnce(float seconds) const
{
	int totalFrames = m_endSpriteIndex - m_startSpriteIndex + 1;
	if (totalFrames <= 0) {
		ERROR_AND_DIE("Invalid sprite animation range set");
	}

	switch (m_playbackType)
	{
	case SpriteAnimPlaybackType::ONCE:
	{
		int animationFrameIndex = static_cast<int>(seconds * m_framesPerSecond);
		if (animationFrameIndex >= totalFrames)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	}

	case SpriteAnimPlaybackType::LOOP:
	{
		int animationFrameIndex = static_cast<int>(seconds * m_framesPerSecond);
		if (animationFrameIndex >= totalFrames)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	}

	case SpriteAnimPlaybackType::PINGPONG:
	{
		int pingPongTotalFrames = static_cast<int>((totalFrames * 2 - 2));
		int animationFrameIndex = static_cast<int>(seconds * m_framesPerSecond);
		if (animationFrameIndex >= pingPongTotalFrames)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	}

	default:
		ERROR_AND_DIE("Invalid sprite animation playback type");
	}
}

bool SpriteAnimDefinition::LoadFromXmlElement(const XmlElement& element)
{
	NamedStrings* animationAttributes = new NamedStrings();
	animationAttributes->PopulateFromXmlElementAttributes(element);

	m_name = animationAttributes->GetValue("name", "Idle");
	std::string shaderName = animationAttributes->GetValue("shader", "Default");
	m_shader = g_theRenderer->CreateOrGetShader(shaderName.c_str());
	return true;
}
