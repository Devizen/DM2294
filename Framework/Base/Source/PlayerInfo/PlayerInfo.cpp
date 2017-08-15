#include "PlayerInfo.h"
#include <iostream>

#include "MouseController.h"
#include "KeyboardController.h"
#include "Mtx44.h"
#include "../Projectile/Projectile.h"
#include "../WeaponInfo/Pistol.h"
#include "../WeaponInfo/AssaultRifle.h"
#include "../Minimap/Minimap.h"
#include "../EntityManager.h"

/*Rendering Things; TODO: Shift all rendering stuff to RenderHelper.*/
#include "MeshBuilder.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "../Application.h"


// Allocating and initializing CPlayerInfo's static data member.  
// The pointer is allocated but not the object's constructor.
CPlayerInfo *CPlayerInfo::s_instance = 0;

CPlayerInfo::CPlayerInfo(void)
	: m_dSpeed(40.0)
	, m_dAcceleration(10.0)
	, m_bJumpUpwards(false)
	, m_dJumpSpeed(30.0)
	, m_dJumpAcceleration(-10.0)
	, m_bFallDownwards(false)
	, m_dFallSpeed(0.0)
	, m_dFallAcceleration(-10.0)
	, m_dElapsedTime(0.0)
	, attachedCamera(NULL)
	, m_pTerrain(NULL)
	, primaryWeapon(NULL)
	, secondaryWeapon(NULL)
	, theCurrentPosture(STAND)
	, weaponManager(NULL)
	, m_iCurrentWeapon(0)
	, m_fCameraSwayAngle(0.0f)
	, m_fCameraSwayDeltaAngle(0.1f)
	, m_fCameraSwayAngle_LeftLimit(-0.3f)
	, m_fCameraSwayAngle_RightLimit(0.3f)
	, m_bCameraSwayDirection(false)
    , theMaxPitch(80)
    , theMinPitch(-80)
	//, health(100)
	, tookDamage(false)
	, score(0.f)
	, KO_Count(0)
{
}

CPlayerInfo::~CPlayerInfo(void)
{
	if (weaponManager)
	{
		for (int i = 0; i < m_iNumOfWeapon; i++)
		{
			delete weaponManager[i];
		}
		delete [] weaponManager;
		weaponManager = NULL;
	}
	if (secondaryWeapon)
	{
		delete secondaryWeapon;
		secondaryWeapon = NULL;
	}
	if (primaryWeapon)
	{
		delete primaryWeapon;
		primaryWeapon = NULL;
	}
	m_pTerrain = NULL;
}

// Initialise this class instance
void CPlayerInfo::Init(void)
{
	// Set the default values
	defaultPosition.Set(0,0,10);
	defaultTarget.Set(0,0,0);
	defaultUp.Set(0,1,0);

	// Set the current values
	position.Set(0, 0, 10);
	target.Set(0, 0, 0);
	up.Set(0, 1, 0);

	// Set Boundary
	maxBoundary.Set(1,1,1);
	minBoundary.Set(-1, -1, -1);

	// Set the pistol as the primary weapon
	primaryWeapon = new CPistol();
	primaryWeapon->Init();
	cout << "Primary Weapon From Player: " << primaryWeapon << endl;

	weaponManager = new CWeaponInfo*[m_iNumOfWeapon];
	weaponManager[0] = new CPistol();
	weaponManager[0]->Init();

	weaponManager[1] = new CAssaultRifle();
	weaponManager[1]->Init();

	/*for (int i = 0; i < m_iNumOfWeapon; i++)
	{
		weaponManager[i] = new CPistol();
		weaponManager[i]->Init();
	}*/

	m_fCameraSwayAngle = 0.0f;
	m_fCameraSwayDeltaAngle = 1.0f;
	m_fCameraSwayAngle_LeftLimit = -5.0f;
	m_fCameraSwayAngle_RightLimit = 5.0f;
	m_bCameraSwayDirection = false;

	/*Set Bounding Box for player*/
	SetAABB(Vector3(2.f, 10.f, 2.f), Vector3(-2.f, -10.f, -2.f));
	EntityManager::GetInstance()->AddPlayer(this);
}

// Returns true if the player is on ground
bool CPlayerInfo::isOnGround(void)
{
	if (m_bJumpUpwards == false && m_bFallDownwards == false && position.y == m_pTerrain->GetTerrainHeight(position))
		return true;

	return false;
}

// Returns true if the player is jumping upwards
bool CPlayerInfo::isJumpUpwards(void)
{
	if (m_bJumpUpwards == true && m_bFallDownwards == false)
		return true;

	return false;
}

