#include "HUDRenderer.h"
#include "ShaderLibrary.h"

HUDRenderer::HUDRenderer(Player* pl)
{
	this->pl = pl;
	this->orthoMat = pl->camera->GetOrthogonalMatrix();
	shd = ShaderLibrary::GetInstance()->GetShader("HUD");

	CreateHUD();
}

void HUDRenderer::CreateHUD()
{
	float centerH = pl->camera->hSize / 2.0f;
	float centerW = pl->camera->wSize / 2.0f;


	// TODO: Transperant, apply cross texture, make the same for hearth/ammo for player.
	float vertices[] = {
		// vertex            // colors
		centerW - 50.0f, centerH + 50.0f, 1.0f,   0.3f, 0.15f, 0.0f, // 980 left
		centerW - 50.0f, centerH - 50.0f, 1.0f,   0.3f, 0.15f, 0.0f,
		centerW + 50.0f, centerH - 50.0f, 1.0f,   0.3f, 0.15f, 0.0f,
		
		centerW + 50.0f, centerH + 50.0f, 1.0f,   0.3f, 0.15f, 0.0f,
		centerW - 50.0f, centerH + 50.0f, 1.0f,   0.3f, 0.15f, 0.0f,
		centerW + 50.0f, centerH - 50.0f, 1.0f,   0.3f, 0.15f, 0.0f
	};

	glGenVertexArrays(1, &VAO); // all information about the object
	glGenBuffers(1, &VBO);      // we keep here attributes etc.

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void HUDRenderer::Visualize()
{
	glDisable(GL_DEPTH_TEST);

	shd->use();
	shd->setMat4("ortho", orthoMat);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);

	glEnable(GL_DEPTH_TEST);
}