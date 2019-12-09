#pragma once
class CheckboxQueue
{
public:
	struct CheckboxData
	{
		std::string m_checkBoxName;
		bool* m_pFlag;
	};
public:
	CheckboxQueue();
	~CheckboxQueue();


	std::queue<CheckboxData> m_queueData;
};

