#pragma once

/**
 * @file Hash.h
 * @author Ramzi Mourtada
 * @date 12/4/2019
 * @brief 64 bit based string hashing functions for paths 
 */

#define MAGIC_PRIME ((uint64_t)0x100000001b3ULL)
class StringId;

/**
 * @brief 64 bit string FNV Hash function
 * 
 * @param str 
 * @param len 
 * @return constexpr uint64_t 
 */
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

/**
 * @brief Hashed version of string. (Contains both size_t and string in DEVELOPER)
 * 
 */
class StringId
{
	friend class StringIdHash;
public:
	StringId() :
		m_name(""),
		m_id(0)
	{	};
	StringId(const StringId& rhs) :
		m_name(rhs.m_name),
		m_id(rhs.m_id)
	{	}
	StringId(const char* _name) :
		m_name(std::string(_name)),
		m_id(FNVHash(std::string(_name).c_str(), std::string(_name).size())) { }
	StringId(const std::string& _name) :
		m_name(_name),
		m_id(FNVHash(_name.c_str(), _name.size())) { }

	StringId& operator=(const StringId& rhs)
	{
		m_id = rhs.m_id; 
		m_name = rhs.m_name;
		return *this;
	}
	bool operator==(const StringId& rhs) const { return m_id == rhs.m_id; }
	bool operator==(const size_t& rhs) const { return m_id == rhs; }
	bool operator<(const StringId& rhs) const { return m_id < rhs.m_id; }
	const std::string& getName() const{ return m_name; }

private:
	size_t m_id;
	std::string m_name;


	RTTR_ENABLE();
	RTTR_REGISTRATION_FRIEND;
};

/**
 * @brief Hashing Function
 * 
 */
class StringIdHash {
public:
	uint64_t operator()(const StringId& p) const
	{
		return p.m_id;
	}
};