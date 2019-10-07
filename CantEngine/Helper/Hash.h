#pragma once

#define MAGIC_PRIME ((uint64_t)0x100000001b3ULL)
class StringId;

inline constexpr uint64_t FNVHash(const char* str, size_t len)
{
	size_t hash = 0;
	size_t i = 0;

	for (i = 0; i < len; i++)
	{
		hash *= MAGIC_PRIME;
		hash ^= (str[i]);
	}

	return hash;
}

inline constexpr size_t operator "" _sid(const char* name, size_t len)
{
	return FNVHash(name, len);
}

class StringId
{
	friend class StringIdHash;
public:
	StringId() :
#ifdef DEVELOPER
		m_name(""),
#endif
		m_id(0)
	{	};
	StringId(const StringId& rhs) :
#ifdef DEVELOPER
		m_name(rhs.m_name),
#endif
		m_id(rhs.m_id)
	{	}
	StringId(const char* _name) :
#ifdef DEVELOPER
		m_name(std::string(_name)),
#endif
		m_id(FNVHash(std::string(_name).c_str(), std::string(_name).size())) { }
	StringId(const std::string& _name) :
#ifdef DEVELOPER
		m_name(_name),
#endif
		m_id(FNVHash(_name.c_str(), _name.size())) { }

	StringId& operator=(const StringId& rhs)
	{
		m_id = rhs.m_id; 
#ifdef DEVELOPER
		m_name = rhs.m_name;
#endif
		return *this;
	}
	bool operator==(const StringId& rhs) const { return m_id == rhs.m_id; }
	bool operator<(const StringId& rhs) const { return m_id < rhs.m_id; }

private:
	size_t m_id;
#ifdef DEVELOPER
	std::string m_name;
#endif

	RTTR_ENABLE();
	RTTR_REGISTRATION_FRIEND;
};

class StringIdHash {
public:
	uint64_t operator()(const StringId& p) const
	{
		return p.m_id;
	}
};