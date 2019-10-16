#pragma once

namespace CantDirectory
{
	const std::string Path();
	void FindOrCreateUserDataFolder(const std::string& name);

	//class UserDirectory
	//{
	//public:
	//	UserDirectory(const std::string& dirName);
	//	~UserDirectory() = default;
	//	static const std::string& Path();
	//	static void FindOrCreateUserDataFolder(const std::string& name);

	//private:
	//	static std::string m_userDirName;
	//	UserDirectory() = delete;
	//	UserDirectory(const UserDirectory& rhs) = delete;
	//};
}