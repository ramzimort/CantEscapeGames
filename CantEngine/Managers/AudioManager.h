#pragma once

/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Ramzi Mourtada
Other Authors: <None>
- End Header --------------------------------------------------------*/

#include "ResourceManager.h"
#include "Audio/AudioTypes.h"
#include "Events/Audio/AudioEvents.h"

enum FadeState { FADE_NONE, FADE_IN, FADE_OUT };
typedef FMOD::Sound* SoundPtr;

struct SoundData
{
	SoundData() = default;
	SoundData(StringId id, SoundPtr ptr, Category type) : 
		m_id(id),  m_pSound(ptr), m_type(type) { }

	StringId m_id;
	SoundPtr m_pSound;
	Category m_type;
};

class AudioManager
{
private:
	typedef std::unordered_map<StringId, SoundData, StringIdHash> SoundMap;
	friend class ResourceManager;
public:
	AudioManager();
	~AudioManager();

	void OnPlaySong(const PlaySongEvent* e);
	void OnPlaySFX(const PlaySFXEvent* e);

	void PlaySFX(StringId pFile);
	void PlaySong(StringId pFile);
	StringId GetCurrentSong() const;
	void StopSFX();
	void StopSong();
	void PauseSong();
	void ContinueSong();

	void SetMasterVolume(float volume);
	void IncrementMasterVolume();
	void DecrementSliderVolume();

	void SetSFXVolume(float volume);
	void IncrementSFXVolume();
	void DecrementSFXVolume();

	void SetSongVolume(float volume);
	void IncrementSongVolume();
	void DecrementSongVolume();

	void Update(float dt);
	void CheckError(FMOD_RESULT result);

	void Save_Settings();

private:
	void RegisterSound(SoundData data);
	void UnregisterSound(StringId id);

private:
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