// Returns true if the player is on freefall
bool CPlayerInfo::isFreeFall(void)
{
	if (m_bJumpUpwards == false && m_bFallDownwards == true)
		return true;

	return false;
}

// Set the player's status to free fall mode
void CPlayerInfo::SetOnFreeFall(bool isOnFreeFall)
{
	if (isOnFreeFall == true)
	{
		m_bJumpUpwards = false;
		m_bFallDownwards = true;
		m_dFallSpeed = 0.0;
	}
}

// Set the player to jumping upwards
void CPlayerInfo::SetToJumpUpwards(bool isOnJumpUpwards)
{
	if (isOnJumpUpwards == true)
	{
		m_bJumpUpwards = true;
		m_bFallDownwards = false;
		m_dJumpSpeed = 10.0;
	}
}

// Set position
void CPlayerInfo::SetPos(const Vector3& pos)
{
	position = pos;
}

// Set target
void CPlayerInfo::SetTarget(const Vector3& target)
{
	this->target = target;
}

// Set position
void CPlayerInfo::SetUp(const Vector3& up)
{
	this->up = up;
}

// Set m_dJumpAcceleration of the player
void CPlayerInfo::SetJumpAcceleration(const double m_dJumpAcceleration)
{
	this->m_dJumpAcceleration = m_dJumpAcceleration;
}

// Set Fall Acceleration of the player
void CPlayerInfo::SetFallAcceleration(const double m_dFallAcceleration)
{
	this->m_dFallAcceleration = m_dFallAcceleration;
}

// Set the boundary for the player info
void CPlayerInfo::SetBoundary(Vector3 max, Vector3 min)
{
	maxBoundary = max;
	minBoundary = min;
}

// Set the terrain for the player info
void CPlayerInfo::SetTerrain(GroundEntity* m_pTerrain)
{
	if (m_pTerrain != NULL)
	{
		this->m_pTerrain = m_pTerrain;

		SetBoundary(this->m_pTerrain->GetMaxBoundary(), this->m_pTerrain->GetMinBoundary());
	}
}

// Stop the player's movement
void CPlayerInfo::StopVerticalMovement(void)
{
	m_bJumpUpwards = false;
	m_bFallDownwards = false;
}

// Reset this player instance to default
void CPlayerInfo::Reset(void)
{
	// Set the current values to default values
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;

	// Stop vertical movement too
	StopVerticalMovement();
}

// Get position x of the player
Vector3 CPlayerInfo::GetPos(void) const
{
	return position;
}

// Get target
Vector3 CPlayerInfo::GetTarget(void) const
{
	return target;
}
// Get Up
Vector3 CPlayerInfo::GetUp(void) const
{
	return up;
}

// Get m_dJumpAcceleration of the player
double CPlayerInfo::GetJumpAcceleration(void) const
{
	return m_dJumpAcceleration;
}

// Update Jump Upwards
void CPlayerInfo::UpdateJumpUpwards(double dt)
{
	if (m_bJumpUpwards == false)
		return;

	// Update the jump's elapsed time
	m_dElapsedTime += dt;

	// Update position and target y values
	// Use SUVAT equation to update the change in position and target
	// s = u * t + 0.5 * a * t ^ 2
	position.y += (float)(m_dJumpSpeed * m_dElapsedTime + 
						  0.5 * m_dJumpAcceleration * m_dElapsedTime * m_dElapsedTime);
	target.y += (float)(m_dJumpSpeed * m_dElapsedTime + 
						0.5 * m_dJumpAcceleration * m_dElapsedTime * m_dElapsedTime);
	// Use this equation to calculate final velocity, v
	// SUVAT: v = u + a * t; v is m_dJumpSpeed AFTER updating using SUVAT where u is 
	// the initial speed and is equal to m_dJumpSpeed
	m_dJumpSpeed = m_dJumpSpeed + m_dJumpAcceleration * m_dElapsedTime;
	// Check if the jump speed is less than zero, then it should be falling
	if (m_dJumpSpeed < 0.0)
	{
		m_dJumpSpeed = 0.0;
		m_bJumpUpwards = false;
		m_dFallSpeed = 0.0;
		m_bFallDownwards = true;
		m_dElapsedTime = 0.0;
	}
}

