#include "Minimap.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "../EntityManager.h"
#include "GL\glew.h"
#include "MeshBuilder.h"
#include "..\Enemy\Enemy3D.h"
#include "../Application.h"

CMinimap::CMinimap(void)
	: m_cMinimap_Background(NULL)
	, m_cMinimap_Border(NULL)
	, m_cMinimap_Avatar(NULL)
	, m_cMinimap_Target(NULL)
	, m_cMinimap_Stencil(NULL)
	, m_iAngle(-90)
	, mode(MODE_2D)
	, enlargedMap(false)
{
	Init();
}
CMinimap::~CMinimap(void)
{
	if (m_cMinimap_Background)
	{
		delete m_cMinimap_Background;
		m_cMinimap_Background = NULL;
	}
	if (m_cMinimap_Border)
	{
		delete m_cMinimap_Border;
		m_cMinimap_Border = NULL;
	}
	if (m_cMinimap_Avatar)
	{
		delete m_cMinimap_Avatar;
		m_cMinimap_Avatar = NULL;
	}
	if (m_cMinimap_Target)
	{
		delete m_cMinimap_Target;
		m_cMinimap_Target = NULL;
	}
}

// Initialise this class instance
bool CMinimap::Init(void)
{
	m_iAngle = -90;
	position.Set(Application::GetInstance().GetWindowWidth() * 0.4f, Application::GetInstance().GetWindowHeight() * 0.4f, 0.0f);
	scale.Set(Application::GetInstance().GetWindowWidth() * 0.1f, Application::GetInstance().GetWindowWidth() * 0.1f, Application::GetInstance().GetWindowWidth() * 0.1f);

	return true;
}

bool CMinimap::SetTarget(Mesh* aTarget)
{
	if(aTarget != NULL)
	{
		m_cMinimap_Target = aTarget;
		return true;
	}
	return false;
}
Mesh* CMinimap::GetTarget(void) const
{
	return m_cMinimap_Target;
}
// Set the background mesh to this class
bool CMinimap::SetBackground(Mesh* aBackground)
{
	if (aBackground != NULL)
	{
		m_cMinimap_Background = aBackground;
		return true;
	}
	return false;
}
// Get the background mesh to this class
Mesh* CMinimap::GetBackground(void) const
{
	return m_cMinimap_Background;
}
// Set the Border mesh to this class
bool CMinimap::SetBorder(Mesh* aBorder)
{
	if (aBorder != NULL)
	{
		m_cMinimap_Border = aBorder;
		return true;
	}
	return false;
}
// Get the Border mesh to this class
Mesh* CMinimap::GetBorder(void) const
{
	return m_cMinimap_Border;
}
// Set the Avatar mesh to this class
bool CMinimap::SetAvatar(Mesh* anAvatar)
{
	if (anAvatar != NULL)
	{
		m_cMinimap_Avatar = anAvatar;
		return true;
	}
	return false;
}
// Get the Avatar mesh to this class
Mesh* CMinimap::GetAvatar(void) const
{
	return m_cMinimap_Avatar;
}

// Set m_iAngle of avatar
bool CMinimap::SetAngle(const int m_iAngle)
{
	this->m_iAngle = m_iAngle;
	return true;
}
// Get m_iAngle
int CMinimap::GetAngle(void) const
{
	return m_iAngle;
}

// Set the Stencil mesh to this class
bool CMinimap::SetStencil(Mesh* aStencil)
{
	if (aStencil != NULL)
	{
		m_cMinimap_Stencil = aStencil;
		return true;
	}
	return false;
}
// Get the Stencil mesh to this class
Mesh* CMinimap::GetStencil(void) const
{
	return m_cMinimap_Stencil;
}

