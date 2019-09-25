#pragma once

namespace CantReflect
{
	rttr::variant GetVariant(const rttr::variant & member, const rapidjson::Value& prefab_value);
}