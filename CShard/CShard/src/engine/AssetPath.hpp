#pragma once
#include <string>
#include <filesystem>
#include <fstream>


inline std::vector<std::string> strSplit(const std::string& str, char chr)
{
	std::stringstream test(str);
	std::string segment;
	std::vector<std::string> seglist;

	while(std::getline(test, segment, chr))
	{
		seglist.push_back(segment);
	}
	return seglist;
}

struct AssetPath
{
	enum AssetType
	{
		OBJ,
		SCRIPT,
		TEXTURE,
		ERR
	};
	std::filesystem::path path;
	bool isTracked = false;


	static std::string getDefaultPath(AssetType def)
	{
		switch (def)
		{
		case OBJ:
			return "pak/resources/obj/";
		case SCRIPT:
			return "pak/resources/scripts/";
		case TEXTURE:
			return "pak/resources/tex/";
		case ERR:;
		}
		return "";
	}

	static AssetPath getPath(AssetPath::AssetType home, const std::string& source)
	{
		AssetPath ret;
		ret.isTracked = false;
		std::filesystem::path defPath = getDefaultPath(home);

		std::filesystem::path path(source);
		path.make_preferred();
		ret.path = path;
		auto desiredPath = std::filesystem::current_path();
		if (path.is_relative())
		{
			std::ifstream test(source);
			if (!test)
			{
				ret.path = defPath / std::filesystem::path(source);
				ret.path.make_preferred();
			}
		}
		else
		{
			if (source.rfind(desiredPath.string(), 0) == 0)
			{
				ret.path = std::filesystem::relative(ret.path, std::filesystem::current_path());
			}
		}
		auto pakPath = desiredPath / "pak";
		if (!pakPath.lexically_relative(std::filesystem::absolute(ret.path)).empty())
		{
			ret.isTracked = true;
		}
		return ret;
	}
};