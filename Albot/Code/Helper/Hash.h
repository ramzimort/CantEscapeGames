#pragma once

#define MAGIC_PRIME ((uint64_t)0x100000001b3ULL)
class StringId;

inline constexpr uint64_t FNVHash(const char* str, uint64_t len)
{
	uint64_t hash = 0;
	uint64_t i = 0;

	for (i = 0; i < len; i++)
	{
		hash *= MAGIC_PRIME;
		hash ^= (str[i]);
	}

	return hash;
}

inline constexpr uint64_t operator "" _sid(const char* name, size_t len)
{
	return FNVHash(name, len);
}

class StringId
{
	friend class StringIdHash;
public:
	StringId() = delete;
	StringId(const StringId& rhs) :
#ifndef DEVELOPER
		m_name(rhs.m_name),
#endif
		m_id(rhs.m_id)
	{	}
	StringId(const char* _name, size_t len) :
#ifndef DEVELOPER
		m_name(_name),
#endif
		m_id(FNVHash(_name, len)) { }
	StringId(const std::string& _name) :
#ifndef DEVELOPER
		m_name(_name),
#endif
		m_id(FNVHash(_name.c_str(), _name.size())) { }
	const StringId& operator=(const StringId& rhs)
	{
		return StringId(rhs);
	}

	bool operator==(const StringId& rhs) const { return m_id == rhs.m_id; }
	bool operator<(const StringId& rhs) const { return m_id < rhs.m_id; }

private:
#ifndef DEVELOPER
const std::string& m_name;
#endif
	uint64_t m_id;
};

class StringIdHash {
public:
	uint64_t operator()(const StringId& p) const
	{
		return p.m_id;
}
};