// Update FreeFall
void CPlayerInfo::UpdateFreeFall(double dt)
{
	if (m_bFallDownwards == false)
		return;

	// Update the jump's elapsed time
	m_dElapsedTime += dt;

	// Update position and target y values.
	// Use SUVAT equation to update the change in position and target
	// s = u * t + 0.5 * a * t ^ 2
	position.y += (float)(m_dFallSpeed * m_dElapsedTime + 
						  0.5 * m_dJumpAcceleration * m_dElapsedTime * m_dElapsedTime);
	target.y += (float)(m_dFallSpeed * m_dElapsedTime + 
						0.5 * m_dJumpAcceleration * m_dElapsedTime * m_dElapsedTime);
	// Use this equation to calculate final velocity, v
	// SUVAT: v = u + a * t;
	// v is m_dJumpSpeed AFTER updating using SUVAT where u is the initial speed and is equal to m_dJumpSpeed
	m_dFallSpeed = m_dFallSpeed + m_dFallAcceleration * m_dElapsedTime;
	// Check if the jump speed is below terrain, then it should be reset to terrain height
	if (position.y < m_pTerrain->GetTerrainHeight(position))
	{
		Vector3 viewDirection = target - position;
		position.y = m_pTerrain->GetTerrainHeight(position);
		target = position + viewDirection;
		m_dFallSpeed = 0.0;
		m_bFallDownwards = false;
		m_dElapsedTime = 0.0;
	}
}

/********************************************************************************
 Hero Update
 ********************************************************************************/
