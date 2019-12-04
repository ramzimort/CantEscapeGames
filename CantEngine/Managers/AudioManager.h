#pragma once
#include "ResourceManager.h"
#include "Audio/AudioTypes.h"
#include "Events/Audio/AudioEvents.h"

/**
 * @file AudioManager.h
 * @author Ramzi Mourtada
 * @date 12/4/2019
 * @brief Audio Manager Class for reacting to play/pause/stop events, and adjusting audio settings
 */

enum FadeState { FADE_NONE, FADE_IN, FADE_OUT };
typedef FMOD::Sound* SoundPtr;

/**
 * @brief Information to the loaded audio file
 * 
 */
struct SoundData
{
	SoundData() = default;
	SoundData(StringId id, SoundPtr ptr, Category type) : 
		m_id(id),  m_pSound(ptr), m_type(type) { }

	StringId m_id;
	SoundPtr m_pSound;
	Category m_type;
};

/**
 * @brief Reacts to song and sfx events
 * 
 */
class AudioManager
{
private:
	typedef std::unordered_map<StringId, SoundData, StringIdHash> SoundMap;
	friend class ResourceManager;
public:
	AudioManager();
	~AudioManager();
	/**
	 * @brief Play SFX file path (must be preloaded)
	 * 
	 * @param pFile 
	 */
	void PlaySFX(StringId pFile);
	/**
	 * @brief Play song file path (must be preloaded)
	 * 
	 * @param pFile 
	 */
	void PlaySong(StringId pFile);
	/**
	 * @brief Get the Current Playing song name
	 * 
	 * @return StringId 
	 */
	StringId GetCurrentSong() const;
	/**
	 * @brief Stop SFX Channel 
	 * 
	 */
	void StopSFX();
	/**
	 * @brief Stop Song Channel
	 * 
	 */
	void StopSong();
	/**
	 * @brief Pause Song
	 * 
	 */
	void PauseSong();
	/**
	 * @brief Continue Playing Song
	 * 
	 */
	void ContinueSong();
	/**
	 * @brief Set the Master Volume
	 * 
	 * @param volume 0-100
	 */
	void SetMasterVolume(float volume);
	void IncrementMasterVolume();
	void DecrementSliderVolume();
	/**
	 * @brief Set the SFX Volume
	 * 
	 * @param volume 0-100
	 */
	void SetSFXVolume(float volume);
	void IncrementSFXVolume();
	void DecrementSFXVolume();

	/**
	 * @brief Set the Song Volume
	 * 
	 * @param volume  0-100
	 */
	void SetSongVolume(float volume);
	void IncrementSongVolume();
	void DecrementSongVolume();
	/**
	 * @brief Per frame update
	 * Used for fade-in fade-out related changes
	 * 
	 * @param dt 
	 */
	void Update(float dt);
	/**
	 * @brief Check the FMOD Error
	 * 
	 * @param result 
	 */
	void CheckError(FMOD_RESULT result);
	/**
	 * @brief Save Audio Settings
	 * 
	 */

private:
	void RegisterSound(SoundData data);
	void UnregisterSound(StringId id);

private:
	void OnPlaySong(const PlaySongEvent* e);
	void OnStopSong(const StopSongEvent* e) { StopSong(); }
	void OnSetMasterVolume(const SetVolumeEvent* e) 
	{ 
		switch (e->m_channelId)
		{
		case 0:
			SetMasterVolume(e->m_value);
			break;
		case 1:
			SetSongVolume(e->m_value);
			break;
		case 2:
			SetSFXVolume(e->m_value);
			break;
		default:
			assert(0);
			break;
		}
	}
	void OnPlaySFX(const PlaySFXEvent* e);

	float Change_Semitone(float frequency, float pitch);
private:
	//void Load(Category type, const std::string& path);
	SoundMap m_sounds;


	FMOD::System* mp_system;
	FMOD::ChannelGroup* mp_master_channel;
	FMOD::ChannelGroup* groups[CATEGORY_COUNT];
	FMOD_MODE m_modes[CATEGORY_COUNT];
	FMOD::Channel* currentSong;
	StringId currentSongPath;
	StringId nextSongPath;
	FadeState fade;
	SoundPtr* mp_stream;
	float m_master_volume, m_music_volume, m_SFX_volume;
};