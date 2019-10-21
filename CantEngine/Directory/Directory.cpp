/* Start Header------------------------------------------------------ -
Copyright(C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author : Ramzi Mourtada
Other Authors : <None>
-End Header--------------------------------------------------------*/

#include "Directory.h"


namespace CantDirectory
{
	namespace fs = std::experimental::filesystem;

	bool FileExists(const std::string& path)
	{
		const char* pathname = path.c_str();
		struct stat info;
		if (stat(pathname, &info) == 0 && S_IFREG)
			return true;
		return false;
	}

	bool FolderExists(const std::string& path)
	{
		const char* pathname = path.c_str();
		struct stat info;
		if (stat(pathname, &info) == 0 && S_IFDIR)
			return true;
		return false;
	}

	std::map<std::string, std::vector<std::string>> GetAllObjectsRecursive(const std::string& dir_path, const std::vector<std::string>& file_extensions)
	{
		std::map<std::string, std::vector<std::string>> Objects;
		fs::path FilePath;
		for (auto& dir_iterator : std::experimental::filesystem::recursive_directory_iterator(dir_path))
		{
			FilePath = dir_iterator;
			if (file_extensions.empty() || std::find(file_extensions.begin(), file_extensions.end(), FilePath.extension()) != file_extensions.end())
			{
				std::string path = FilePath.parent_path().string();
				std::string type = path.substr(fs::path(dir_path).string().size());
				std::string name = FilePath.stem().string();
				Objects[type].push_back(name);
			}
		}
		return Objects;
	}

	std::vector<std::string> GetAllObjects(const std::string& dir_path, const std::vector<std::string>& file_extensions)
	{
		std::vector<std::string> Objects;
		fs::path FilePath;
		for (auto& dir_iterator : std::experimental::filesystem::recursive_directory_iterator(dir_path))
		{
			FilePath = dir_iterator;

			if (file_extensions.empty() || std::find(file_extensions.begin(), file_extensions.end(), FilePath.extension()) != file_extensions.end())
			{
				if (!FilePath.has_extension())
					continue;
				std::string full_path = FilePath.string();
				std::string path = full_path.substr(fs::path(dir_path).string().size());
				Objects.push_back(path);
			}
		}
		return Objects;
	}

	std::vector<std::string> GetObjectsIn(const std::string& dir_path, const std::string& file_extension)
	{
		std::vector<std::string> Objects;
		fs::path FilePath;
		for (auto& dir_iterator : std::experimental::filesystem::directory_iterator(dir_path))
		{
			FilePath = dir_iterator;
			if (FilePath.extension() == file_extension)
			{
				std::string name = FilePath.stem().string();
				Objects.push_back(name);
			}
		}
		return Objects;
	}
}