void CPlayerInfo::Update(double dt)
{
	//double mouse_diff_x, mouse_diff_y;
	//MouseController::GetInstance()->GetMouseDelta(mouse_diff_x, mouse_diff_y);

	//double camera_yaw = mouse_diff_x * 0.0174555555555556;		// 3.142 / 180.0
	//double camera_pitch = mouse_diff_y * 0.0174555555555556;	// 3.142 / 180.0

	// Check if there is a need to change posture
	/*if (KeyboardController::GetInstance()->IsKeyReleased('Z'))
	{
		theCurrentPosture = (CURRENT_POSTURE)(theCurrentPosture + 1);
		if (theCurrentPosture == NUM_POSTURE)
			theCurrentPosture = STAND;

		Vector3 viewDirection = target - position;
		switch (theCurrentPosture)
		{
		case STAND:
			position.y = m_pTerrain->GetTerrainHeight(Vector3(position.x, 0.0f, position.z));
			target = position + viewDirection;
			break;
		case CROUCH:
			position.y = m_pTerrain->GetTerrainHeight(Vector3(position.x, 0.0f, position.z));
			position.y -= 5.0f;
			target = position + viewDirection;
			break;
		case PRONE:
			position.y = m_pTerrain->GetTerrainHeight(Vector3(position.x, 0.0f, position.z));
			position.y -= 8.0f;
			target = position + viewDirection;
			break;
		default:
			break;
		}
	}*/

	//// Update the position if the WASD buttons were activated
	//if (KeyboardController::GetInstance()->IsKeyDown('W') ||
	//	KeyboardController::GetInstance()->IsKeyDown('A') ||
	//	KeyboardController::GetInstance()->IsKeyDown('S') ||
	//	KeyboardController::GetInstance()->IsKeyDown('D'))
	//{
	//	Vector3 viewVector = target - position;
	//	Vector3 rightUV;
	//	if (KeyboardController::GetInstance()->IsKeyDown('W'))
	//	{
	//		if ((KeyboardController::GetInstance()->IsKeyDown('W')) && (KeyboardController::GetInstance()->IsKeyDown(VK_SHIFT)))
	//			position += viewVector.Normalized() * (float)m_dSpeed * 2.0f * (float)dt;
	//		else
	//			position += viewVector.Normalized() * (float)m_dSpeed * (float)dt;
	//	}
	//	else if (KeyboardController::GetInstance()->IsKeyDown('S'))
	//	{
	//		position -= viewVector.Normalized() * (float)m_dSpeed * (float)dt;
	//	}
	//	if (KeyboardController::GetInstance()->IsKeyDown('A'))
	//	{
	//		rightUV = (viewVector.Normalized()).Cross(up);
	//		rightUV.y = 0;
	//		rightUV.Normalize();
	//		position -= rightUV * (float)m_dSpeed * (float)dt;
	//	}
	//	else if (KeyboardController::GetInstance()->IsKeyDown('D'))
	//	{
	//		rightUV = (viewVector.Normalized()).Cross(up);
	//		rightUV.y = 0;
	//		rightUV.Normalize();
	//		position += rightUV * (float)m_dSpeed * (float)dt;
	//	}
	//	 Constrain the position
	//	Constrain();
	//	// Update the target
	//	target = position + viewVector;
	//}

	//// Rotate the view direction
	//if (KeyboardController::GetInstance()->IsKeyDown(VK_LEFT) ||
	//	KeyboardController::GetInstance()->IsKeyDown(VK_RIGHT) ||
	//	KeyboardController::GetInstance()->IsKeyDown(VK_UP) ||
	//	KeyboardController::GetInstance()->IsKeyDown(VK_DOWN))
	//{
	//	Vector3 viewUV = (target - position).Normalized();
	//	Vector3 rightUV;
	//	if (KeyboardController::GetInstance()->IsKeyDown(VK_LEFT))
	//	{
	//		float yaw = (float)m_dSpeed * (float)dt;
	//		Mtx44 rotation;
	//		rotation.SetToRotation(yaw, 0, 1, 0);
	//		viewUV = rotation * viewUV;
	//		target = position + viewUV;
	//		rightUV = viewUV.Cross(up);
	//		rightUV.y = 0;
	//		rightUV.Normalize();
	//		up = rightUV.Cross(viewUV).Normalized();
	//	}
	//	else if (KeyboardController::GetInstance()->IsKeyDown(VK_RIGHT))
	//	{
	//		float yaw = (float)(-m_dSpeed * (float)dt);
	//		Mtx44 rotation;
	//		rotation.SetToRotation(yaw, 0, 1, 0);
	//		viewUV = rotation * viewUV;
	//		target = position + viewUV;
	//		rightUV = viewUV.Cross(up);
	//		rightUV.y = 0;
	//		rightUV.Normalize();
	//		up = rightUV.Cross(viewUV).Normalized();
	//	}
	//	if (KeyboardController::GetInstance()->IsKeyDown(VK_UP))
	//	{
	//		float pitch = (float)(m_dSpeed * (float)dt);
	//		rightUV = viewUV.Cross(up);
	//		rightUV.y = 0;
	//		rightUV.Normalize();
	//		up = rightUV.Cross(viewUV).Normalized();
	//		Mtx44 rotation;
	//		rotation.SetToRotation(pitch, rightUV.x, rightUV.y, rightUV.z);
	//		viewUV = rotation * viewUV;
	//		target = position + viewUV;
	//	}
	//	else if (KeyboardController::GetInstance()->IsKeyDown(VK_DOWN))
	//	{
	//		float pitch = (float)(-m_dSpeed * (float)dt);
	//		rightUV = viewUV.Cross(up);
	//		rightUV.y = 0;
	//		rightUV.Normalize();
	//		up = rightUV.Cross(viewUV).Normalized();
	//		Mtx44 rotation;
	//		rotation.SetToRotation(pitch, rightUV.x, rightUV.y, rightUV.z);
	//		viewUV = rotation * viewUV;
	//		target = position + viewUV;
	//	}
	//}

	////Update the camera direction based on mouse move
	//{
	//	Vector3 viewUV = (target - position).Normalized();
	//	Vector3 rightUV;

	//	if (camera_yaw != 0.0)
	//	{
	//		float yaw = (float)(-m_dSpeed * camera_yaw * (float)dt);
	//		Mtx44 rotation;
	//		rotation.SetToRotation(yaw, 0, 1, 0);
	//		viewUV = rotation * viewUV;
	//		target = position + viewUV;
	//		rightUV = viewUV.Cross(up);
	//		rightUV.y = 0;
	//		rightUV.Normalize();
	//		up = rightUV.Cross(viewUV).Normalized();
	//		
	//		// Update the minimap rotation angle
	//		CMinimap::GetInstance()->SetAngle(atan2(viewUV.z, viewUV.x) * 57.2883513685549146);
	//	}
	//	{
	//		float pitch = (float)(-m_dSpeed * camera_pitch * (float)dt);
	//		rightUV = viewUV.Cross(up);
	//		rightUV.y = 0;
	//		rightUV.Normalize();
	//		up = rightUV.Cross(viewUV).Normalized();
	//		Mtx44 rotation;
	//		rotation.SetToRotation(pitch, rightUV.x, rightUV.y, rightUV.z);
	//		viewUV = rotation * viewUV;
	//		target = position + viewUV;
	//	}
	//}

	//// If the user presses SPACEBAR, then make him jump
	//if (KeyboardController::GetInstance()->IsKeyDown(VK_SPACE) &&
	//	position.y == m_pTerrain->GetTerrainHeight(position))
	//{
	//	SetToJumpUpwards(true);
	//}

	//// Update the weapons
	//if (KeyboardController::GetInstance()->IsKeyReleased('R'))
	//{
	//	//if (primaryWeapon)
	//	//{
	//	//	primaryWeapon->Reload();
	//	//	//primaryWeapon->PrintSelf();
	//	//}
	//	if (weaponManager[m_iCurrentWeapon])
	//		weaponManager[m_iCurrentWeapon]->Reload();
	//}
	if (primaryWeapon)
		primaryWeapon->Update(dt);
	if (secondaryWeapon)
		secondaryWeapon->Update(dt);
	if (weaponManager[m_iCurrentWeapon])
		weaponManager[m_iCurrentWeapon]->Update(dt);

	//if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) != m_iCurrentWeapon)
	//{
	//	if ((MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) >= 0) &&
	//		(MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) < m_iNumOfWeapon))
	//	{
	//		m_iCurrentWeapon = MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET);
	//	}
	//}

	//// if Mouse Buttons were activated, then act on them
	//if (MouseController::GetInstance()->IsButtonPressed(MouseController::LMB))
	//{
	//	//if (primaryWeapon)
	//	//	primaryWeapon->Discharge(position, target, this);

	//	if (weaponManager[m_iCurrentWeapon])
	//		weaponManager[m_iCurrentWeapon]->Discharge(position, target, this);
	//}
	//else if (MouseController::GetInstance()->IsButtonPressed(MouseController::RMB))
	//{

	//}

	//// If the user presses R key, then reset the view to default values
	//if (KeyboardController::GetInstance()->IsKeyDown('P'))
	//{
	//	Reset();
	//}
	//else
	{
		// Constrain the position
		//Constrain();
		UpdateJumpUpwards(dt);
		UpdateFreeFall(dt);
	}

	// Do camera sway
	if (m_fCameraSwayAngle != 0.0f)
	{
		Mtx44 rotation;
		if (m_bCameraSwayDirection == false)
			rotation.SetToRotation(-m_fCameraSwayDeltaAngle, 0.0f, 0.0f, 1.0f);
		else if (m_bCameraSwayDirection == true)
			rotation.SetToRotation(m_fCameraSwayDeltaAngle, 0.0f, 0.0f, 1.0f);
		up = rotation * up;
	}

	// Update minimap rotation angle
	Vector3 viewUV = (target - position).Normalized();
	CMinimap::GetInstance()->SetAngle(atan2(viewUV.z, viewUV.x) * 57.2883513685549146);

	// If a camera is attached to this playerInfo class, then update it
	if (attachedCamera)
	{
		attachedCamera->SetCameraPos(position);
		attachedCamera->SetCameraTarget(target);
		attachedCamera->SetCameraUp(up);
	}
}

