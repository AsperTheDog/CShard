#pragma once
#include <string>
#include <filesystem>
#include <fstream>

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
			return "pak/resources/textures/";
		case ERR:;
		}
		return "";
	}

	static std::string getDefaultExt(AssetType def)
	{
		switch (def)
		{
		case OBJ:
			return ".obj";
		case SCRIPT:
			return ".lua";
		case TEXTURE:
			return ".png";
		case ERR:;
		}
		return "";
	}

	static AssetPath getPath(AssetPath::AssetType home, const std::string& source)
	{
		AssetPath ret;
		ret.isTracked = false;
		std::filesystem::path defPath = getDefaultPath(home);
		auto defExt = getDefaultExt(home);

		std::filesystem::path path(source);
		path.make_preferred();
		ret.path = path;
		auto desiredPath = std::filesystem::current_path();
		if (path.is_relative())
		{
			std::ifstream test(source);
			if (!test)
			{
				std::string name = source;
				if (source.size() < defExt.size() || 0 != source.compare(source.length() - defExt.size(), defExt.size(), defExt))
				{
					name = source + defExt;
				}
				ret.path = defPath / std::filesystem::path(name);
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