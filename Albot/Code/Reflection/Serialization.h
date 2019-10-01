#pragma once


namespace CantReflect
{
	const std::string ToJson(rttr::instance instance);
	bool FromJson(const std::string& json, rttr::instance obj);
	const std::string StringifyJson(const std::string& filePath);
}