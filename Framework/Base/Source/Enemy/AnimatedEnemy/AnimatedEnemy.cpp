#include "AnimatedEnemy.h"
#include "MeshBuilder.h"
#include "RenderHelper.h"
#include "GraphicsManager.h"
#include"../../EntityManager.h"

CAnimatedEnemy::CAnimatedEnemy(Mesh* _core,
	Mesh* _leftArm,
	Mesh* _rightArm,
	Mesh* _leftLeg,
	Mesh* _rightLeg,
	Mesh* _head)
	:CEnemy3D(NULL)
	, minAlertBoundary(Vector3(-100.f, -50.f, -100.f))
	, maxAlertBoundary(Vector3(100.f, 50.f, 100.f))
	, m_fElapsedTimeBeforeUpdate(3.0f)
	, state(PATROL)
	, leftArmRotation(0.f)
	, rightArmRotation(0.f)
	, leftLegRotation(0.f)
	, rightLegRotation(0.f)
	, bArmRotationPositive(true)
	, bLegRotationPositive(true)
{
	this->modelMesh[0] = _core;
	this->modelMesh[1] = _leftArm;
	this->modelMesh[2] = _rightArm;
	this->modelMesh[3] = _leftLeg;
	this->modelMesh[4] = _rightLeg;
	this->modelMesh[5] = _head;
}

CAnimatedEnemy::~CAnimatedEnemy()
{
}

