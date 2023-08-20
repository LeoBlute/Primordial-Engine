#include "VUtils/DebugUtils.hpp"
#include "Audio.hpp"
#include "irrKlang/Include/irrKlang.h"

#define SDEF irrklang::ISoundSource* source = static_cast<irrklang::ISoundSource*>(mRawSource)

class SoundStopReceiver : public irrklang::ISoundStopEventReceiver {
public:
	virtual void OnSoundStopped(irrklang::ISound* sound,
		irrklang::E_STOP_EVENT_CAUSE reason, void* userData) override
	{
		switch (reason)
		{
		case irrklang::ESEC_SOUND_FINISHED_PLAYING:
			break;
		case irrklang::ESEC_SOUND_STOPPED_BY_USER:
			break;
		case irrklang::ESEC_SOUND_STOPPED_BY_SOURCE_REMOVAL:
			break;
		case irrklang::ESEC_FORCE_32_BIT:
			break;
		default:
			break;
		}
	}
};

static inline irrklang::ISoundEngine* rawEngine;
//Useless but implemented
static inline SoundStopReceiver* soundStopCatcher;

namespace Audio {
	void Init()
	{
		rawEngine = irrklang::createIrrKlangDevice();
		if (!rawEngine) [[unlikely]] {
			DEBUG_ERROR("Audio System could not initialize, irrKlang device have\
			not been created properly");
		}
		soundStopCatcher = new SoundStopReceiver();
	}

	void Terminate()
	{
		rawEngine->removeAllSoundSources();
		delete soundStopCatcher;
		rawEngine->drop();
	}

	const float GetVolume()
	{
		return rawEngine->getSoundVolume();
	}

	void PlayFromFile(const char* path)
	{
		irrklang::ISound* rawSound = rawEngine->play2D(path, false, false, true);
		if (rawSound)
			rawSound->setSoundStopEventReceiver(soundStopCatcher);
	}

	void SetVolume(const float value)
	{
		rawEngine->setSoundVolume(value);
	}


	void* GetRawEngine()
	{
		return rawEngine;
	}

	void PlayFromSource(Audio::Source* source)
	{
		if (!source->mRawSource)
			return;

		irrklang::ISoundSource* rawSource =
			static_cast<irrklang::ISoundSource*>(source->mRawSource);

		irrklang::ISound* rawSound =
			rawEngine->play2D(rawSource, false, false, true);

		if(rawSound)
			rawSound->setSoundStopEventReceiver(soundStopCatcher);
	}

	//Source::Source()
	//{
	//	mRawSource = NULL;
	//}

	Source::Source(const char* path, const float volume)
	{
		mRawSource = rawEngine->addSoundSourceFromFile(path);
		SDEF;
		source->setDefaultVolume(volume);
	}

	Source::~Source()
	{
		SDEF;
		rawEngine->removeSoundSource(source);
		mRawSource = NULL;
	}

	void Source::SetFile(const char* path)
	{
		SDEF;
		const float volume = source->getDefaultVolume();
		rawEngine->removeSoundSource(source);
		source = rawEngine->addSoundSourceFromFile(path);
		source->setDefaultVolume(volume);
		mRawSource = source;
	}
}