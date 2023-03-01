#pragma once
#include <string>
#include <filesystem>

struct AssetPath
{
	enum AssetType
	{
		OBJ,
		SCRIPT,
		TEXTURE
	};
	std::filesystem::path path;
	bool isTracked;


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
		}
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
		}
	}
};