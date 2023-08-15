#include "Audio.hpp"
#include "irrKlang/Include/irrKlang.h"

#define SDEF irrklang::ISoundSource* source = static_cast<irrklang::ISoundSource*>(mRawSource)

static inline irrklang::ISoundEngine* rawEngine;

namespace Audio {
	void Init()
	{
		rawEngine = irrklang::createIrrKlangDevice();
	}

	void Terminate()
	{
		rawEngine->removeAllSoundSources();
		rawEngine->drop();
	}

	const float GetVolume()
	{
		return rawEngine->getSoundVolume();
	}

	void SetVolume(const float value)
	{
		rawEngine->setSoundVolume(value);
	}

	void* GetRawEngine()
	{
		return rawEngine;
	}

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

	void Source::Play() const
	{
		SDEF;
		rawEngine->play2D(source);
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