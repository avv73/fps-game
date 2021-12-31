#include "ShaderLibrary.h"
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;

ShaderLibrary::ShaderLibrary(const std::string& shaderFolder)
{
	if (LoadedShaders != NULL)
	{
		throw new std::logic_error("ShaderLibrary is singleton!");
	}

	shadersPath = shaderFolder;
	LoadedShaders = this;
}

bool ShaderLibrary::LoadShaders()
{
	std::vector<std::string> loadedShaders;

	for (const fs::path& entry : fs::directory_iterator(shadersPath))
	{
		std::string currentFilename = entry.stem().string();
		std::string currentPath = entry.parent_path().string();

		if (std::find(loadedShaders.begin(), loadedShaders.end(), entry.filename()) == loadedShaders.end())
		{
			// load shader if it is not contained
			loadedShaders.push_back(currentFilename);
			Shader* shdToLoad = new Shader();
			//shdToLoad->Load() // load shader here, add it to library; model & shader name (vertex, frag) should match
		}
	}
}