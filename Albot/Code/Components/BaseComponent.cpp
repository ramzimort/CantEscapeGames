/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/

#include "BaseComponent.h"

unsigned int BaseComponent::numberOfTypes = 0;

RTTR_REGISTRATION
{
	rttr::registration::class_<BaseComponent>("BaseComponent");
}