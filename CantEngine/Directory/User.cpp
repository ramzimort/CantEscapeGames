
#include "User.h"
#include "Directory.h"
#include "Reflection/Serialization.h"

using namespace rapidjson;
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

UserManager::UserManager(const std::string& gameName) :
	m_userDirPath(DocumentsPath() + gameName + "\\"),
	m_userSettingsPath(m_userDirPath + "Settings")
{
	if (CantDirectory::FolderExists(m_userDirPath))
		return;

	_mkdir(m_userDirPath.c_str());

	if (!CantDirectory::FileExists(m_userSettingsPath))
	{
		std::ofstream ofs(m_userSettingsPath, std::ofstream::out);
		assert(ofs.is_open());
		StringBuffer sb;
		PrettyWriter<StringBuffer> writer(sb);

		writer.StartObject();
		writer.EndObject();
		ofs << sb.GetString();
		ofs.close();
		sb.Clear();
	}
	const std::string settings = CantReflect::StringifyJson(m_userSettingsPath);
	m_settings.Parse(settings);
	if (m_settings.HasParseError())
	{
		DEBUG_LOG("ERROR PARSING SETTINGS FILE");
	}
}

void UserManager::SaveSettingsToDisk() const
{
	StringBuffer sb;
	PrettyWriter<StringBuffer> writer(sb);
	m_settings.Accept(writer);
	std::ofstream ofs(m_userSettingsPath, std::ofstream::out);
	ofs << sb.GetString();
	ofs.close();
}

bool UserManager::HasSetting(const std::string& parent, const std::string& setting) const
{
	const auto& obj = m_settings.GetObjectA();
	if (!obj.HasMember(parent))
		return false;
	if (!obj[parent].HasMember(setting))
		return false;
	return true;
}

const bool UserManager::GetSettingBool(const std::string& parent, const std::string& setting)
{
	if (HasSetting(parent, setting))
		return m_settings.GetObjectA()[parent][setting].GetBool();
	return false;
}

const int UserManager::GetSettingInt(const std::string& parent, const std::string& setting)
{
	if (HasSetting(parent, setting))
		return m_settings.GetObjectA()[parent][setting].GetInt();
	return false;
}

const double UserManager::GetSettingDouble(const std::string& parent, const std::string& setting)
{
	if (HasSetting(parent, setting))
		return m_settings.GetObjectA()[parent][setting].GetDouble();
	return 0.0;
}

const std::string UserManager::GetSettingString(const std::string& parent, const std::string& setting)
{
	if (HasSetting(parent, setting))
		return m_settings.GetObjectA()[parent][setting].GetString();
	return "";
}

void UserManager::SetSettingBool(const std::string& parent, const std::string& setting, const bool value)
{
	StringBuffer sb;
	PrettyWriter<StringBuffer> w;
	rapidjson::Document::AllocatorType& allocator = m_settings.GetAllocator();
	auto obj = m_settings.GetObjectA();
	if (!obj.HasMember(parent))
	{
		Value parentKey(parent, allocator); // Key
		Value object(rapidjson::kObjectType); // Value
		m_settings.AddMember(parentKey, object, allocator);
	}
	if (!obj[parent].HasMember(setting))
	{
		Value settingKey(setting, allocator); // Key
		Value object(false, allocator); // Value
		obj[parent].AddMember(settingKey, false, allocator);
	}
	obj[parent][setting].SetBool(value);
}

void UserManager::SetSettingInt(const std::string& parent, const std::string& setting, const int& value)
{
	StringBuffer sb;
	PrettyWriter<StringBuffer> w;
	rapidjson::Document::AllocatorType& allocator = m_settings.GetAllocator();
	auto obj = m_settings.GetObjectA();
	if (!obj.HasMember(parent))
	{
		Value parentKey(parent, allocator); // Key
		Value object(rapidjson::kObjectType); // Value
		m_settings.AddMember(parentKey, object, allocator);
	}
	if (!obj[parent].HasMember(setting))
	{
		Value settingKey(setting, allocator); // Key
		Value object(false, allocator); // Value
		obj[parent].AddMember(settingKey, 0, allocator);
	}
	obj[parent][setting].SetInt(value);
}

void UserManager::SetSettingDouble(const std::string& parent, const std::string& setting, const double& value)
{
	StringBuffer sb;
	PrettyWriter<StringBuffer> w;
	rapidjson::Document::AllocatorType& allocator = m_settings.GetAllocator();
	auto obj = m_settings.GetObjectA();
	if (!obj.HasMember(parent))
	{
		Value parentKey(parent, allocator); // Key
		Value object(rapidjson::kObjectType); // Value
		m_settings.AddMember(parentKey, object, allocator);
	}
	if (!obj[parent].HasMember(setting))
	{
		Value settingKey(setting, allocator); // Key
		Value object(false, allocator); // Value
		obj[parent].AddMember(settingKey, 0.0, allocator);
	}
	obj[parent][setting].SetDouble(value);
}

void UserManager::SetSettingString(const std::string& parent, const std::string& setting, const std::string& value)
{
	StringBuffer sb;
	PrettyWriter<StringBuffer> w;
	rapidjson::Document::AllocatorType& allocator = m_settings.GetAllocator();
	auto obj = m_settings.GetObjectA();
	if (!obj.HasMember(parent))
	{
		Value parentKey(parent, allocator); // Key
		Value object(rapidjson::kObjectType); // Value
		m_settings.AddMember(parentKey, object, allocator);
	}
	if (!obj[parent].HasMember(setting))
	{
		Value settingKey(setting, allocator); // Key
		Value object(false, allocator); // Value
		obj[parent].AddMember(settingKey, "", allocator);
	}
	obj[parent][setting].SetString(value, allocator);
}