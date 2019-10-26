
/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Ramzi Mourtada
Other Authors: <None>
- End Header --------------------------------------------------------*/

#include "AudioManager.h"
#include "EventManager.h"
#include "ResourceManager.h"

AudioManager::AudioManager()
{
	FMOD_RESULT result = FMOD::System_Create(&mp_system);
	if (result != FMOD_OK)
		CheckError(result);

	int driverCount = 0;
	mp_system->getNumDrivers(&driverCount);

	if (driverCount == 0)
	{
		// Report Error
		std::cout << "Audio drivers couldn't be initialized" << std::endl;
		return;
	}

	// Initialize our Instance with 36 Channels
	mp_system->init(36, FMOD_INIT_NORMAL, NULL);
	mp_system->getMasterChannelGroup(&mp_master_channel);
	for (int i = 0; i < CATEGORY_COUNT; ++i) {
		mp_system->createChannelGroup(0, &groups[i]);
		mp_master_channel->addGroup(groups[i]);
	}

	m_modes[CATEGORY_SFX] = FMOD_DEFAULT;
	m_modes[CATEGORY_SONG] = FMOD_DEFAULT | FMOD_CREATESTREAM | FMOD_LOOP_NORMAL;

	//TODO: Load audio from previously saved settings in userdata.json
	EventManager::Get()->SubscribeEvent<PlaySongEvent>(this, std::bind(&AudioManager::OnPlaySong, this, std::placeholders::_1));
	EventManager::Get()->SubscribeEvent<PlaySFXEvent>(this, std::bind(&AudioManager::OnPlaySFX, this, std::placeholders::_1));
}

AudioManager::~AudioManager()
{
	//TODO: Load audio from previously saved settings in userdata.json
	EventManager::Get()->UnsubscribeEvent<PlaySongEvent>(this);
	EventManager::Get()->UnsubscribeEvent<PlaySFXEvent>(this);

	SoundMap::iterator iter;
	for (iter = m_sounds.begin(); iter != m_sounds.end(); ++iter)
		iter->second.m_pSound->release();
	m_sounds.clear();
	mp_system->release();
}

void AudioManager::OnPlaySong(const PlaySongEvent* e)
{
	PlaySong(e->m_songPath);
}

void AudioManager::OnPlaySFX(const PlaySFXEvent* e)
{
	PlaySFX(e->m_sfxPath);
}

void AudioManager::RegisterSound(SoundData data)
{
	if (m_sounds.find(data.m_id) != m_sounds.end())
		return;
	
	m_sounds.insert(std::make_pair(data.m_id, data));
}

void AudioManager::UnregisterSound(StringId id)
{
	if (m_sounds.find(id) == m_sounds.end())
		return;
	m_sounds.erase(id);
}

void AudioManager::Update(float dt)
{
	const float fadeTime = 1.0f; // in seconds
	if (currentSong != 0 && fade == FADE_IN)
	{
		float volume;
		currentSong->getVolume(&volume);
		float nextVolume = volume + dt / fadeTime;
		if (nextVolume >= 1.0f)
		{
			currentSong->setVolume(1.0f);
			fade = FADE_NONE;
		}
		else
		{
			currentSong->setVolume(nextVolume);
		}
	}
	else if (currentSong != 0 && fade == FADE_OUT)
	{
		float volume;
		currentSong->getVolume(&volume);
		float nextVolume = volume - dt / fadeTime;
		if (nextVolume <= 0.0f)
		{
			currentSong->stop();
			currentSong = 0;
			currentSongPath = 0;
			fade = FADE_NONE;
		}
		else
		{
			currentSong->setVolume(nextVolume);
		}
	}
	else if (currentSong == 0 && !(nextSongPath == 0))
	{
		PlaySong(nextSongPath);
		nextSongPath = 0;
	}
	mp_system->update();
	SetMasterVolume(3.f);
	SetSongVolume(3.f);
	SetSFXVolume(3.f);
	//// Cheat Code for Audio
	//if (World::Get()->Get_Input_Manager()->is_Key_Triggered(SDL_SCANCODE_M))
	//	Set_Song_Volume(0.f);
}

void AudioManager::PlaySFX(StringId SongName)
{
	const auto& it = m_sounds.find(SongName);
	if (it == m_sounds.end())
		return;
	const SoundPtr pSound = it->second.m_pSound;

	//float volume = RandomBetween(minVolume, maxVolume);
	//float pitch = (float)(Utility::random_range(1, 20));

	// Play the sound effect with these initial values
	FMOD::Channel* channel;
	mp_system->playSound(pSound, groups[CATEGORY_SFX], true, &channel);
	//channel->setChannelGroup(groups[CATEGORY_SFX]);
	//channel->setVolume(volume);

	//float frequency;
	//channel->getFrequency(&frequency);
	//channel->setFrequency(Change_Semitone(frequency, pitch));

	channel->setPaused(false);
}

