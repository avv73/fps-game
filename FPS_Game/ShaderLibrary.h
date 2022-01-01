#pragma once
#ifndef SHADERLIBRARY_H

#include <iostream>
#include <string>
#include <vector>
#include "Shader.h"

class ShaderLibrary
{
public:
	bool LoadShaders();
	void UnloadShaders();

	void SetPVGlobal(const glm::mat4& proj, const glm::mat4& view);
	void SetShaderPath(const std::string& path);

	Shader* GetShader(const std::string& shaderName);

	static ShaderLibrary* GetInstance();
private:
	ShaderLibrary();

	std::vector<Shader*> loadedShaders;
	std::string shadersPath;

	static ShaderLibrary* libInstance;
};
#endif