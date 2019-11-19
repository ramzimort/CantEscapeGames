#pragma once

/**
 * @file Directory.h
 * @author Ramzi Mourtada
 * @date 12/4/2019
 * @brief Directory and File helper functions
 */

/**
 * @brief Directory Related Helper Functions
 * 
 */
namespace CantDirectory
{
	/**
	 * @brief Checks if path exists and is a file
	 *
	 * @param path
	 * @return true
	 * @return false
	 */
	bool FileExists(const std::string& path);
	
	/**
	 * @brief Checks if path exists and is a folder
	 *
	 * @param path
	 * @return true
	 * @return false
	 */
	bool FolderExists(const std::string& path);
	
	/**
	 * @brief Gets all files in a directory recursively.
	 * Can specify optional list of file extensions to filter
	 *
	 *
	 * @param dir_path
	 * @param file_extensions
	 * @return std::map<std::string, std::vector<std::string>> Map of parent folder to list of items
	 */
	std::map<std::string, std::vector<std::string>> GetAllObjectsRecursive(const std::string& dir_path, const std::vector<std::string>& file_extensions = {});

	/**
	 * @brief Gets all files in a directory recursively.
	 * Can specify optional list of file extensions to filter
	 *
	 * @param dir_path
	 * @param file_extensions
	 * @return std::vector<std::string> Returns all files
	 */
	std::vector<std::string> GetAllObjects(const std::string& dir_path, const std::vector<std::string>& file_extensions = { } );

	/**
	 * @brief Gets all files in a directory non-recursively.
	 * Can specify optional list of file extensions to filter
	 *
	 * @param dir_path
	 * @param file_extension
	 * @return std::vector<std::string>
	 */
	std::vector<std::string> GetObjectsIn(const std::string& dir_path, const std::string& file_extension);

};

