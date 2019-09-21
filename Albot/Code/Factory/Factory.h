/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/

#pragma once

class GameObjectManager;
class SystemManager;


class Factory
{

public:
	Factory(std::string path, GameObjectManager *goMgr, SystemManager *sysMgr);
	~Factory();

private:
	Factory(Factory const& rhs);

};