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
		// vertex								// tex coords
		centerW - 50.0f, centerH + 50.0f, 1.0f,   0.0f, 1.0f, // 980 left
		centerW - 50.0f, centerH - 50.0f, 1.0f,   0.0f, 0.0f,
		centerW + 50.0f, centerH - 50.0f, 1.0f,   1.0f, 0.0f,
		
		centerW + 50.0f, centerH + 50.0f, 1.0f,   1.0f, 1.0f,
		centerW - 50.0f, centerH + 50.0f, 1.0f,   0.0f, 1.0f,
		centerW + 50.0f, centerH - 50.0f, 1.0f,   1.0f, 0.0f
	};

	glGenVertexArrays(1, &VAO_cross); // all information about the object
	glGenBuffers(1, &VBO_cross);      // we keep here attributes etc.

	glBindVertexArray(VAO_cross);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_cross);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	if (!Model::LoadTexture("./models/crosshair.png", texID_cross))
	{
		printf("WARNING: Unable to load crosshair texture!\n");
	}

	float vertices_h[] = {
		50.0f, 100.0f, 1.0f,    1.0f, 0.0f,
		50.0f, 50.0f, 1.0f,     1.0f, 1.0f,
		100.0f, 50.0f, 1.0f,    0.0f, 1.0f,

		100.0f, 100.0f, 1.0f,   0.0f, 0.0f,
		50.0f, 100.0f, 1.0f,    1.0f, 0.0f,
		100.0f, 50.0f, 1.0f,    0.0f, 1.0f
	};

	glGenVertexArrays(1, &VAO_heart);
	glGenBuffers(1, &VBO_heart);

	glBindVertexArray(VAO_heart);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_heart);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_h), vertices_h, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	if (!Model::LoadTexture("./models/heart.png", texID_heart_f) | !Model::LoadTexture("./models/heart_empty.png", texID_heart_e))
	{
		printf("WARNING: Unable to load heart texture!\n");
	}
}

void HUDRenderer::Visualize()
{
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	VisualizeCrosshair();

	int h_offset = 0;

	for (int i = 0; i < pl->health; i++)
	{
		VisualizeHealth(true, h_offset * 30.0f);
		h_offset++;
	}

	for (int i = pl->health_max - pl->health; i > 0; i--)
	{
		VisualizeHealth(false, h_offset * 30.0f);
		h_offset++;
	}

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

void HUDRenderer::VisualizeCrosshair()
{
	shd->use();
	shd->setMat4("ortho", orthoMat);
	shd->setFloat("render_offset", 0.0f);

	glActiveTexture(GL_TEXTURE0);

	glUniform1i(glGetUniformLocation(shd->ID, "text_diffuse"), 0);
	glBindTexture(GL_TEXTURE_2D, texID_cross);

	glBindVertexArray(VAO_cross);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
}

void HUDRenderer::VisualizeHealth(bool isFilled, float offset)
{
	shd->use();
	shd->setMat4("ortho", orthoMat);
	shd->setFloat("render_offset", offset);

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(shd->ID, "text_diffuse"), 0);
	if (isFilled)
	{ // use filled heart texture
		glBindTexture(GL_TEXTURE_2D, texID_heart_f);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, texID_heart_e);
	}

	glBindVertexArray(VAO_heart);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
}