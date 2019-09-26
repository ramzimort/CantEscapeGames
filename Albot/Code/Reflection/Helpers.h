#pragma once

namespace CantReflect
{
	// Write Helpers
	bool WriteBaseType(const rttr::type& t, const rttr::variant& var, rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
	void WriteArray(const rttr::variant_sequential_view& view, rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
	void WriteMap(const rttr::variant_associative_view& view, rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
	void WriteRecursive(const rttr::instance& obj2, rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
	bool WriteVariant(const rttr::variant& var, rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);

	// Read Helpers
	rttr::variant ReadBaseType(rapidjson::Value& json_value);
	void ReadArray(rttr::variant_sequential_view& view, rapidjson::Value& json_array_value);
	void ReadMap(rttr::variant_associative_view& view, rapidjson::Value& json_array_value);
	rttr::variant ReadVariant(rapidjson::Value::MemberIterator& itr, const rttr::type& t);
	void ReadRecursive(rttr::instance obj2, rapidjson::Value& json_object);
}