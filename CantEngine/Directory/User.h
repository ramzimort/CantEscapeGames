#pragma once

/**
 * @file User.h
 * @author Ramzi Mourtada
 * @date 12/4/2019
 * @brief User Directory and File helper functions
 */

namespace CantDirectory
{
	/**
	 * @brief Returns path to user directory in My Documents
	 * 
	 * @return const std::string 
	 */
	const std::string Path();
	/**
	 * @brief Initializes the user data folder for any required storage
	 * 
	 * @param name 
	 */
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