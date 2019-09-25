
/* Start Header------------------------------------------------------ -
Copyright(C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author : Ramzi Mourtada
Other Authors : <None>
-End Header--------------------------------------------------------*/

#include "Helpers.h"
namespace CantReflect
{
	rttr::variant GetVariant(const rttr::variant &member, const rapidjson::Value& prefab_value)
	{
		rttr::type member_type = member.get_type();
		std::string member_name = member_type.get_name().to_string();

		if (member_type == rttr::type::get<float>())
		{
			float prefab_val = prefab_value.GetFloat();
			return rttr::variant(prefab_val);
		}
		else if (member_type == rttr::type::get<double>())
		{
			double prefab_val = prefab_value.GetDouble();
			return rttr::variant(prefab_val);
		}
		else if (member_type == rttr::type::get<int>())
		{
			int prefab_val = prefab_value.GetInt();
			return rttr::variant(prefab_val);
		}
		else if (member_type == rttr::type::get<bool>())
		{
			bool prefab_val = prefab_value.GetBool();
			return rttr::variant(prefab_val);
		}
		else if (member_type == rttr::type::get<std::string>())
		{
			std::string prefab_val = prefab_value.GetString();
			return rttr::variant(prefab_val);
		}
		else if (member_type == rttr::type::get<Vector2>())
		{
			Vector2 prefab_val;
			prefab_val.x = static_cast<float>(prefab_value.GetArray()[0].GetDouble());
			prefab_val.y = static_cast<float>(prefab_value.GetArray()[1].GetDouble());
			return rttr::variant(prefab_val);
		}
		else if (member_type == rttr::type::get<Vector3>())
		{
			Vector3 prefab_val;
			prefab_val.x = static_cast<float>(prefab_value.GetArray()[0].GetDouble());
			prefab_val.y = static_cast<float>(prefab_value.GetArray()[1].GetDouble());
			prefab_val.z = static_cast<float>(prefab_value.GetArray()[2].GetDouble());
			return rttr::variant(prefab_val);

		}
		else if (member_type == rttr::type::get<Vector4>())
		{
			Vector4 prefab_val;
			prefab_val.x = static_cast<float>(prefab_value.GetArray()[0].GetDouble());
			prefab_val.y = static_cast<float>(prefab_value.GetArray()[1].GetDouble());
			prefab_val.z = static_cast<float>(prefab_value.GetArray()[2].GetDouble());
			prefab_val.w = static_cast<float>(prefab_value.GetArray()[3].GetDouble());

			return rttr::variant(prefab_val);
		}
		else if (member_type.is_enumeration())
		{
			std::string prefab_val = prefab_value.GetString();
			return member_type.get_enumeration().name_to_value(prefab_val);
		}
	}


}
