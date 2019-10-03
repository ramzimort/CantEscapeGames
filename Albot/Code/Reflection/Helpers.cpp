
/* Start Header------------------------------------------------------ -
Copyright(C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author : Ramzi Mourtada
Other Authors : <None>
-End Header--------------------------------------------------------*/

#include "Helpers.h"
#include "CantDebug/CantDebug.h"
#include "Helper/Hash.h"

namespace CantReflect
{
	using namespace rttr;
	using namespace rapidjson;

	bool WriteBaseType(const type& t, const variant& var, PrettyWriter<StringBuffer>& writer)
	{
		if (t.is_arithmetic())
		{
			if (t == type::get<bool>())
				writer.Bool(var.to_bool());
			else if (t == type::get<char>())
				writer.Bool(var.to_bool());
			else if (t == type::get<int8_t>())
				writer.Int(var.to_int8());
			else if (t == type::get<int16_t>())
				writer.Int(var.to_int16());
			else if (t == type::get<int32_t>())
				writer.Int(var.to_int32());
			else if (t == type::get<int64_t>())
				writer.Int64(var.to_int64());
			else if (t == type::get<uint8_t>())
				writer.Uint(var.to_uint8());
			else if (t == type::get<uint16_t>())
				writer.Uint(var.to_uint16());
			else if (t == type::get<uint32_t>())
				writer.Uint(var.to_uint32());
			else if (t == type::get<uint64_t>())
				writer.Uint64(var.to_uint64());
			else if (t == type::get<float>())
				writer.Double(var.to_double());
			else if (t == type::get<double>())
				writer.Double(var.to_double());
			return true;
		}
		else if (t.is_enumeration())
		{
			bool ok = false;
			auto result = var.to_string(&ok);
			if (ok)
			{
				writer.String(var.to_string());
			}
			else
			{
				ok = false;
				auto value = var.to_uint64(&ok);
				if (ok)
					writer.Uint64(value);
				else
					writer.Null();
			}

			return true;
		}
		else if (t == type::get<std::string>())
		{
			writer.String(var.to_string());
			return true;
		}

		else if (t == type::get<Vector2>())
		{
			Vector2 val = var.get_value<Vector2>();
			writer.StartArray();
			writer.Double(val.x);
			writer.Double(val.y);
			writer.EndArray();
			return true;
		}
		else if (t == type::get<Vector3>())
		{
			Vector3 val = var.get_value<Vector3>();
			writer.StartArray();
			writer.Double(val.x);
			writer.Double(val.y);
			writer.Double(val.z);
			writer.EndArray();
			return true;
		}
		else if (t == type::get<Vector4>())
		{

			Vector4 val = var.get_value<Vector4>();
			writer.StartArray();
			writer.Double(val.x);
			writer.Double(val.y);
			writer.Double(val.z);
			writer.Double(val.w);
			writer.EndArray();
			return true;
		}
		return false;
	}

	void WriteArray(const variant_sequential_view& view, PrettyWriter<StringBuffer>& writer)
	{
		writer.StartArray();
		for (const auto& item : view)
		{
			if (item.is_sequential_container())
			{
				WriteArray(item.create_sequential_view(), writer);
			}
			else
			{
				variant wrapped_var = item.extract_wrapped_value();
				type value_type = wrapped_var.get_type();
				if (value_type.is_arithmetic() || value_type == type::get<std::string>() || value_type.is_enumeration())
				{
					WriteBaseType(value_type, wrapped_var, writer);
				}
				else // object
				{
					WriteRecursive(wrapped_var, writer);
				}
			}
		}
		writer.EndArray();
	}

	void WriteMap(const variant_associative_view& view, PrettyWriter<StringBuffer>& writer)
	{
		static const string_view key_name("key");
		static const string_view value_name("value");

		writer.StartArray();

		if (view.is_key_only_type())
		{
			for (auto& item : view)
			{
				WriteVariant(item.first, writer);
			}
		}
		else
		{
			for (auto& item : view)
			{
				writer.StartObject();
				writer.String(key_name.data(), static_cast<rapidjson::SizeType>(key_name.length()), false);

				WriteVariant(item.first, writer);

				writer.String(value_name.data(), static_cast<rapidjson::SizeType>(value_name.length()), false);

				WriteVariant(item.second, writer);

				writer.EndObject();
			}
		}

		writer.EndArray();
	}