// Detect and process front / back movement on the controller
bool CPlayerInfo::Move_FrontBack(const float deltaTime, const bool direction, const float speedMultiplier)
{
	/*The logic of this is sending a ghost character to move first, if it collides with an object, do not update the actual player position.*/
	/*Ghost Position*/
	Vector3 ghostPosition(this->GetPos());
	Vector3 ghostMinAABB(this->GetMinAABB());
	Vector3 ghostMaxAABB(this->GetMaxAABB());

	// Add camera sway
	if (m_bCameraSwayDirection == false)
	{
		m_fCameraSwayAngle -= m_fCameraSwayDeltaAngle;
		if (m_fCameraSwayAngle < m_fCameraSwayAngle_LeftLimit * speedMultiplier)
			m_bCameraSwayDirection = !m_bCameraSwayDirection;
	}
	else
	{
		m_fCameraSwayAngle += m_fCameraSwayDeltaAngle;
		if (m_fCameraSwayAngle > m_fCameraSwayAngle_RightLimit * speedMultiplier)
			m_bCameraSwayDirection = !m_bCameraSwayDirection;
	}

	Vector3 viewVector = (target - position).Normalized();

	if (direction)
	{

		if ((theCurrentPosture == STAND) && (KeyboardController::GetInstance()->IsKeyDown(VK_SHIFT)))
			ghostPosition += viewVector * (float)m_dSpeed * 2.0f * (float)deltaTime;
		else if (theCurrentPosture == CROUCH)
			ghostPosition += viewVector * (float)m_dSpeed * 0.75f * (float)deltaTime;
		else if (theCurrentPosture == PRONE)
			ghostPosition += viewVector * (float)m_dSpeed * 0.25f * (float)deltaTime;
		else
			ghostPosition += viewVector * (float)m_dSpeed * (float)deltaTime;

		if (!EntityManager::GetInstance()->CheckAABBCollision(ghostMinAABB, ghostMaxAABB,ghostPosition))
		{
			if ((theCurrentPosture == STAND) && (KeyboardController::GetInstance()->IsKeyDown(VK_SHIFT)))
				position += viewVector * (float)m_dSpeed * 2.0f * (float)deltaTime;
			else if (theCurrentPosture == CROUCH)
				position += viewVector * (float)m_dSpeed * 0.75f * (float)deltaTime;
			else if (theCurrentPosture == PRONE)
				position += viewVector * (float)m_dSpeed * 0.25f * (float)deltaTime;
			else
				position += viewVector * (float)m_dSpeed * (float)deltaTime;

			//	 Constrain the position
			Constrain();
			// Update the target
			target = position + viewVector;
			return true;
		}
	}
	else
	{
		ghostPosition -= viewVector * (float)m_dSpeed * (float)deltaTime;
		////	 Constrain the position
		//Constrain();
		//// Update the target
		//target = position + viewVector;

		if (!EntityManager::GetInstance()->CheckAABBCollision(ghostMinAABB, ghostMaxAABB, ghostPosition))
		{
			position -= viewVector * (float)m_dSpeed * (float)deltaTime;
			//	 Constrain the position
			Constrain();
			// Update the target
			target = position + viewVector;
			return true;
		}
	}

	return false;
}
// Detect and process left / right movement on the controller
bool CPlayerInfo::Move_LeftRight(const float deltaTime, const bool direction, const float speedMultiplier)
{
	/*The logic of this is sending a ghost character to move first, if it collides with an object, do not update the actual player position.*/
	/*Ghost Position*/
	Vector3 ghostPosition(this->GetPos());
	Vector3 ghostMinAABB(this->GetMinAABB());
	Vector3 ghostMaxAABB(this->GetMaxAABB());

	Vector3 viewVector = target - position;
	Vector3 rightUV;
	if (direction)
	{
		rightUV = (viewVector.Normalized()).Cross(up);
		rightUV.y = 0;
		rightUV.Normalize();

		ghostPosition -= rightUV * (float)m_dSpeed * deltaTime;
		if (!EntityManager::GetInstance()->CheckAABBCollision(ghostMinAABB, ghostMaxAABB, ghostPosition))
		{
			position -= rightUV * (float)m_dSpeed * deltaTime;
			//	 Constrain the position
			Constrain();
			// Update the target
			target = position + viewVector;
			return true;
		}
	}
	else
	{
		rightUV = (viewVector.Normalized()).Cross(up);
		rightUV.y = 0;
		rightUV.Normalize();
		ghostPosition += rightUV * (float)m_dSpeed * deltaTime;

		if (!EntityManager::GetInstance()->CheckAABBCollision(ghostMinAABB, ghostMaxAABB, ghostPosition))
		{
			position += rightUV * (float)m_dSpeed * deltaTime;
			//	 Constrain the position
			Constrain();
			// Update the target
			target = position + viewVector;
			return true;
		}
	}
	return false;
}