void AudioManager::StopSFX()
{
	groups[CATEGORY_SFX]->stop();
}

void AudioManager::PlaySong(StringId songName)
{
	// Ignore if this song is already playing
	if (currentSongPath == songName)
		return;

	// If a song is playing stop them and set this as the next song
	if (currentSong != 0)
	{
		StopSong();
		nextSongPath = songName;
		return;
	}

	// Find the song in the corresponding sound map
	const auto& it = m_sounds.find(songName);
	if (it == m_sounds.end())
		return;
	const SoundPtr pSound = it->second.m_pSound;
	// Start playing song with volume set to 0 and fade in
	currentSongPath = songName;
	mp_system->playSound(pSound, NULL, true, &currentSong);
	currentSong->setChannelGroup(groups[CATEGORY_SONG]);
	currentSong->setVolume(0.0f);
	currentSong->setPaused(false);
	fade = FADE_IN;
}

StringId AudioManager::GetCurrentSong() const
{
	return currentSongPath;
}

void AudioManager::StopSong()
{
	if (currentSong != 0)
		fade = FADE_OUT;
	nextSongPath = 0;
}

void AudioManager::PauseSong()
{
	groups[CATEGORY_SONG]->setPaused(true);
}

void AudioManager::ContinueSong()
{
	groups[CATEGORY_SONG]->setPaused(false);
}

void AudioManager::SetMasterVolume(float volume)
{
	if (volume < 0.f)
		volume = 0.f;
	else if (volume > 3.f)
		volume = 3.f;
	mp_master_channel->setVolume(volume);
}

void AudioManager::IncrementMasterVolume()
{
	float volume;
	mp_master_channel->getVolume(&volume);
	float nextVolume = volume + 0.2f;
	SetMasterVolume(nextVolume);
}

void AudioManager::DecrementSliderVolume()
{
	float volume;
	mp_master_channel->getVolume(&volume);
	float nextVolume = volume - 0.2f;
	SetMasterVolume(nextVolume);
}

void AudioManager::SetSFXVolume(float volume)
{
	if (volume < 0.f)
		volume = 0.f;
	else if (volume > 3.f)
		volume = 3.f;
	groups[CATEGORY_SFX]->setVolume(volume);
}

void AudioManager::IncrementSFXVolume()
{
	float volume;
	groups[CATEGORY_SFX]->getVolume(&volume);
	float nextVolume = volume + 0.2f;
	SetSFXVolume(nextVolume);
}

void AudioManager::DecrementSFXVolume()
{
	float volume;
	groups[CATEGORY_SFX]->getVolume(&volume);
	float nextVolume = volume - 0.2f;
	SetSFXVolume(nextVolume);
}

void AudioManager::SetSongVolume(float volume)
{
	if (volume < 0.f)
		volume = 0.f;
	else if (volume > 3.f)
		volume = 3.f;
	groups[CATEGORY_SONG]->setVolume(volume);
}

void AudioManager::CheckError(const FMOD_RESULT result)
{
	std::cout << FMOD_ErrorString(result) << std::endl;
	assert(!result);
}

void AudioManager::IncrementSongVolume()
{
	float volume;
	groups[CATEGORY_SONG]->getVolume(&volume);
	float nextVolume = volume + 0.2f;
	SetSongVolume(nextVolume);
	//std::cout << "Volume: " << nextVolume << std::endl;
}

void AudioManager::DecrementSongVolume()
{
	float volume;
	groups[CATEGORY_SONG]->getVolume(&volume);
	float nextVolume = volume - 0.2f;
	SetSongVolume(nextVolume);
	//std::cout << "Volume: " << nextVolume << std::endl;
}

float AudioManager::Change_Semitone(float freq, float pitch)
{
	static float semitone_ratio = pow(2.0f, 1.0f / 12.0f);
	return freq * pow(semitone_ratio, pitch);
}

void AudioManager::Save_Settings()
{
	//std::string path = User_Data_Generator::Get_User_Settings_Path();
	//CSimpleIniA ini;
	//ini.SetUnicode();
	//auto err = ini.LoadFile(path.c_str());
	//assert(err == SI_Error::SI_OK);

	//float volume;
	//mp_master_channel->getVolume(&volume);
	//ini.SetDoubleValue("Audio_Settings", "Master", volume);

	//groups[CATEGORY_SONG]->getVolume(&volume);
	//ini.SetDoubleValue("Audio_Settings", "Music", volume);

	//groups[CATEGORY_SFX]->getVolume(&volume);
	//ini.SetDoubleValue("Audio_Settings", "SFX", volume);
	//ini.SaveFile(path.c_str());
}