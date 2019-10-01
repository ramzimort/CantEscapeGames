

#include "Serialization.h"
#include "Helpers.h"

namespace CantReflect
{
	using namespace rapidjson;
	using namespace rttr;

	const std::string ToJson(rttr::instance obj)
	{
		if (!obj.is_valid())
			return std::string();

		StringBuffer sb;
		PrettyWriter<StringBuffer> writer(sb);

		WriteRecursive(obj, writer);

		return sb.GetString();
	}

	bool FromJson(const std::string& json, instance obj)
	{
		Document document;  // Default template parameter uses UTF8 and MemoryPoolAllocator.

		// "normal" parsing, decode strings to new buffers. Can use other input stream via ParseStream().
		if (document.Parse(json.c_str()).HasParseError())
			return 1;

		ReadRecursive(obj, document);

		return true;
	}

	const std::string StringifyJson(const std::string& filePath)
	{
		std::string path = (filePath);
		std::ifstream ifs(path);
		assert(ifs.is_open());

		rapidjson::Document lvlDoc;
		rapidjson::IStreamWrapper isw(ifs);

		assert(!lvlDoc.ParseStream(isw).HasParseError());
		assert(lvlDoc.IsObject());
		
		ifs.close();
		rapidjson::StringBuffer buffer;
		buffer.Clear();
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		lvlDoc.Accept(writer);

		const std::string jsonText = std::string(buffer.GetString());

		return jsonText;
	}
}