
namespace Audio {
	void Init();
	void Terminate();

	const float GetVolume();
	void SetVolume(const float value);
	void* GetRawEngine();

	class Source
	{
	public:
		Source() = delete;
		Source(const char* path, const float volume = 1.0f);
		~Source();

		void Play() const;
		void SetFile(const char* path);
	private:
		void* mRawSource;
	};
}