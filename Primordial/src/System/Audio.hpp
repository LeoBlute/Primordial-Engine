
namespace Audio {
	void Init();
	void Terminate();

	const float GetVolume();
	void PlayFromFile(const char* path);
	void SetVolume(const float value);
	void* GetRawEngine();

	class Source
	{
	private:
		friend void PlayFromSource(Audio::Source* source);
	public:
		Source() = delete;
		Source(const char* path, const float volume = 1.0f);
		~Source();
		void SetFile(const char* path);
		constexpr inline void* GetRaw() const { return mRawSource; };
	private:
		void* mRawSource;
	};

	//Currently not implemented
	class Sound {
	};

	void PlayFromSource(Audio::Source* source);
}