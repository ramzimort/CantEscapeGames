#pragma once

#pragma once

/**
 * @file AudioEvents.h
 * @author Ramzi Mourtada
 * @date 12/4/2019
 * @brief Audio Events
 */


#include "Events/Event.h"
#include "Events/Multicast.h"
#include "Helper/Hash.h"

/**
 * @brief Plays a song
 * 
 */
class PlaySongEvent : public Event<PlaySongEvent>
{
public:
	/**
	 * @brief Plays a song.
	 * 
	 * @param songPath 
	 */
	PlaySongEvent(const std::string& songPath) : m_songPath(songPath)
	{
		//DEBUG_LOG("Pushing State: %s\n", levelPath.c_str());
		//OnNewStatePushed()((int)key_scancode, press);
	}
	virtual ~PlaySongEvent() { }
	virtual void operator()()
	{
	}

	/**
	 * @brief Song Path
	 * 
	 */
	const std::string m_songPath;
};

/**
 * @brief Plays a sound effect
 * 
 */
class PlaySFXEvent : public Event<PlaySFXEvent>
{
public:
	/**
	 * @brief Plays a sound effect
	 * 
	 * @param sfxPath 
	 */
	PlaySFXEvent(const std::string& sfxPath) : m_sfxPath(sfxPath)
	{
		//DEBUG_LOG("Pushing State: %s\n", levelPath.c_str());
		//OnNewStatePushed()((int)key_scancode, press);
	}
	virtual ~PlaySFXEvent() { }
	virtual void operator()()
	{
	}

	/**
	 * @brief SFX Path
	 * 
	 */
	const StringId m_sfxPath;
};

