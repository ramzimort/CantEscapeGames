#pragma once

#include "..\CantDebug\CantDebug.h"

class MaterialMaker
{
public:
	MaterialMaker();
	~MaterialMaker();

	void Update();
	void UpdateInfo(CantDebugAPI::MaterialInfo info);

private:
	CantDebugAPI::MaterialInfo m_materialParams;
};