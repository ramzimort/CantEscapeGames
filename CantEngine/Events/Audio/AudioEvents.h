#pragma once

#pragma once

#include "Events/Event.h"
#include "Events/Multicast.h"
#include "Helper/Hash.h"


class PlaySongEvent : public Event<PlaySongEvent>
{
public:
	PlaySongEvent(const std::string& songPath) : m_songPath(songPath)
	{
		//DEBUG_LOG("Pushing State: %s\n", levelPath.c_str());
		//OnNewStatePushed()((int)key_scancode, press);
	}
	virtual ~PlaySongEvent() { }
	virtual void operator()()
	{
	}

	const std::string m_songPath;
};

class PlaySFXEvent : public Event<PlaySFXEvent>
{
public:
	PlaySFXEvent(const std::string& sfxPath) : m_sfxPath(sfxPath)
	{
		//DEBUG_LOG("Pushing State: %s\n", levelPath.c_str());
		//OnNewStatePushed()((int)key_scancode, press);
	}
	virtual ~PlaySFXEvent() { }
	virtual void operator()()
	{
	}

	const StringId m_sfxPath;
};

