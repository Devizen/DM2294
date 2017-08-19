#include "RenderHelper.h"
#include "Mesh.h"
#include "GraphicsManager.h"
#include "ShaderProgram.h"
#include "MatrixStack.h"
#include "GL\glew.h"
#include "../../Base/Source/DepthFBO.h"
#include "../../Base/Source/Attributes.h"
#include "../../Base/Source/PlayerInfo/PlayerInfo.h"
#include "../../Base/Source/Application.h"
#include "MeshBuilder.h"

void RenderHelper::RenderMesh(Mesh* _mesh)
{
	// Get all our transform matrices & update shader
	Mtx44 MVP;
	GraphicsManager* g = GraphicsManager::GetInstance();
	if (DepthFBO::GetInstance()->m_renderPass == DepthFBO::RENDER_PASS_PRE)
	{
		Mtx44 lightDepthMVP = DepthFBO::GetInstance()->m_lightDepthProj * DepthFBO::GetInstance()->m_lightDepthView * g->GetModelStack().Top();
		g->SetActiveShader("gpass");
		
		ShaderProgram* gpassShader = g->GetActiveShader();
		gpassShader->UpdateMatrix44("lightDepthMVP", &lightDepthMVP.a[0]);
		//glUniformMatrix4fv(g->GetActiveShader()->GetUniform("lightDepthMVP"), 1, GL_FALSE, &lightDepthMVP.a[0]);
		//mesh->Render();

		//for (int i = 0; i < MAX_TEXTURES; ++i)
		//{
		if (_mesh->textureID > 0)
		{
			gpassShader->UpdateInt("colorTextureEnabled[0]", 1);
			GraphicsManager::GetInstance()->UpdateTexture(0, _mesh->textureID);
			gpassShader->UpdateInt("colorTexture[0]", 0);

		}
		else
			gpassShader->UpdateInt("colorTextureEnabled[0]", 0);
		//	}
		_mesh->Render();

		g->SetActiveShader("default");

		return;
	}
	g->SetActiveShader("default");
	MVP = GraphicsManager::GetInstance()->GetProjectionMatrix() * GraphicsManager::GetInstance()->GetViewMatrix() * GraphicsManager::GetInstance()->GetModelStack().Top();
	ShaderProgram* currProg = GraphicsManager::GetInstance()->GetActiveShader();
	currProg->UpdateMatrix44("MVP", &MVP.a[0]);

	// Disable lighting stuff
	currProg->UpdateInt("lightEnabled", 0);

	// Update textures first if available
	if (_mesh->textureID > 0)
	{
		currProg->UpdateInt("colorTextureEnabled", 1);
		GraphicsManager::GetInstance()->UpdateTexture(0, _mesh->textureID);
		currProg->UpdateInt("colorTexture", 0);
	}
	else
	{
		currProg->UpdateInt("colorTextureEnabled", 0);
	}

	// Do actual rendering
	_mesh->Render();

	// Unbind texture for safety (in case next render call uses it by accident)
	if (_mesh->textureID > 0)
	{
		GraphicsManager::GetInstance()->UnbindTexture(0);
	}
}

