#pragma once

class UserManager
{
public:
	UserManager() = delete;
	UserManager(const std::string& gameName);
	~UserManager() { SaveSettingsToDisk(); }
	const std::string& UserDirectoryPath() const { return m_userDirPath; }
	const std::string& SettingsPath() const { return m_userSettingsPath; }
	void SaveSettingsToDisk() const;

	bool HasSetting(const std::string& parent, const std::string& setting) const;

	const bool GetSettingBool			(const std::string& parent, const std::string& setting);
	const int GetSettingInt				(const std::string& parent, const std::string& setting);
	const double GetSettingDouble		(const std::string& parent, const std::string& setting);
	const std::string GetSettingString	(const std::string& parent, const std::string& setting);

	void SetSettingBool					(const std::string& parent, const std::string& setting, const bool value);
	void SetSettingInt					(const std::string& parent, const std::string& setting, const int& value);
	void SetSettingDouble				(const std::string& parent, const std::string& setting, const double& value);
	void SetSettingString				(const std::string& parent, const std::string& setting, const std::string& value);

private:
	const std::string m_userDirPath;
	const std::string m_userSettingsPath;
	rapidjson::Document m_settings;
};