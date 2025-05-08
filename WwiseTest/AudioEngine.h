#pragma once
#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

#include <iostream>
#include <thread>

// Wwise include
#include <AK/SoundEngine/Common/AkMemoryMgr.h>
#include <AK/SoundEngine/Common/AkSoundEngine.h>
#include <AK/SoundEngine/Common/AkModule.h>
#include <AK/SoundEngine/Common/IAkStreamMgr.h>
#include <AK/SoundEngine/Common/AkTypes.h>
#include <AK/MusicEngine/Common/AkMusicEngine.h>
#include <AK/Comm/AkCommunication.h>

// Sample low-level I/O implementation
#include "Win32/AkFilePackageLowLevelIOBlocking.h"

#include "Maths.hpp"
#include "Vector3.hpp"

class AudioEngine
{
public: 

	static bool init_wwise();
	static void init_comm_settings();
	static void term_wwise();
	static void create_listener();

	#undef PlaySound
	static void play_sound(const char* eventName, AkGameObjectID gameObjectID);
	#undef StopSound
	static void stop_sound(const char* eventName, AkGameObjectID gameObjectID);

	static void pause_sound(AkPlayingID playingID, bool pause);
	static void set_RTPC_value(const char* rtpcName, float value, AkGameObjectID gameObjectID);
	static void set_bus_effect(const char* busName, int effectIndex, bool enable);
	static void set_volume(AkGameObjectID gameObjectID, float volumeDB);
	static void set_pitch(AkGameObjectID gameObjectID, float pitch);
	static AkVector vector_to_akvec(const maths::Vector3F soundPos);
	static void set_game_object_position(AkGameObjectID gameObjectID, const AkVector& position);
	static void set_priority(AkGameObjectID gameObject, AkGameObjectID listener, int priority);

	static void game_main_loop();

private:
	inline static CAkDefaultIOHookBlocking s_blockingDevice;
};

#endif // !AUDIOENGINE_H