void CAnimatedEnemy::Init(void)
{
	defaultPosition.Set(0, 0, 10);
	defaultTarget.Set(0, 0, 0);
	defaultUp.Set(0, 1, 0);

	position.Set(position.x, position.y, position.z);
	target.Set(10.0f, 0.0f, 450.0f);
	up.Set(0.0f, 1.0f, 0.0f);

	// Set Boundary
	maxBoundary.Set(1, 1, 1);
	minBoundary.Set(-1, -1, -1);

	// Set speed
	m_dSpeed = 10.0;

	/*Set State*/
	state = IDLE;

	/*Init rotation values for animation*/
	rotationSetZero();
	bArmRotationPositive = true;
	bLegRotationPositive = true;

	// Add to EntityManager
	EntityManager::GetInstance()->AddEntity(this);

}
//
//void CAnimatedEnemy::Init(void)
//{
//	// Set the default values
//	defaultPosition.Set(0, 0, 10);
//	defaultTarget.Set(0, 0, 0);
//	defaultUp.Set(0, 1, 0);
//
//	// Set the current values
//	//position.Set(10.0f, 0.0f, 0.0f);
//	//target.Set(10.0f, 0.0f, 450.0f);
//	//up.Set(0.0f, 1.0f, 0.0f);
//	position.Set(position.x, position.y, position.z);
//	cout << "Init Position : " << position << endl;
//	target.Set(10.0f, 0.0f, 450.0f);
//	up.Set(0.0f, 1.0f, 0.0f);
//
//	// Set Boundary
//	maxBoundary.Set(1, 1, 1);
//	minBoundary.Set(-1, -1, -1);
//
//	// Set speed
//	m_dSpeed = 10.0;
//
//	/*Set State*/
//	state = IDLE;
//
//	// Add to EntityManager
//	EntityManager::GetInstance()->AddEntity(this);
//
//}
//
// Reset this player instance to default
void CAnimatedEnemy::Reset(void)
{
	// Set the current values to default values
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}
//
//// Set position
//void CAnimatedEnemy::SetPos(const Vector3& pos)
//{
//	position = pos;
//}
//
//// Set target
//void CAnimatedEnemy::SetTarget(const Vector3& target)
//{
//	this->target = target;
//}
//// Set Up
//void CAnimatedEnemy::SetUp(const Vector3& up)
//{
//	this->up = up;
//}
//// Set the boundary for the player info
//void CAnimatedEnemy::SetBoundary(Vector3 max, Vector3 min)
//{
//	maxBoundary = max;
//	minBoundary = min;
//}
//// Set the terrain for the player info
//void CAnimatedEnemy::SetTerrain(GroundEntity* m_pTerrain)
//{
//	if (m_pTerrain != NULL)
//	{
//		this->m_pTerrain = m_pTerrain;
//
//		SetBoundary(this->m_pTerrain->GetMaxBoundary(), this->m_pTerrain->GetMinBoundary());
//	}
//}
//
//// Get position
//Vector3 CAnimatedEnemy::GetPos(void) const
//{
//	return position;
//}
//
//// Get target
//Vector3 CAnimatedEnemy::GetTarget(void) const
//{
//	return target;
//}
//// Get Up
//Vector3 CAnimatedEnemy::GetUp(void) const
//{
//	return up;
//}
//// Get the terrain for the player info
//GroundEntity* CAnimatedEnemy::GetTerrain(void)
//{
//	return m_pTerrain;
//}
/*
Updates rotational values for arms and legs
*/
void CAnimatedEnemy::UpdatesRotationValue(double dt)
{
	/*
	Checks current rotational values
	*/

	/*
	Limits arm rotation between -45.f - 45.f degrees
	*/
	if (leftArmRotation <= -45.f)
	{
		bArmRotationPositive = true;
	}
	else if (leftArmRotation >= 45.f)
	{
		bArmRotationPositive = false;
	}
	/*
	Limits leg rotation between -30.f - 30.f degrees
	*/
	if (leftLegRotation <= -30.f)
	{
		bLegRotationPositive = true;
	}
	else if (leftLegRotation >= 30.f)
	{
		bLegRotationPositive = false;
	}

	float rotationSpeed = 100.f; //Speed of rotation

	/*
	updating of rotation values
	*/
	/*
	Left arm rotates the opposite direction as right arm
	*/
	if (bArmRotationPositive)
	{
		leftArmRotation += rotationSpeed*dt;
		rightArmRotation -= rotationSpeed*dt;
	}
	else
	{
		leftArmRotation -= rotationSpeed*dt;
		rightArmRotation += rotationSpeed*dt;
	}
	/*
	Left leg rotates the opposite direction as right leg
	*/
	if (bLegRotationPositive)
	{
		leftLegRotation += rotationSpeed*dt;
		rightLegRotation -= rotationSpeed*dt;
	}
	else
	{
		leftLegRotation -= rotationSpeed*dt;
		rightLegRotation += rotationSpeed*dt;
	}
}
/*
Set rotation values to zero
*/
void CAnimatedEnemy::rotationSetZero()
{
	leftArmRotation = 0.f;
	rightArmRotation = 0.f;
	leftLegRotation = 0.f;
	rightLegRotation = 0.f;
}
// Update
void CAnimatedEnemy::Update(double dt)
{
	_enemy = this;
	//cout << "Enemy Min: " << position + minAABB << endl;
	//cout << "Enemy Max: " << position + maxAABB << endl;
	if (!pathFindingMode)
	{
		if (checkCollision())
		{
			position = previousPosition;

#ifdef _DEBUG
			cout << "COLLIDED" << endl;
#endif
			pathFindingMode = true;
		}
		else
		{
			previousPosition = position;
#ifdef _DEBUG
			cout << "NO COLLISION" << endl; 
#endif
		}

		//cout << "Displacement: " << (CPlayerInfo::GetInstance()->GetPos() - this->GetPos()).LengthSquared() << endl;
		if ((CPlayerInfo::GetInstance()->GetPos() - this->GetPos()).LengthSquared() < 100.f * 100.f && checkInsideBoundary(minAlertBoundary, maxAlertBoundary))
			state = ALERT;
		else
			state = PATROL;

		switch (state)
		{
		//case PATROL:
		//{
		//	Vector3 displacement(waypoint[waypointToGo] - this->GetPos());
		//	//position += displacement.Normalized() * (float)dt * 20.f;
		//	try
		//	{
		//		position += displacement.Normalized() * (float)dt * 20.f;
		//	}
		//	catch (exception e)
		//	{
		//		/*Divide By Zero does no harm to this situation because it will only happen when there is only 1 waypoint and the enemy is on top, thus, can be left unresolved.*/
		//	}
		//	if (displacement.LengthSquared() <= 20.f)
		//		waypointToGo = ((waypointToGo == waypoint.size() - 1) ? 0 : ++waypointToGo);
		//	break;
		//}
			case ALERT:
			{
				//cout << "ALERT FIND" << endl;
				Vector3 positionWithoutY(CPlayerInfo::GetInstance()->GetPos().x, -10.f, CPlayerInfo::GetInstance()->GetPos().z);
				Vector3 displacement(positionWithoutY - this->GetPos());

				/*Using comparison of magnitude to mimic the real world environment where if the a person just left you not long ago, you will be more alerted and prepare if the person will return.*/
				if (displacement.LengthSquared() > scale.LengthSquared() * 5.f)
					position += displacement.Normalized() * (float)dt * getAttribute(CAttributes::ATTRIBUTE_TYPES::TYPE_SPEED);

				break;
			}
		}
	}
	else
	{
		//static bool once = false;
		//if (!once)
		//{
		//	once = true;
		if (!scanned)
		{
			positionWithoutY.Set(position.x, -10.f, position.z);
			targetObjectPosition = CPlayerInfo::GetInstance()->GetPos();
			updatePathfinding(positionWithoutY, scale, dt);
			nearestPosition = nearestPath();
			/*cout << "Nearest Path: " << nearestPosition << endl;
			cout << "Position: " << position << endl;*/
			scanned = true;
			try
			{
				directionToGo = (nearestPosition - position).Normalized();
			}
			catch (exception e)
			{
				pathFindingMode = false;
				scanned = false;
				nearestPosition.SetZero();
				position = previousPosition;
			}
		}
		//cout << "Nearest Position: " << nearestPosition << endl;
		//cout << "Displacement to Nearest Path: " << (nearestPosition - Vector3(position.x, -10.f, position.z)).LengthSquared() << endl;
		//if ((nearestPath() - position).LengthSquared() > 0.1f)
		//	position += (nearestPath() - position).Normalized() * (float)dt * 20.f;
		if ((nearestPosition - Vector3(position.x, -10.f, position.z)).LengthSquared() >  0.1f)
			position += directionToGo * (float)dt * getAttribute(CAttributes::ATTRIBUTE_TYPES::TYPE_SPEED);
		else
		{
			while (path.size() > 0)
			{
				path.pop_back();
				//cout << "REMOVING" << endl;
			}
			pathFindingMode = false;
			scanned = false;
			//nearestPath().SetZero();
			nearestPosition.SetZero();
			previousPosition = position;
		}
		//cout << "PATH FIND" << endl;
	}
	//}
	////CEnemy3D::Update(dt);
	//if (checkInsideBoundary(minAlertBoundary, maxAlertBoundary))
	//	state = ALERT;
	//else
	//	state = IDLE;
	//switch (state)
	//{
	//	case IDLE:
	//	{
	//		Vector3 displacement(defaultPosition - this->GetPos());
	//		if (displacement.LengthSquared() > 0.f)
	//			position += displacement.Normalized() * (float)dt * 20.f;
	//		break;
	//	}
	//	case ALERT:
	//	{
	//		Vector3 positionWithoutY(CPlayerInfo::GetInstance()->GetPos().x, -10.f, CPlayerInfo::GetInstance()->GetPos().z);
	//		Vector3 displacement(positionWithoutY - this->GetPos());
	//		if (displacement.LengthSquared() > scale.LengthSquared() * 5.f)
	//			position += displacement.Normalized() * (float)dt * 20.f;
	//		break;
	//	}
	//}
	if (state != PATROL)
		UpdatesRotationValue(dt);
	else
		rotationSetZero();
}

