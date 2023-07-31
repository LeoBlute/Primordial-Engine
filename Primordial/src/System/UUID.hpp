#pragma once

struct UUID
{
public:
	UUID();
	UUID(const UUID&) = default;
	UUID(unsigned long long rawID)
		: mRawUUID(rawID) {};
	~UUID() = default;
	constexpr inline unsigned long long GetRawUUID() const { return mRawUUID; };
	bool operator==(const UUID& other) const = default;

private:
	unsigned long long mRawUUID;
};

namespace std {
	template <typename T> struct hash;

	template<>
	struct hash<UUID>
	{
		size_t operator()(const UUID& uuid) const
		{
			return uuid.GetRawUUID();
		}
	};

}