// Detect and process look up / down on the controller
bool CPlayerInfo::Look_UpDown(const float deltaTime, const bool direction, const float speedMultiplier)
{
	if (speedMultiplier == 0.0f)
		return false;

	Vector3 viewUV = (target - position).Normalized();
	Vector3 rightUV;

	if (direction)
	{
		float pitch = (float)-(m_dSpeed * speedMultiplier * (float)deltaTime);
		pitch = Math::Clamp(pitch, theMinPitch, theMaxPitch);
		rightUV = viewUV.Cross(up);
		rightUV.y = 0;
		rightUV.Normalize();
		up = rightUV.Cross(viewUV).Normalized();

		Mtx44 rotation;
		rotation.SetToRotation(pitch, rightUV.x, rightUV.y, rightUV.z);
		if ((rotation*viewUV).y > 0.8 && pitch > 0)
			return false;
		else if ((rotation*viewUV).y < -0.8 && pitch < 0)
			return false;
		viewUV = rotation * viewUV;
		target = position + viewUV;
	}
	else
	{
		float pitch = (float)(m_dSpeed * speedMultiplier * (float)deltaTime);
		pitch = Math::Clamp(pitch, theMinPitch, theMaxPitch);
		rightUV = viewUV.Cross(up);
		rightUV.y = 0;
		rightUV.Normalize();
		up = rightUV.Cross(viewUV).Normalized();

		Mtx44 rotation;
		rotation.SetToRotation(pitch, rightUV.x, rightUV.y, rightUV.z);
		if ((rotation*viewUV).y > 0.8 && pitch > 0)
			return false;
		else if ((rotation*viewUV).y < -0.8 && pitch < 0)
			return false;
		viewUV = rotation * viewUV;
		target = position + viewUV;
	}

	return true;
}
// Detect and process look left / right on the controller
bool CPlayerInfo::Look_LeftRight(const float deltaTime, const bool direction, const float speedMultiplier)
{
	if (speedMultiplier == 0.0f)
		return false;

	Vector3 viewUV = (target - position).Normalized();
	Vector3 rightUV;

	if (direction)
	{
		float yaw = (float)-m_dSpeed * speedMultiplier * (float)deltaTime;
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		viewUV = rotation * viewUV;
		target = position + viewUV;
		rightUV = viewUV.Cross(up);
		rightUV.y = 0;
		rightUV.Normalize();
		up = rightUV.Cross(viewUV).Normalized();
	}
	else
	{
		float yaw = (float)m_dSpeed * speedMultiplier * (float)deltaTime;
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		viewUV = rotation * viewUV;
		target = position + viewUV;
		rightUV = viewUV.Cross(up);
		rightUV.y = 0;
		rightUV.Normalize();
		up = rightUV.Cross(viewUV).Normalized();
	}

	return true;
}