// Constrain the position within the borders
//void CAnimatedEnemy::Constrain(void)
//{
//	// Constrain player within the boundary
//	if (position.x > maxBoundary.x - 1.0f)
//		position.x = maxBoundary.x - 1.0f;
//	if (position.z > maxBoundary.z - 1.0f)
//		position.z = maxBoundary.z - 1.0f;
//	if (position.x < minBoundary.x + 1.0f)
//		position.x = minBoundary.x + 1.0f;
//	if (position.z < minBoundary.z + 1.0f)
//		position.z = minBoundary.z + 1.0f;
//
//	/*If Y-position is not equal to terrain height at position, 
//	update Y-position to the terrain height*/
//	/*Commented out to prevent the Y from changing.*/
//	//if (position.y != m_pTerrain->GetTerrainHeight(position))
//	//	position.y = m_pTerrain->GetTerrainHeight(position);
//}

// Render
void CAnimatedEnemy::Render(void)
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	position.y = -10.f + scale.y * 3.8f; //Robot pieces together from the torso, hence displacement needed based on scale 
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	if (state == CEnemy3D::AI_STATE::IDLE)
	{
		try {
			angleToFace = Math::RadianToDegree(atan2(defaultPosition.x - this->position.x, defaultPosition.z - this->position.z));
		}

		catch (string Error)
		{
			cout << "Divide by Zero" << endl;
		}
	}
	else if (pathFindingMode)
	{
		Vector3 displacement(/*nearestPath()*/nearestPosition - this->GetPos());
		angleToFace = Math::RadianToDegree(atan2(displacement.x, displacement.z));
	}
	else if (state == CEnemy3D::AI_STATE::ALERT)
	{
		try {
			angleToFace = Math::RadianToDegree(atan2(CPlayerInfo::GetInstance()->GetPos().x - this->position.x, CPlayerInfo::GetInstance()->GetPos().z - this->position.z));
		}

		catch (string Error)
		{
			cout << "Divide by Zero" << endl;
		}
	}
	modelStack.Rotate(angleToFace, 0.f, 1.f, 0.f);

	for (int i = 1; i < 6; ++i)
	{
		modelStack.PushMatrix();
		/*if (state == ALERT)
		{
			if (!getPlayerProperty())
			{
				if (whoCloser == PLAYER)
					angleToFace = Math::RadianToDegree(atan2(CPlayerInfo::GetInstance()->GetPos().x - this->position.x, CPlayerInfo::GetInstance()->GetPos().z - this->position.z));
				else if (whoCloser == ENEMY && returnNearestEnemy() != nullptr)
					angleToFace = Math::RadianToDegree(atan2(returnNearestEnemy()->GetPos().x - this->position.x, returnNearestEnemy()->GetPos().z - this->position.z));
			}
			else if (returnNearestEnemy() != nullptr)
				angleToFace = Math::RadianToDegree(atan2(returnNearestEnemy()->GetPos().x - this->position.x, returnNearestEnemy()->GetPos().z - this->position.z));
			modelStack.Rotate(angleToFace, 0.f, 1.f, 0.f);
		}
		if (state == RECOVERY)
		{
			angleToFace += static_cast<float>(getPortableDT() * 20.f);
			if (angleToFace >= 90.f)
				angleToFace = 90.f;
			modelStack.Rotate(-angleToFace, 1.f, 0.f, 0.f);
		}*/
		switch (i)
		{
		case 1:
			modelStack.Translate(scale.x * 0.6f, 0, 0);
			modelStack.Rotate(leftArmRotation, 1, 0, 0);
			break;
		case 2:
			modelStack.Translate(- scale.x * 0.6f, 0, 0);
			modelStack.Rotate(rightArmRotation, 1, 0, 0);
			break;
		case 3:
			modelStack.Translate(scale.x * 0.4f, -scale.y * 0.8f, 0);
			modelStack.Rotate(leftLegRotation, 1, 0, 0);
			break;
		case 4:
			modelStack.Translate(-scale.x * 0.38f, -scale.y * 0.8f, 0);
			modelStack.Rotate(rightLegRotation, 1, 0, 0);
			break;
		case 5:
			modelStack.Translate(0, scale.y * 0.3f, 0);
			break;
		}
		
		modelStack.Scale(scale.x, scale.y, scale.z);
		if (!light)
			RenderHelper::RenderMesh(modelMesh[i]);
		else
			RenderHelper::RenderMeshWithLight(modelMesh[i]);
		modelStack.PopMatrix();
	}

	modelStack.Scale(scale.x, scale.y, scale.z);
	if (!light)
		RenderHelper::RenderMesh(modelMesh[0]);
	else
		RenderHelper::RenderMeshWithLight(modelMesh[0]);
	modelStack.PopMatrix();

	if (getAttribute(CAttributes::TYPE_HEALTH) > 0.f)
		renderHealthBar();

	if (pathFindingMode)
	{
		for (vector<Vector3>::iterator it = path.begin(); it != path.end(); ++it)
		{
			Vector3 _position = (Vector3)*it;

			if (_position == Vector3(/*nearestPath().x*/nearestPosition.x, -10.f, /*nearestPath().z*/nearestPosition.z))
				continue;
			//cout << _position <<  " and " << nearestPath() << endl;

			modelStack.PushMatrix();
			modelStack.Translate(_position.x, _position.y, _position.z);
			RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("PATH"));
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix();
		modelStack.Translate(nearestPosition.x, -10.f, nearestPosition.z);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("ENEMY"));
		modelStack.PopMatrix();
	}
}
//
//void CAnimatedEnemy::setHealth(int _health)
//{
//	health = _health;
//}
//
//int CAnimatedEnemy::getHealth(void)
//{
//	return health;
//}
//
//void CAnimatedEnemy::setState(AI_STATE _state)
//{
//	state = _state;
//}
//
//int CAnimatedEnemy::getState(void)
//{
//	return state;
//}
//
//bool CAnimatedEnemy::checkInsideBoundary(Vector3 minBoundary, Vector3 maxBoundary)
//{
//	Vector3 boundaryMin = minBoundary + GetPos();
//	Vector3 boundaryMax = maxBoundary + GetPos();
//
//	if (!this->getPlayerProperty())
//	{
//		//cout << "Enemy Address: " << this << endl;
//		if (EntityManager::GetInstance()->returnEnemy().size() < 2)
//		{
//			//cout << "Lesser than Two: " << this << endl;
//			Vector3 playerMin = CPlayerInfo::GetInstance()->GetMinAABB() + Vector3(CPlayerInfo::GetInstance()->GetPos().x, -5.f, CPlayerInfo::GetInstance()->GetPos().z);
//			Vector3 playerMax = CPlayerInfo::GetInstance()->GetMaxAABB() + Vector3(CPlayerInfo::GetInstance()->GetPos().x, -5.f, CPlayerInfo::GetInstance()->GetPos().z);
//
//			whoCloser = PLAYER;
//			if ((boundaryMin.x < playerMax.x && boundaryMax.x >playerMin.x) &&
//				(boundaryMin.y < playerMax.y && boundaryMax.y >playerMin.y) &&
//				(boundaryMin.z < playerMax.z && boundaryMax.z >playerMin.z))
//				return true;
//			else
//				return false;
//		}
//	
//		else if ((returnNearestEnemy()->GetPos() - this->GetPos()).LengthSquared() < (CPlayerInfo::GetInstance()->GetPos() - this->GetPos()).LengthSquared() && returnNearestEnemy()->getPlayerProperty())
//		{
//			//cout << "Enemy Closer Than Player: " << this << endl;
//			whoCloser = ENEMY;
//			if (returnNearestEnemy() == nullptr)
//				return false;
//			//cout << "Nearest Enemy: " << (returnNearestEnemy()->GetPos() - this->GetPos()).LengthSquared() << " Player: " << (CPlayerInfo::GetInstance()->GetPos() - this->GetPos()).LengthSquared() << " " << returnNearestEnemy()->getPlayerProperty()  <<endl;
//			//cout << "I AM HERE" << endl;
//			Vector3 objectMin = returnNearestEnemy()->GetMinAABB() + Vector3(returnNearestEnemy()->GetPos().x, -5.f, returnNearestEnemy()->GetPos().z);
//			Vector3 objectMax = returnNearestEnemy()->GetMaxAABB() + Vector3(returnNearestEnemy()->GetPos().x, -5.f, returnNearestEnemy()->GetPos().z);
//
//			if ((boundaryMin.x < objectMax.x && boundaryMax.x >objectMin.x) &&
//				(boundaryMin.y < objectMax.y && boundaryMax.y >objectMin.y) &&
//				(boundaryMin.z < objectMax.z && boundaryMax.z >objectMin.z))
//
//			{
//				//cout << "ENEMY INSIDE BOUNDARY" << endl;
//				return true;
//			}
//
//			else
//				return false;
//		}
//		else
//		{
//			//cout << "Player Closer than Enemy" << this << endl;
//			//cout << "Nearest Enemy: " << (returnNearestEnemy()->GetPos() - this->GetPos()).LengthSquared() << " Player: " << (CPlayerInfo::GetInstance()->GetPos() - this->GetPos()).LengthSquared() << " " << returnNearestEnemy()->getPlayerProperty()<<  endl;
//			Vector3 playerMin = CPlayerInfo::GetInstance()->GetMinAABB() + Vector3(CPlayerInfo::GetInstance()->GetPos().x, -5.f, CPlayerInfo::GetInstance()->GetPos().z);
//			Vector3 playerMax = CPlayerInfo::GetInstance()->GetMaxAABB() + Vector3(CPlayerInfo::GetInstance()->GetPos().x, -5.f, CPlayerInfo::GetInstance()->GetPos().z);
//
//			whoCloser = PLAYER;
//			if ((boundaryMin.x < playerMax.x && boundaryMax.x >playerMin.x) &&
//				(boundaryMin.y < playerMax.y && boundaryMax.y >playerMin.y) &&
//				(boundaryMin.z < playerMax.z && boundaryMax.z >playerMin.z))
//				return true;
//			else
//				return false;
//		}
//	}
//	else
//	{
//		if (returnNearestEnemy() == nullptr)
//			return false;
//
//		whoCloser = NONE;
//		Vector3 objectMin = returnNearestEnemy()->GetMinAABB() + Vector3( returnNearestEnemy()->GetPos().x, -5.f,  returnNearestEnemy()->GetPos().z);
//		Vector3 objectMax = returnNearestEnemy()->GetMaxAABB() + Vector3( returnNearestEnemy()->GetPos().x, -5.f,  returnNearestEnemy()->GetPos().z);
//
//		if ((boundaryMin.x < objectMax.x && boundaryMax.x >objectMin.x) &&
//			(boundaryMin.y < objectMax.y && boundaryMax.y >objectMin.y) &&
//			(boundaryMin.z < objectMax.z && boundaryMax.z >objectMin.z))
//
//			return true;
//
//		else
//			return false;
//	}
//}
//
//CAnimatedEnemy * CAnimatedEnemy::returnNearestEnemy(void)
//{
//	CAnimatedEnemy* enemy = nullptr;
//	float nearestDistance = 0.f;
//	for (list<CAnimatedEnemy*>::iterator it = EntityManager::GetInstance()->returnEnemy().begin(); it != EntityManager::GetInstance()->returnEnemy().end(); ++it)
//	{
//		if (this == (*it))
//			continue;
//
//		if ((*it)->getPlayerProperty() && this->getPlayerProperty())
//			continue;
//
//		/*cout << "From " << this << " aim " << (CAnimatedEnemy*)*it << endl;*/
//		nearestDistance = ((*it)->GetPos() - this->GetPos()).LengthSquared();
//		enemy = (CAnimatedEnemy*)*it;
//		break;
//	}
//
//	if (enemy == nullptr)
//		return enemy;
//
//	for (list<CAnimatedEnemy*>::iterator it = EntityManager::GetInstance()->returnEnemy().begin(); it != EntityManager::GetInstance()->returnEnemy().end(); ++it)
//	{
//		/*enemy = (CAnimatedEnemy*)*it;*/
//		/*cout << "Next From " << this << " aim " << (CAnimatedEnemy*)*it << " PLAYER? " << (*it)->getPlayerProperty() << endl;*/
//		if ((*it)->getPlayerProperty() && this->getPlayerProperty())
//			continue;
//
//		if (((*it)->GetPos() - this->GetPos()).LengthSquared() < nearestDistance && this != (*it))
//		{
//			enemy = (CAnimatedEnemy*)*it;
//			nearestDistance = ((*it)->GetPos() - this->GetPos()).LengthSquared();
//		}
//		else
//			continue;
//	}
//	return enemy;
//}
//
//void CAnimatedEnemy::setAlertBoundary(Vector3 _minAlertBoundary, Vector3 _maxAlertBoundary)
//{
//	minAlertBoundary = _minAlertBoundary;
//	maxAlertBoundary = _maxAlertBoundary;
//}
//
//Vector3 CAnimatedEnemy::getMinAlertBoundary(void)
//{
//	return minAlertBoundary;
//}
//
//Vector3 CAnimatedEnemy::getMaxAlertBoundary(void)
//{
//	return maxAlertBoundary;
//}
//
//void CAnimatedEnemy::setPortableDT(double _portableDT)
//{
//	portableDT = _portableDT;
//}
//
//double CAnimatedEnemy::getPortableDT(void)
//{
//	return portableDT;
//}
//
////void CAnimatedEnemy::setAttributes(ATTRIBUTES _attributes)
////{
////	attributes = _attributes;
////}
////
////float CAnimatedEnemy::getAttributes(ATTRIBUTE_TYPES type)
////{
////	if (type == ENEMY_HEALTH)
////		return attributes.HEALTH;
////
////	else if (type == ENEMY_ATTACK)
////		return attributes.ATTACK;
////
////	else if (type == ENEMY_DEFENSE)
////		return attributes.DEFENSE;
////}
//
//void CAnimatedEnemy::setPlayerProperty(bool _playerProperty)
//{
//	playerProperty = _playerProperty;
//}
//
//bool CAnimatedEnemy::getPlayerProperty(void)
//{
//	return playerProperty;
//}
//
//void CAnimatedEnemy::setShootDelay(float _shootDelay)
//{
//	shootDelay = _shootDelay;
//}
//
//float CAnimatedEnemy::getShootDelay(void)
//{
//	return shootDelay;
//}
//
//void CAnimatedEnemy::renderHealthBar(void)
//{
//	/*Calculate the displacement from enemy to player.*/
//	Vector3 displacement(CPlayerInfo::GetInstance()->GetPos() - this->GetPos());
//	/*Calculate the displacement from player to enemy.*/
//	Vector3 furtherDisplacement(this->GetPos() - CPlayerInfo::GetInstance()->GetPos());
//	/*Move the position slightly further away to prevent Z-Axis fighting.*/
//	furtherDisplacement *= 1.01f;
//	/*Get the player position.*/
//	Vector3 playerPosition(CPlayerInfo::GetInstance()->GetPos());
//	/*Add the displacement with player position to bring the further than displacement Vector3 to prevent Z-Axis fighting.*/
//	furtherDisplacement = furtherDisplacement + playerPosition;
//
//	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
//
//	/*Black health bar that depicts the total health of enemy.*/
//	modelStack.PushMatrix();
//	/*Keep the health bar fixed to the left of the enemy.*/
//	modelStack.Translate(furtherDisplacement.x, furtherDisplacement.y + (maxAABB.y - furtherDisplacement.y), furtherDisplacement.z);
//	modelStack.Rotate(Math::RadianToDegree(atan2f(displacement.x, displacement.z)), 0.f, 1.f, 0.f);
//	/*Scale it according to the health left.*/
//	modelStack.Scale(MAX_HEALTH_SCALE, Application::GetInstance().GetWindowHeight() * 0.005f, 0.000001f);
//	MeshBuilder::GetInstance()->GenerateCube("cube", Color(0.f, 0.f, 0.0f), 1.f);
//	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("cube"));
//	modelStack.PopMatrix();
//
//	/*Health Bar above enemy head*/
//	modelStack.PushMatrix();
//	/*Keep the health bar fixed to the left of the enemy.*/
//	modelStack.Translate(position.x/*+ minAABB.x + ((attributes.HEALTH / attributes.MAX_HEALTH) * MAX_HEALTH_SCALE) * 0.5f*/, position.y + (maxAABB.y - position.y), position.z);
//	/*Vector3 displacement(CPlayerInfo::GetInstance()->GetPos() - this->GetPos());*/
//	modelStack.Rotate(Math::RadianToDegree(atan2f(displacement.x, displacement.z)), 0.f, 1.f, 0.f);
//	/*Scale it according to the health left.*/
//	modelStack.Scale((getAttribute(CAttributes::TYPE_HEALTH) / getAttribute(CAttributes::TYPE_MAXHEALTH)) * MAX_HEALTH_SCALE, Application::GetInstance().GetWindowHeight() * 0.005f, 0.000001f);
//
//	/*Set health bar to green colour before damage.*/
//	if (getAttribute(CAttributes::TYPE_HEALTH) / getAttribute(CAttributes::TYPE_MAXHEALTH) == 1)
//		MeshBuilder::GetInstance()->GenerateCube("cube", Color(0.f, 1.f, 0.0f), 1.f);
//
//	/*Set health bar to yellow color when taken damage and is above 20%.*/
//	if (getAttribute(CAttributes::TYPE_HEALTH) / getAttribute(CAttributes::TYPE_MAXHEALTH) > 0.2f && getAttribute(CAttributes::TYPE_HEALTH) / getAttribute(CAttributes::TYPE_MAXHEALTH) < 1.f)
//		MeshBuilder::GetInstance()->GenerateCube("cube", Color(1.f, 1.f, 0.0f), 1.f);
//
//	/*Set health bar to red color when health is 20% and below.*/
//	if (getAttribute(CAttributes::TYPE_HEALTH) / getAttribute(CAttributes::TYPE_MAXHEALTH) <= 0.2f)
//		MeshBuilder::GetInstance()->GenerateCube("cube", Color(1.f, 0.f, 0.0f), 1.f);
//	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("cube"));
//
//	modelStack.PopMatrix();
//}

