#include "AudioEngine.h"

bool AudioEngine::init_wwise()
{
	AkMemSettings memSettings;
	AK::MemoryMgr::GetDefaultSettings(memSettings);

	if (AK::MemoryMgr::Init(&memSettings) != AK_Success)
	{
		std::cout << "Memory Manager failed to initialise" << std::endl;
	}

	AkStreamMgrSettings stmSettings;
	AK::StreamMgr::GetDefaultSettings(stmSettings);
	if (!AK::StreamMgr::Create(stmSettings)) 
	{ 
		std::cout << "Could not Create the Streaming Manager" << std::endl; 
	}

	AkDeviceSettings deviceSettings;
	AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);
	//g_blockingDevice.Init(deviceSettings);
	if (s_blockingDevice.Init(deviceSettings) != AK_Success)
	{
		std::cout << "Could not Create the low level I/O system" << std::endl;
	}

	// Setting the base path for your banks
	s_blockingDevice.SetBasePath(AKTEXT("./Assets/WwiseProject/Wwise/GeneratedSoundBanks/Windows"));

	AkInitSettings initSettings;
	AkPlatformInitSettings pfInitSettings;
	AK::SoundEngine::GetDefaultInitSettings(initSettings);
	AK::SoundEngine::GetDefaultPlatformInitSettings(pfInitSettings);
	
	if (AK::SoundEngine::Init(&initSettings, &pfInitSettings) != AK_Success)
	{
		std::cout << "Could not initialise the Sound Engine" << std::endl;
	}

	// Initialise the music engine
	AkMusicSettings musicInit;
	AK::MusicEngine::GetDefaultInitSettings(musicInit);
	AK::MusicEngine::Init(&musicInit);

	// Loading banks from your project
	AkBankID bankID;
	AK::SoundEngine::LoadBank("Init.bnk", bankID);
	AK::SoundEngine::LoadBank("SFX.bnk", bankID);
	AK::SoundEngine::LoadBank("Music.bnk", bankID);

	init_comm_settings();
	return true;
}

void AudioEngine::init_comm_settings()
{
	AkCommSettings commSettings;
	AK::Comm::GetDefaultInitSettings(commSettings);
	AK::Comm::Init(commSettings);
}

void AudioEngine::term_wwise()
{
	AK::Comm::Term();
	// Unloading the banks
	AK::SoundEngine::UnloadBank("SFX.bnk", nullptr);
	AK::SoundEngine::UnloadBank("Init.bnk", nullptr);
	AK::SoundEngine::UnloadBank("Music.bnk", nullptr);

	AK::SoundEngine::Term();
	AK::MusicEngine::Term();
	s_blockingDevice.Term();

	if (AK::IAkStreamMgr::Get())
		AK::IAkStreamMgr::Get()->Destroy();

	AK::MemoryMgr::Term();
}

void AudioEngine::create_listener()
{
	AkGameObjectID myDefaultListener = 0;
	AK::SoundEngine::RegisterGameObj(myDefaultListener, "My Default Listener");
	AK::SoundEngine::SetDefaultListeners(&myDefaultListener, 1);
}

#undef PlaySound
void AudioEngine::play_sound(const char* eventName, AkGameObjectID gameObjectID)
{
	AkUniqueID eventID = AK::SoundEngine::GetIDFromString(eventName);
	AK::SoundEngine::PostEvent(eventID, gameObjectID);

	// Commented out as it prevents from being able to stop sound?
	//AK::SoundEngine::UnregisterGameObj(gameObjectID);
}

#undef StopSound
void AudioEngine::stop_sound(const char* eventName, AkGameObjectID gameObjectID)
{
	AkUniqueID eventID = AK::SoundEngine::GetIDFromString(eventName);
	AK::SoundEngine::ExecuteActionOnEvent(eventID, AK::SoundEngine::AkActionOnEventType_Stop, gameObjectID);
}

void AudioEngine::pause_sound(AkPlayingID playingID, bool pause)
{
	if (pause)
	{
		// Pause the sound
		AK::SoundEngine::ExecuteActionOnPlayingID(AK::SoundEngine::AkActionOnEventType_Pause, playingID);
	}
	else
	{
		// Resume the sound
		AK::SoundEngine::ExecuteActionOnPlayingID(AK::SoundEngine::AkActionOnEventType_Resume, playingID);
	}
}

void AudioEngine::set_RTPC_value(const char* rtpcName, float value, AkGameObjectID gameObjectID)
{
	AkRtpcID rtpcID = AK::SoundEngine::GetIDFromString(rtpcName);
	AK::SoundEngine::SetRTPCValue(rtpcID, value, gameObjectID);
}

void AudioEngine::set_bus_effect(const char* busName, int effectIndex, bool enable)
{
	AkUniqueID busID = AK::SoundEngine::GetIDFromString(busName);
	AK::SoundEngine::SetBusEffect(busID, effectIndex, enable);
}

void AudioEngine::set_volume(AkGameObjectID gameObjectID, float volumeDB)
{
	AK::SoundEngine::SetRTPCValue(AKTEXT("Voice Volume"), volumeDB, gameObjectID);
}

void AudioEngine::set_pitch(AkGameObjectID gameObjectID, float pitch)
{
	AK::SoundEngine::SetRTPCValue(AKTEXT("Voice Pitch"), pitch, gameObjectID);
}

AkVector AudioEngine::vector_to_akvec(const maths::Vector3F soundPos)
{
	AkVector vecAk;
	vecAk.X = soundPos.X;
	vecAk.Y = soundPos.Y;
	vecAk.Z = soundPos.Z;
	return vecAk;
}

void AudioEngine::set_game_object_position(AkGameObjectID gameObjectID, const AkVector& position)
{
	AkSoundPosition soundPosition;
	AK::SoundEngine::SetPosition(gameObjectID, soundPosition);
}

void AudioEngine::set_priority(AkGameObjectID gameObject, AkGameObjectID listener, int priority)
{
	AK::SoundEngine::SetGameObjectAuxSendValues(gameObject, nullptr, 0);
	AK::SoundEngine::SetGameObjectOutputBusVolume(gameObject, listener, priority / 100.0f);
}

void AudioEngine::game_main_loop()
{
	int frameCount = 0;
	while (frameCount++ < 300) // Run 300 frames and exit
	{
		auto time = std::chrono::milliseconds(30);
		std::this_thread::sleep_for(time);
		AK::SoundEngine::RenderAudio();
	}
}