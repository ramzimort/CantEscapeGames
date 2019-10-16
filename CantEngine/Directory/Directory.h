#pragma once

namespace CantDirectory
{
	bool FileExists(const std::string& path);

	bool FolderExists(const std::string& path);

	std::map<std::string, std::vector<std::string>> GetAllObjects(const std::string& dir_path, const std::string& file_extension);

	std::vector<std::string> GetAllObjectsIn(const std::string& dir_path);

	std::vector<std::string> GetObjectsIn(const std::string& dir_path, const std::string& file_extension);

};

