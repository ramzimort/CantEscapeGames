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
  * @brief Stops a song
  *
  */
class StopSongEvent : public Event<StopSongEvent>
{
public:
	/**
	 * @brief Stops current song.
	 */
	StopSongEvent() = default;
	virtual ~StopSongEvent() = default;
	virtual void operator()() {	}
};

/**
 * @brief Stops a song
 *
 */
class SetVolumeEvent : public Event<SetVolumeEvent>
{
public:
	/**
	 * @brief Sets current volume.
	 */
	SetVolumeEvent(float value, uint32_t channelId) : 
		m_value(value), m_channelId(channelId) { };
	virtual ~SetVolumeEvent() = default;
	virtual void operator()() {	}

	/**
	* @brief New Volume
	*
	*/
	float m_value;

	/**
	* @brief Channel (0: Master, 1: Music, 2: SFX)
	*
	*/
	uint32_t m_channelId;
};

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