void RenderHelper::RenderMeshWithLight(Mesh* _mesh)
{
	// Get all our transform matrices & update shader
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	GraphicsManager* g = GraphicsManager::GetInstance();
	
	if (DepthFBO::GetInstance()->m_renderPass == DepthFBO::RENDER_PASS_PRE)
	{
		Mtx44 lightDepthMVP = DepthFBO::GetInstance()->m_lightDepthProj * DepthFBO::GetInstance()->m_lightDepthView * g->GetModelStack().Top();
		g->SetActiveShader("gpass");
		ShaderProgram* gpassShader = g->GetActiveShader();
		glUniformMatrix4fv(g->GetActiveShader()->GetUniform("lightDepthMVP"), 1, GL_FALSE, &lightDepthMVP.a[0]);
		//mesh->Render();

		//for (int i = 0; i < MAX_TEXTURES; ++i)
		//{
			if (_mesh->textureID > 0)
			{
				gpassShader->UpdateInt("colorTextureEnabled[0]", 1);
				GraphicsManager::GetInstance()->UpdateTexture(0, _mesh->textureID);
				gpassShader->UpdateInt("colorTexture[0]", 0);

			}
			else
				gpassShader->UpdateInt("colorTextureEnabled[0]", 0);
	//	}
		_mesh->Render();

		g->SetActiveShader("default");

		return;
	}

	MVP = GraphicsManager::GetInstance()->GetProjectionMatrix() * GraphicsManager::GetInstance()->GetViewMatrix() * GraphicsManager::GetInstance()->GetModelStack().Top();
	ShaderProgram* currProg = GraphicsManager::GetInstance()->GetActiveShader();
	currProg->UpdateMatrix44("MVP", &MVP.a[0]);

	// Update light stuff
	currProg->UpdateInt("lightEnabled", 1);
	modelView = GraphicsManager::GetInstance()->GetViewMatrix() * GraphicsManager::GetInstance()->GetModelStack().Top();
	currProg->UpdateMatrix44("MV", &modelView.a[0]);
	modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
	currProg->UpdateMatrix44("MV_inverse_transpose", &modelView.a[0]);

	Mtx44 lightDepthMVP = DepthFBO::GetInstance()->m_lightDepthProj *
		DepthFBO::GetInstance()->m_lightDepthView * g->GetModelStack().Top();

	glUniformMatrix4fv(g->GetActiveShader()->GetUniform("lightDepthMVP"), 1,
		GL_FALSE, &lightDepthMVP.a[0]);

	//load material
	currProg->UpdateVector3("material.kAmbient", &_mesh->material.kAmbient.r);
	currProg->UpdateVector3("material.kDiffuse", &_mesh->material.kDiffuse.r);
	currProg->UpdateVector3("material.kSpecular", &_mesh->material.kSpecular.r);
	currProg->UpdateFloat("material.kShininess", _mesh->material.kShininess);
	
	// Update textures first if available
	if (_mesh->textureID > 0)
	{
		currProg->UpdateInt("colorTextureEnabled", 1);
		GraphicsManager::GetInstance()->UpdateTexture(0, _mesh->textureID);
		currProg->UpdateInt("colorTexture", 0);
	}
	else
	{
		currProg->UpdateInt("colorTextureEnabled", 0);
	}

	// Do actual rendering
	_mesh->Render();

	// Unbind texture for safety (in case next render call uses it by accident)
	if (_mesh->textureID > 0)
	{
		GraphicsManager::GetInstance()->UnbindTexture(0);
	}
}

void RenderHelper::RenderText(Mesh* _mesh, const std::string& _text, Color _color)
{
	// Trivial Rejection : Unable to render without mesh or texture
	if (!_mesh || _mesh->textureID <= 0)
		return;

	ShaderProgram* currProg = GraphicsManager::GetInstance()->GetActiveShader();

	currProg->UpdateInt("textEnabled", 1);
	currProg->UpdateVector3("textColor", &_color.r);
	currProg->UpdateInt("lightEnabled", 0);
	currProg->UpdateInt("colorTextureEnabled", 1);
	
	currProg->UpdateInt("colorTextureEnabled", 1);
	GraphicsManager::GetInstance()->UpdateTexture(0, _mesh->textureID);
	currProg->UpdateInt("colorTexture", 0);

	for (unsigned i = 0; i < _text.length(); ++i)
	{
		Mtx44 characterSpacing, MVP;
		characterSpacing.SetToTranslation(i * 0.5f, 0, 0); // 1.0f is the spacing of each character, you may change this value
		//characterSpacing.SetToTranslation((i+0.7f) * 1.f, 0, 0); // 1.0f is the spacing of each character, you may change this value
		//characterSpacing.SetToTranslation((float)(0.75f + (float)i), 0.0f, 0.0f); // 1.0f is the spacing of each character, you may change this value
		MVP = GraphicsManager::GetInstance()->GetProjectionMatrix() * GraphicsManager::GetInstance()->GetViewMatrix() * GraphicsManager::GetInstance()->GetModelStack().Top() * characterSpacing;
		currProg->UpdateMatrix44("MVP", &MVP.a[0]);

		_mesh->Render((unsigned)_text[i] * 6, 6);
	}

	GraphicsManager::GetInstance()->UnbindTexture(0);
	currProg->UpdateInt("textEnabled", 0);
}