	void WriteRecursive(const instance& obj2, PrettyWriter<StringBuffer>& writer)
	{
		writer.StartObject();
		instance obj = obj2.get_type().get_raw_type().is_wrapper() ? obj2.get_wrapped_instance() : obj2;

		auto prop_list = obj.get_type().get_properties();
		for (auto prop : prop_list)
		{
			if (prop.get_metadata("NO_SERIALIZE"))
				continue;

			variant prop_value = prop.get_value(obj);
			if (!prop_value)
				continue; // cannot serialize, because we cannot retrieve the value

			const auto name = prop.get_name();
			writer.String(name.data(), static_cast<rapidjson::SizeType>(name.length()), false);
			if (!WriteVariant(prop_value, writer))
			{
				DEBUG_LOG("CANT SERIALIZE OBJECT %s", obj.get_type().get_name());
			}
		}

		writer.EndObject();
	}

	bool WriteVariant(const variant& var, PrettyWriter<StringBuffer>& writer)
	{
		auto value_type = var.get_type();
		auto wrapped_type = value_type.is_wrapper() ? value_type.get_wrapped_type() : value_type;
		bool is_wrapper = wrapped_type != value_type;

		if (WriteBaseType(is_wrapper ? wrapped_type : value_type,
			is_wrapper ? var.extract_wrapped_value() : var, writer))
		{
		}
		else if (var.is_sequential_container())
		{
			WriteArray(var.create_sequential_view(), writer);
		}
		else if (var.is_associative_container())
		{
			WriteMap(var.create_associative_view(), writer);
		}
		else
		{
			auto child_props = is_wrapper ? wrapped_type.get_properties() : value_type.get_properties();
			if (!child_props.empty())
			{
				WriteRecursive(var, writer);
			}
			else
			{
				bool ok = false;
				auto text = var.to_string(&ok);
				if (!ok)
				{
					writer.String(text);
					return false;
				}

				writer.String(text);
			}
		}

		return true;
	}

	variant ReadBaseType(Value& json_value, const type& var_t)
	{
		switch (json_value.GetType())
		{
		case kStringType:
		{
			// Automatically convert StringId	
			if (var_t == type::get<StringId>())
			{
				const char* name = json_value.GetString();
				StringId id = StringId(name);
				variant id_var = variant(id);
				return id_var;
			}

			return std::string(json_value.GetString());
			break;
		}
		case kNullType:     break;
		case kFalseType:
		case kTrueType:
		{
			return json_value.GetBool();
			break;
		}
		case kNumberType:
		{
			if (json_value.IsInt())
				return json_value.GetInt();
			else if (json_value.IsDouble())
				return json_value.GetDouble();
			else if (json_value.IsUint())
				return json_value.GetUint();
			else if (json_value.IsInt64())
				return json_value.GetInt64();
			else if (json_value.IsUint64())
				return json_value.GetUint64();
			break;
		}
		// we handle only the basic types here
		case kObjectType:
		case kArrayType: return variant();
		}

		return variant();
	}

	void ReadArray(variant_sequential_view& view, Value& json_array_value)
	{
		view.set_size(json_array_value.Size());
		const type array_value_type = view.get_rank_type(1);

		for (SizeType i = 0; i < json_array_value.Size(); ++i)
		{
			auto& json_index_value = json_array_value[i];
			if (json_index_value.IsArray())
			{
				auto sub_array_view = view.get_value(i).create_sequential_view();
				ReadArray(sub_array_view, json_index_value);
			}
			else if (json_index_value.IsObject())
			{
				variant var_tmp = view.get_value(i);
				variant wrapped_var = var_tmp.extract_wrapped_value();
				ReadRecursive(wrapped_var, json_index_value);
				view.set_value(i, wrapped_var);
			}
			else
			{
				variant extracted_value = ReadBaseType(json_index_value, array_value_type);
				if (extracted_value.convert(array_value_type))
					view.set_value(i, extracted_value);
			}
		}
	}