// Stop sway
bool CPlayerInfo::StopSway(const float deltaTime)
{
	m_bCameraSwayDirection = false;
	m_fCameraSwayAngle = 0.0f;
	up = Vector3(0.0f, 1.0f, 0.0f);
	return true;
}

// Reload current weapon
bool CPlayerInfo::ReloadWeapon(void)
{
	if (weaponManager[m_iCurrentWeapon])
	{
		weaponManager[m_iCurrentWeapon]->Reload();
		return true;
	}
	return false;
}

// Change current weapon
bool CPlayerInfo::ChangeWeapon(void)
{
	if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) != m_iCurrentWeapon)
	{
		if ((MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) >= 0) &&
			(MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) < m_iNumOfWeapon))
		{
			m_iCurrentWeapon = MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET);
		}
	}

	//if (m_iCurrentWeapon < m_iNumOfWeapon)
	//	++m_iCurrentWeapon;

	//if (m_iCurrentWeapon == m_iNumOfWeapon)
	//	m_iCurrentWeapon = 0;
	return true;
}

// Get Current Weapon
int CPlayerInfo::GetWeapon(void) const
{
	return m_iCurrentWeapon;
}

// Discharge Primary Weapon
bool CPlayerInfo::DischargePrimaryWeapon(const float deltaTime)
{
	//if (primaryWeapon)
	//	primaryWeapon->Discharge(position, target, this);

	if (weaponManager[m_iCurrentWeapon])
	{
		/*Get the direction as a magnitude; moves spawning point to front of camera.*/
		Vector3 newPosition = target - position;
		/*Increase the position even further.*/
		newPosition *= 3.f;
		/*Create a new direction based on this new position.*/
		Vector3 targetVector((newPosition.x * 2.f) + position.x, (newPosition.y * 2.f) + position.y, (newPosition.z * 2.f) + position.z);
		/*Add the additional distance with original position.*/
		newPosition += position;

		weaponManager[m_iCurrentWeapon]->Discharge(newPosition, targetVector, this);
		return true;
	}

	return false;
}

// Discharge Secondary Weapon
bool CPlayerInfo::DischargeSecondaryWeapon(const float deltaTime)
{
	if (secondaryWeapon)
	{
		/*Get the direction as a magnitude; moves spawning point to front of camera.*/
		Vector3 newPosition = target - position;
		/*Increase the position even further.*/
		newPosition *= 3.f;
		/*Create a new direction based on this new position.*/
		Vector3 targetVector((newPosition.x * 2.f) + position.x, (newPosition.y * 2.f) + position.y, (newPosition.z * 2.f) + position.z);
		/*Add the additional distance with original position.*/
		newPosition += position;

		secondaryWeapon->Discharge(position, target, this);
		return true;
	}

	return false;
}
#include "../CameraEffects/CameraEffects.h"