void RenderHelper::renderPlayerHealth(void)
{
	CPlayerInfo* player = CPlayerInfo::GetInstance();
	/*Calculating the alignment offset of health bar. Value is based on the scaling.*/
	float healthBarOffset = CPlayerInfo::GetInstance()->getAttribute(CAttributes::TYPE_HEALTH) * Application::GetInstance().GetWindowWidth() * 0.005f;

	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();

	/*Translate the health bar to the left bottom of screen with offset that align the health bar.*/
	if (Application::GetInstance().GetWindowWidth() / Application::GetInstance().GetWindowHeight() < 1.5f)
		modelStack.Translate((-Application::GetInstance().GetWindowWidth() * 0.49f) + (healthBarOffset) * 0.5f, -Application::GetInstance().GetWindowHeight() * 0.4f, 0.f);
	else
		modelStack.Translate((-Application::GetInstance().GetWindowWidth() * 0.49f) + (healthBarOffset) * 0.5f, -Application::GetInstance().GetWindowHeight() * 0.35f, 0.f);
	modelStack.Scale(CPlayerInfo::GetInstance()->getAttribute(CAttributes::TYPE_HEALTH) * Application::GetInstance().GetWindowWidth() * 0.005f, Application::GetInstance().GetWindowHeight() * 0.05f, 1.f);
	
	/*Changing the health bar colour according to player health left %. 100% == Green, > 20% == Yellow and below 20% == Red.*/
	if (player->getAttribute(CAttributes::TYPE_HEALTH) == player->getAttribute(CAttributes::TYPE_MAXHEALTH))
		MeshBuilder::GetInstance()->GenerateCube("PLAYER_HEALTH_BAR", Color(0.f, 1.0f, 0.0f), 1.0f);
	else if (player->getAttribute(CAttributes::TYPE_HEALTH) / player->getAttribute(CAttributes::TYPE_MAXHEALTH) > 0.2f)
		MeshBuilder::GetInstance()->GenerateCube("PLAYER_HEALTH_BAR", Color(1.f, 1.0f, 0.0f), 1.0f);
	else
		MeshBuilder::GetInstance()->GenerateCube("PLAYER_HEALTH_BAR", Color(1.f, 0.0f, 0.0f), 1.0f);
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("PLAYER_HEALTH_BAR"));
	modelStack.PopMatrix();
}

void RenderHelper::renderKOCount(void)
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	if (Application::GetInstance().GetWindowWidth() / Application::GetInstance().GetWindowHeight() < 1.5f)
		modelStack.Translate(Application::GetInstance().GetWindowWidth() * 0.3f, -Application::GetInstance().GetWindowHeight() * 0.2f, 0.f);
	else
		modelStack.Translate(Application::GetInstance().GetWindowWidth() * 0.3f, -Application::GetInstance().GetWindowHeight() * 0.1f, 0.f);
	modelStack.Scale(Application::GetInstance().GetWindowWidth() * 0.1f, Application::GetInstance().GetWindowWidth() * 0.1f, 1.f);
	RenderHelper::RenderText(MeshBuilder::GetInstance()->GetMesh("text"), "KO:" + to_string(CPlayerInfo::GetInstance()->getKO_Count()), Color(1.f, 0.f, 0.f));
	modelStack.PopMatrix();
}
