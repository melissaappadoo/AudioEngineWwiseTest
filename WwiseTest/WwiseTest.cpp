#include "AudioEngine.h"

#include <AK/SoundEngine/Common/AkMemoryMgr.h>
#include <AK/SoundEngine/Common/AkSoundEngine.h>
#include <AK/SoundEngine/Common/AkModule.h>
#include <AK/SoundEngine/Common/IAkStreamMgr.h>
#include <AK/Comm/AkCommunication.h>
#undef PlaySound

int main(int argc, const char* argv[])
{
	if (AudioEngine::init_wwise())
	{
		//Gives you 5 seconds to connect before the sound starts
		auto time = std::chrono::seconds(5);
		std::this_thread::sleep_for(time);

		AkGameObjectID myGameObj = 1;
		AK::SoundEngine::RegisterGameObj(myGameObj, "GameObj1");
		AudioEngine::create_listener();
		AudioEngine::set_RTPC_value("Voice Volume", 75.0f, myGameObj);
		AudioEngine::play_sound("Play_SFX_1", myGameObj);
		AudioEngine::play_sound("Play_MainMenu_Music", myGameObj);
		AudioEngine::game_main_loop();
		AK::Comm::Term();
	}

	AudioEngine::term_wwise();

	return 0;
}