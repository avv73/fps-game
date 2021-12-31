#pragma once
#ifndef SHADERLIBRARY_H

#include <iostream>
#include <string>
#include <vector>
#include "Shader.h"

class ShaderLibrary
{
public:
	ShaderLibrary(const std::string& shaderFolder);

	bool LoadShaders();
	void UnloadShaders();

	Shader* GetShader(const std::string& shaderName);
private:
	std::vector<Shader*> loadedShaders;
	std::string shadersPath;
};

static ShaderLibrary* LoadedShaders = NULL;
#endif