CAnimatedEnemy* Create::AnimatedEnemy(const std::string& _core,
						const std::string& _leftArm,
						const std::string& _rightArm,
						const std::string& _leftLeg,
						const std::string& _rightLeg,
						const std::string& _Head,
						const Vector3& _position,
						const Vector3& _scale)
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_core);
	if (modelMesh == nullptr)
		return nullptr;
	Mesh* modelMesh2 = MeshBuilder::GetInstance()->GetMesh(_leftArm);
	if (modelMesh2 == nullptr)
		return nullptr;
	Mesh* modelMesh3 = MeshBuilder::GetInstance()->GetMesh(_rightArm);
	if (modelMesh3 == nullptr)
		return nullptr;
	Mesh* modelMesh4 = MeshBuilder::GetInstance()->GetMesh(_leftLeg);
	if (modelMesh4 == nullptr)
		return nullptr;
	Mesh* modelMesh5 = MeshBuilder::GetInstance()->GetMesh(_rightLeg);
	if (modelMesh5 == nullptr)
		return nullptr;
	Mesh* modelMesh6 = MeshBuilder::GetInstance()->GetMesh(_Head);
	if (modelMesh6 == nullptr)
		return nullptr;

	CAnimatedEnemy* result = new CAnimatedEnemy(modelMesh, modelMesh2, modelMesh3, modelMesh4, modelMesh5, modelMesh6);

	//cout << "Position in Create: " << _position << endl;
	result->SetPosition(_position);
	result->SetScale(_scale);
	result->SetCollider(true);
	result->setPlayerProperty(false);
	result->setSpeed(30);
	EntityManager::GetInstance()->AddEnemy(result);
	return result;
}