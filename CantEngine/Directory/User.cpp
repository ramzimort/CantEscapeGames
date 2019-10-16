
#include "User.h"
#include "Directory.h"

const std::string& DocumentsPath();

static std::string UserDirName = "";

namespace CantDirectory
{
	void FindOrCreateUserDataFolder(const std::string& name)
	{
		UserDirName = DocumentsPath() + name + "\\";
		if (FolderExists(UserDirName))
			return;

		_mkdir(UserDirName.c_str());

		//std::string user_settings = GetUserSettings();
		//if (FileExists(user_settings) == false)
		//{
		//	std::ofstream ofs(user_settings, std::ofstream::out);
		//	assert(ofs.is_open());
		//	ofs.close();
		//	//Create_Default_User_Window_Settings();
		//	//Create_Default_User_Audio_Settings();
		//	//Create_Default_User_Control_Bindings();
		//}
	}

	const std::string Path()
	{
		return UserDirName;
	}	
}

// Helpers

const std::string& DocumentsPath()
{
	static std::string path("");
	if (path == "")
	{
		wchar_t Folder[1024];
		HRESULT hr = SHGetFolderPathW(0, CSIDL_MYDOCUMENTS, 0, 0, Folder);
		assert(SUCCEEDED(hr));
		char str[1024];
		wcstombs(str, Folder, 1023);
		path = std::string(str) + "\\";
	}

	return path;
}