	variant ReadVariant(Value::MemberIterator& itr, const type& t, const std::vector<argument>& args)
	{
		auto& json_value = itr->value;
		variant extracted_value = ReadBaseType(json_value, t);
		const bool could_convert = extracted_value.convert(t);
		if (!could_convert)
		{
			if (json_value.IsObject())
			{
				std::vector<rttr::type> argType;
				for (auto arg : args)
					argType.push_back(arg.get_type());
				constructor ctor = t.get_constructor(argType);
				for (auto& item : t.get_constructors())
				{
					if (item.get_instantiated_type() == t)
						ctor = item;
				}
				switch (args.size())
				{
				case 1:
					extracted_value = ctor.invoke(args[0]);
					break;
				case 2:
					extracted_value = ctor.invoke(args[0], args[1]);
					break;
				case 3:
					extracted_value = ctor.invoke(args[0], args[1], args[2]);
					break;
				case 4:
					extracted_value = ctor.invoke(args[0], args[1], args[2], args[3]);
					break;
				case 5:
					extracted_value = ctor.invoke(args[0], args[1], args[2], args[3], args[4]);
					break;
				case 6:
					extracted_value = ctor.invoke(args[0], args[1], args[2], args[3], args[4], args[5]);
				default:
					extracted_value = ctor.invoke();
					break;
				}
				ReadRecursive(extracted_value, json_value);
			}
		}
		return extracted_value;
	};

	void ReadMap(variant_associative_view& view, Value& json_array_value)
	{
		for (SizeType i = 0; i < json_array_value.Size(); ++i)
		{
			auto& json_index_value = json_array_value[i];
			if (json_index_value.IsObject()) // a key-value associative view
			{
				Value::MemberIterator key_itr = json_index_value.FindMember("key");
				Value::MemberIterator value_itr = json_index_value.FindMember("value");

				if (key_itr != json_index_value.MemberEnd() &&
					value_itr != json_index_value.MemberEnd())
				{
					auto key_var = ReadVariant(key_itr, view.get_key_type(), std::vector<rttr::argument>());
					auto value_var = ReadVariant(value_itr, view.get_value_type(), std::vector<rttr::argument>());
					if (key_var && value_var)
					{
						view.insert(key_var, value_var);
					}
				}
			}
			else // a key-only associative view
			{
				variant extracted_value = ReadBaseType(json_index_value, view.get_key_type());
				if (extracted_value && extracted_value.convert(view.get_key_type()))
					view.insert(extracted_value);
			}
		}
	}

	void ReadRecursive(instance obj2, Value& json_object)
	{
		instance obj = obj2.get_type().get_raw_type().is_wrapper() ? obj2.get_wrapped_instance() : obj2;
		const auto prop_list = obj.get_derived_type().get_properties();

		for (auto prop : prop_list)
		{
			Value::MemberIterator ret = json_object.FindMember(prop.get_name().data());
			if (ret == json_object.MemberEnd())
				continue;
			const type value_t = prop.get_type();

			auto& json_value = ret->value;
			switch (json_value.GetType())
			{
			case kArrayType:
			{
				variant var;
				if (value_t == type::get<Vector2>())
				{
					Vector2 vec2;
					vec2.x = json_value.GetArray()[0].GetFloat();
					vec2.y = json_value.GetArray()[1].GetFloat();
					var = vec2;
				}
				else if (value_t == type::get<Vector3>())
				{
					Vector3 vec3;
					vec3.x = json_value.GetArray()[0].GetFloat();
					vec3.y = json_value.GetArray()[1].GetFloat();
					vec3.z = json_value.GetArray()[2].GetFloat();
					var = vec3;
				}
				else if (value_t == type::get<Vector4>())
				{
					Vector4 vec4;
					vec4.x = json_value.GetArray()[0].GetFloat();
					vec4.y = json_value.GetArray()[1].GetFloat();					
					vec4.z = json_value.GetArray()[2].GetFloat();
					vec4.w = json_value.GetArray()[3].GetFloat();
					var = vec4;
				}
				else if (value_t.is_sequential_container())
				{
					var = prop.get_value(obj);
					auto view = var.create_sequential_view();
					ReadArray(view, json_value);
				}
				else if (value_t.is_associative_container())
				{
					var = prop.get_value(obj);
					auto associative_view = var.create_associative_view();
					ReadMap(associative_view, json_value);
				}

				prop.set_value(obj, var);
				break;
			}
			case kObjectType:
			{
				variant var = prop.get_value(obj);
				ReadRecursive(var, json_value);
				prop.set_value(obj, var);
				break;
			}
			default:
			{
				variant extracted_value = ReadBaseType(json_value, value_t);
				if (extracted_value.convert(value_t)) // REMARK: CONVERSION WORKS ONLY WITH "const type", check whether this is correct or not!
					prop.set_value(obj, extracted_value);
			}
			}
		}
	}
}