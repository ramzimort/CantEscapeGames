#pragma once


namespace CantReflect
{
	std::string ToJson(rttr::instance instance);
	bool FromJson(const std::string& json, rttr::instance obj);
}