#ifndef WORLD_H
#define WORLD_H

#include "Scene.h"
#include <vector>

using std::vector;

class FPSCamera;
class ShaderProgram;
class SceneManager;
class Light;
class DepthFBO;
class CCinematic;
class CPlayer;
class CEnemy;
class CBattle;
class CKeyboard;
class CMouse;

/*Forward Declaration*/
class Controls;

class World : public Scene
{
	//float m_worldHeight;
	//float m_worldWidth;
public:
	World();
	~World();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	World(SceneManager* _sceneMgr); // This is used to register to SceneManager

	CPlayer* player;
	CEnemy* enemy;
	FPSCamera* camera;
	CCinematic* cinematic;
	Light* lights[2];

	static World* sInstance; // The pointer to the object that gets registered

	void RenderPassGPass(void);
	void RenderPassMain(void);
	void RenderWorld(void);
	void RenderTerrain(void);

	void UpdatePlayerMovement(double dt);
	void UpdateCameraMovement(double dt);
	/*bool openInventory;
	bool openEQ;*/

	/*Cinematic*/
	//bool cinematicMode;

	float windowHeight;
	float windowWidth;

	//double saveMapTime;

	int selectedChoice;

	int loopCredits;
};

#endif