// Constrain the position within the borders
void CPlayerInfo::Constrain(void)
{
	// Constrain player within the boundary
	if (position.x > maxBoundary.x - 5.f)
	{
		position.x = maxBoundary.x - 5.f;
		CCameraEffects::GetInstance()->SetStatus_BloodScreen(true);
	}
	if (position.y > maxBoundary.y - 1.f)
	{
		position.y = maxBoundary.y - 1.f;
		m_dJumpSpeed = 0.0;
		m_bJumpUpwards = false;
		m_dFallSpeed = 0.0;
		m_bFallDownwards = true;
		m_dElapsedTime = 0.0;
	}
	if (position.z > maxBoundary.z - 5.f)
	{
		position.z = maxBoundary.z - 5.f;
		CCameraEffects::GetInstance()->SetStatus_BloodScreen(true);
	}
	if (position.x < minBoundary.x + 5.f)
	{
		position.x = minBoundary.x + 5.f;
		CCameraEffects::GetInstance()->SetStatus_BloodScreen(true);
	}

	if (position.y < minBoundary.y + 1.f)
		position.y = minBoundary.y + 1.f;

	if (position.z < minBoundary.z + 5.f)
	{
		position.z = minBoundary.z + 5.f;
		CCameraEffects::GetInstance()->SetStatus_BloodScreen(true);
	}

	// if the player is not jumping nor falling, then adjust his y position
	if ((m_bJumpUpwards == false) && (m_bFallDownwards == false))
	{
		// if the y position is not equal to terrain height at that position, 
		// then update y position to the terrain height
		if (position.y != m_pTerrain->GetTerrainHeight(position))
			position.y = m_pTerrain->GetTerrainHeight(position);

		Vector3 viewDirection = target - position;
		switch (theCurrentPosture)
		{
		case STAND:
			position.y = m_pTerrain->GetTerrainHeight(Vector3(position.x, 0.0f, position.z));
			target = position + viewDirection;
			break;
		case CROUCH:
			position.y = m_pTerrain->GetTerrainHeight(Vector3(position.x, 0.0f, position.z)) - 5.0f;
			target.y = position.y + viewDirection.y;
			break;
		case PRONE:
			position.y = m_pTerrain->GetTerrainHeight(Vector3(position.x, 0.0f, position.z)) - 8.0f;
			target.y = position.y + viewDirection.y;
			break;
		default:
			break;
		}
	}
}

void CPlayerInfo::AttachCamera(FPSCamera* _cameraPtr)
{
	attachedCamera = _cameraPtr;
}

void CPlayerInfo::DetachCamera()
{
	attachedCamera = nullptr;
}

void CPlayerInfo::ChangePosture()
{
	theCurrentPosture = (CURRENT_POSTURE)(theCurrentPosture + 1);
	if (theCurrentPosture == NUM_POSTURE)
		theCurrentPosture = STAND;

	Vector3 viewDirection = target - position;
	switch (theCurrentPosture)
	{
	case STAND:
		position.y = m_pTerrain->GetTerrainHeight(Vector3(position.x, 0.0f, position.z));
		target = position + viewDirection;
		break;
	case CROUCH:
		position.y = m_pTerrain->GetTerrainHeight(Vector3(position.x, 0.0f, position.z)) - 5.0f;
		target = position + viewDirection;
		break;
	case PRONE:
		position.y = m_pTerrain->GetTerrainHeight(Vector3(position.x, 0.0f, position.z)) - 8.0f;
		target = position + viewDirection;
		break;
	default:
		break;
	}
}

CWeaponInfo* CPlayerInfo::getPrimaryWeapon(void)
{
	return primaryWeapon;
}

CWeaponInfo ** CPlayerInfo::getWeaponManager(void)
{
	return weaponManager;
}

//void CPlayerInfo::setHealth(int _health)
//{
//	health = _health;
//}

//int CPlayerInfo::getHealth(void)
//{
//	return health;
//}

void CPlayerInfo::setTookDamage(bool _tookDamage)
{
	tookDamage = _tookDamage;
}

bool CPlayerInfo::getTookDamage(void)
{
	return tookDamage;
}

void CPlayerInfo::setScore(float _score)
{
	score = _score;
}

float CPlayerInfo::getScore(void)
{
	return score;
}

void CPlayerInfo::setKO_Count(float _KO_Count)
{
	KO_Count = _KO_Count;
}

int CPlayerInfo::getKO_Count(void)
{
	return KO_Count;
}

void CPlayerInfo::renderPlayerHealth()
{
	Mtx44 ortho;
	ortho.SetToOrtho(-80, 80, -60, 60, -10, 10);
	Mtx44& projectionStack = GraphicsManager::GetInstance()->GetProjectionMatrix();
	projectionStack.SetToOrtho(-80, 80, -60, 60, -10, 10);
	Mtx44& viewStack = GraphicsManager::GetInstance()->GetViewMatrix();
	viewStack.SetToIdentity();
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(20.f, 40.f, 0.f);
	modelStack.Scale((this->getAttribute(CAttributes::TYPE_HEALTH) * Application::GetInstance().GetWindowWidth()) * 0.0005f, Application::GetInstance().GetWindowHeight() * 0.01f, 1.f);
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("cube"));
	modelStack.PopMatrix();
}