void CMinimap::RenderUI()
{
	if (mode == MODE_3D)
		return;

	/*Translating base on aspect raio 4:3 and 16:9*/
	if (Application::GetInstance().GetWindowWidth() / Application::GetInstance().GetWindowHeight() < 1.5f)
		position.Set(Application::GetInstance().GetWindowWidth() * 0.4f, Application::GetInstance().GetWindowHeight() * 0.4f, 0.0f);
	else
		position.Set(Application::GetInstance().GetWindowWidth() * 0.4f, Application::GetInstance().GetWindowHeight() * 0.35f, 0.0f);

	scale.Set(Application::GetInstance().GetWindowWidth() * 0.1f, Application::GetInstance().GetWindowWidth() * 0.1f, Application::GetInstance().GetWindowWidth() * 0.1f);

	//int halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2;
	//int halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2;
	//GraphicsManager::GetInstance()->SetOrthographicProjection(-halfWindowWidth, halfWindowWidth, -halfWindowHeight, halfWindowHeight, -10, 10);

	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();

	// Push the current transformation into the modelStack
	modelStack.PushMatrix();

		// Translate the current transformation
		modelStack.Translate(position.x, position.y, position.z);
		// Scale the current transformation
		modelStack.Scale(scale.x, scale.y, scale.z);

		// Push the current transformation into the modelStack
		modelStack.PushMatrix();

			// Enable stencil mode
			glEnable(GL_STENCIL_TEST);

			// Configure stencil mode
			glStencilFunc(GL_ALWAYS, 1, 0xFF); // Set any stencil to 1
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			glStencilMask(0xFF); // Write to stencil buffer
			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); // Don't write to colour buffer
			glDepthMask(GL_FALSE); // Don't write to depth buffer
			glClear(GL_STENCIL_BUFFER_BIT); // Clear stencil buffer (0 by default)

			if (m_cMinimap_Stencil)
				RenderHelper::RenderMesh(m_cMinimap_Stencil);

			// Switch off stencil function
			glStencilFunc(GL_EQUAL, 1, 0xFF); // Pass test if stencil value is 1
			glStencilMask(0x00); // Don't write anything to stencil buffer
			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); // Write to colour buffer
			glDepthMask(GL_TRUE); // Write to depth buffer

			modelStack.PushMatrix();
				// Rotate the current transformation
				modelStack.Rotate(m_iAngle, 0.0f, 0.0f, -1.0f);
				if (m_cMinimap_Background)
					RenderHelper::RenderMesh(m_cMinimap_Background);

				//// Render enemies
				//for (list<CEnemy3D*>::iterator it = EntityManager::GetInstance()->returnEnemy().begin(); it != EntityManager::GetInstance()->returnEnemy().end(); ++it)
				//{
				//	CEnemy3D* enemy = (CEnemy3D*)*it;

				//	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh("cube");
				//	modelStack.PushMatrix();
				//	Vector3 displacement = -CPlayerInfo::GetInstance()->GetPos() + enemy->GetPos();
				//	modelStack.Translate(displacement.x, displacement.y, 0);
				//	modelStack.Scale(20, 20, 20);
				//		// Rotate the current transformation
				//		modelStack.Rotate(m_iAngle, 0.0f, 0.0f, -1.0f);
				//		// Render an enemy
				//		RenderHelper::RenderMesh(modelMesh);
				//	modelStack.PopMatrix();
				//}
			modelStack.PopMatrix();

			// Disable depth test
			glDisable(GL_DEPTH_TEST);

			// Display the Avatar
			if (m_cMinimap_Avatar)
				RenderHelper::RenderMesh(m_cMinimap_Avatar);

			// Enable depth test
			glEnable(GL_DEPTH_TEST);

			// Disable stencil test
			glDisable(GL_STENCIL_TEST);
		modelStack.PopMatrix();

		if (m_cMinimap_Border)
			RenderHelper::RenderMesh(m_cMinimap_Border);

	modelStack.PopMatrix();

	if (enlargedMap)
	{
		Mesh* modelMesh;
		modelMesh = MeshBuilder::GetInstance()->GetMesh("MINIMAP_ENLARGED");
		MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
		modelStack.PushMatrix();
		modelStack.Translate(0.f, 0.f, 0.f);
		//modelStack.Rotate(-20.f, 0.f, 0.f, 1.f);
		modelStack.Scale(500.f, 500.f, 0.f);
		RenderHelper::RenderMesh(modelMesh);
		modelStack.PopMatrix();
	}
	// Render enemies
	for (list<CEnemy3D*>::iterator it = EntityManager::GetInstance()->returnEnemy().begin(); it != EntityManager::GetInstance()->returnEnemy().end(); ++it)
	{
		CEnemy3D* enemy = (CEnemy3D*)*it;

		Vector3 displacement = -CPlayerInfo::GetInstance()->GetPos() + enemy->GetPos();
		float length = displacement.Length();
		if (!enlargedMap)
		{
			if (displacement.Length() > 300.f)
				continue;
		}
		Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh("ENEMY");
		modelStack.PushMatrix();
		if (!enlargedMap)
			modelStack.Translate(this->position.x, this->position.y, 0.f);
		Vector3 direction = (-CPlayerInfo::GetInstance()->GetPos() + CPlayerInfo::GetInstance()->GetTarget()).Normalize();
		float angle = Math::RadianToDegree(atan2f(direction.z - displacement.z, direction.x - displacement.x));
		angle = Math::RadianToDegree(acos(direction.Normalized().Dot(displacement.Normalized())));

		Vector3 rotateAboutAxis(1.f, 1.f, 1.f);

		try {
			rotateAboutAxis = direction.Cross(displacement).Normalize();
		}
		catch (std::exception &e) {
			//std::cout << "Divide by Zero" << std::endl;
		}
		displacement *= 0.1f;
		/*This is to throw Divide By Zero that happens when the player and enemy position aligns.*/
		try {
			modelStack.Rotate(angle, 0.f, 0.f, rotateAboutAxis.y);
		}
		catch (std::exception &e) {
			//std::cout << "Divide by Zero" << std::endl;
		}
		modelStack.Rotate(90.f, 0.f, 0.f, 1.f);
		
		if (!enlargedMap)
			modelStack.Translate(length / 8.f, 0, 0);
		else
			modelStack.Translate(length / 5.f, 0, 0);
		modelStack.Scale(10, 10, 10);
		RenderHelper::RenderMesh(modelMesh);
		modelStack.PopMatrix();
	}
}

CMinimap* Create::Minimap(const bool m_bAddToLibrary)
{
	CMinimap* result = CMinimap::GetInstance();
	if (m_bAddToLibrary)
		EntityManager::GetInstance()->AddEntity(result);
	return result;
}
