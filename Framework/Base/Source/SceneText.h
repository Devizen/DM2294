#ifndef SCENE_TEXT_H
#define SCENE_TEXT_H

#include "Scene.h"
#include "Mtx44.h"
#include "PlayerInfo/PlayerInfo.h"
#include "GroundEntity.h"
#include "FPSCamera.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "GenericEntity.h"
#include "Enemy/Enemy3D.h"
#include "HardwareAbstraction\Keyboard.h"
#include "Minimap\Minimap.h"
#include "CameraEffects\CameraEffects.h"
#include "HardwareAbstraction\Mouse.h"
#include "DepthFBO.h"

class ShaderProgram;
class SceneManager;
class TextEntity;
class SpriteEntity;
class Light;
class DepthFBO;

/*Forward Declaration*/
class Controls;

class SceneText : public Scene
{	
public:
	SceneText();
	~SceneText();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	SceneText(SceneManager* _sceneMgr); // This is used to register to SceneManager

	ShaderProgram* currProg;
	ShaderProgram* m_gPassShaderID;
	CPlayerInfo* playerInfo;
	GroundEntity* groundEntity;
	FPSCamera camera;
	TextEntity* textObj[30];
	Light* lights[2];

	CKeyboard* theKeyboard;
	CMouse* theMouse;
	CMinimap* theMinimap;
	CCameraEffects* theCameraEffects;
	GenericEntity* theOption;
	vector<GenericEntity*>theObjects;

	GenericEntity* theCube;
	CEnemy3D* anEnemy3D;	// This is the CEnemy class for 3D use.

	static SceneText* sInstance; // The pointer to the object that gets registered

	/*Controls*/
	vector<Controls*>controls;
	TextEntity* controlText[17];
	Controls* input;

	void pauseOptions(double dt,bool &pause);
	void clearKeyDisplay(void);
	SpriteEntity* optionUI;
	SpriteEntity* weaponUI;
	SpriteEntity* hitDisplay;

	/*DepthFBO; Shadow*/
	//DepthFBO m_lightDepthFBO;
	Mtx44 m_lightDepthProj;
	Mtx44 m_lightDepthView;
	/*0 for Render Pass Pre and 1 for Render Pass Main*/
	int m_renderPass;

	CWeaponInfo* primaryWeapon;
	CWeaponInfo** weaponManager;

	/*Changing Input*/
	void displayControls(void);
	template<typename T>
	string checkInput(T input);

	/*Render Weapon*/
	void renderWeapon(void);

	/*Render Weapon UI*/
	void renderWeaponUI(void);

	/*Create Random Enemies*/
	void createEnemies(double dt);

	/*Create Random Crates*/
	void createCrates(double dt);

	/*Create Random Bullets*/
	void createBullets(double dt);

	/*Create Random Health*/
	void createHealth(double dt);

	/*Render HIT*/
	void renderHit(void);

	/*Current highscore*/
	int currentHighscore;

	void RenderPassGPass(void);
	void RenderPassMain(void);
	void RenderWorld(void);

	/*Render KO Count*/
	void renderKO_Count(void);

	float m_worldHeight;
	float m_worldWidth;
};

#endif

template<typename T>
inline string SceneText::checkInput(T input)
{
	if (input == 1)
		return "Left Click";

	else if (input == 32)
		return "Spacebar";

	else if (input == 37)
		return "Right";

	else if (input == 38)
		return "Down";

	else if (input == 39)
		return "Left";

	else if (input == 40)
		return "Up";

	else if (input == 160)
		return "Left Shift";

	else if (input == 162)
		return "Left Control";

	else
	{
		char key = static_cast<char>(input);
		string value = "";
		value += key;
		return value;
	}
}
