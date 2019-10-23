#pragma once

struct MaterialControls
{

};

struct MaterialInfo
{

};

class MaterialMaker
{
	typedef std::unordered_map<std::string, std::vector<MaterialInfo>> MaterialList;
public:
	MaterialMaker();
	~MaterialMaker();

	void Update();

private:
	MaterialList m_materialParams;
};