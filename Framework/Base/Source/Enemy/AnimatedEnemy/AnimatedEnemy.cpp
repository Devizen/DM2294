/*A.I logic done by Lai Wei Jing, Eugene (160540A)*/
/*Binding of A.I Rendering Parts done by Kwong Yu Ming (160853T)*/
/*Tested by Chan Zhi Hao and Lim Guan Sheng, Marcus*/
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
	, m_fElapsedTimeBeforeUpdate(3.0f)
	, state(IDLE)
	, leftArmRotation(0.f)
	, rightArmRotation(0.f)
	, leftLegRotation(0.f)
	, rightLegRotation(0.f)
	, bArmRotationPositive(true)
	, bLegRotationPositive(true)
	, playerProperty(false)
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

// Reset this player instance to default
void CAnimatedEnemy::Reset(void)
{
	// Set the current values to default values
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}

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
	/*If the enemy is not at default position, keep it moving.*/
	if ((defaultPosition - position).LengthSquared() > 250.f)
		UpdatesRotationValue(dt);
	else
		rotationSetZero();

	/*Set positions without Y axis displacement for easier calculations.*/
	Vector3 playerWithoutY(CPlayerInfo::GetInstance()->GetPos().x, -10.f, CPlayerInfo::GetInstance()->GetPos().z);
	Vector3 enemyWithoutY(0.f, 0.f, 0.f);
	if (returnNearestEnemy() != nullptr)
		enemyWithoutY = returnNearestEnemy()->GetPos();
	Vector3 thisWithoutY(position.x, -10.f, position.z);

	_enemy = this;

	/*If enemy is IDLE, check if player or player object stepped into the boundary.*/
	if (state == IDLE && !pathFindingMode)
	{
		//cout << "IN IDLE" << endl;
		if (checkInsideBoundary(getMinAlertBoundary(), getMaxAlertBoundary()))
			state = ALERT;
		else
		{
			/*If player is not in boundary and enemy is not at the default location, move it back to default location.*/
			if (position != defaultPosition)
			{
				if (checkCollision())
				{
					position = previousPosition;
					targetObjectPosition = defaultPosition;
					pathFindingMode = true;
				}
				else
					previousPosition = position;

				Vector3 returnToDefaultPosition(defaultPosition - position);
				position += returnToDefaultPosition.Normalized() * (float)dt * GetAttribute(CAttributes::ATTRIBUTE_TYPES::TYPE_SPEED);
			}
		}
	}

	else if (state == ALERT && !pathFindingMode)
	{
		//cout << "IN ALERT" << endl;
		if (checkCollision())
		{
			position = previousPosition;
			pathFindingMode = true;
		}
		else
		{
			if (checkInsideBoundary(getMinAlertBoundary(), getMaxAlertBoundary()))
			{
				Vector3 displacement(0.f, 0.f, 0.f);

				if (GetWhoCloser() == ENEMY && returnNearestEnemy() != nullptr)
				{
					enemyWithoutY = Vector3(returnNearestEnemy()->GetPos().x, -10.f, returnNearestEnemy()->GetPos().z);
					//cout << "Enemy without Y: " << enemyWithoutY << endl;
					displacement = Vector3(enemyWithoutY.x - thisWithoutY.x, position.y, enemyWithoutY.z - thisWithoutY.z);
				}
				else if (GetWhoCloser() == PLAYER)
					displacement = playerWithoutY - thisWithoutY;

				/*Using comparison of magnitude to mimic the real world environment where if the a person just left you not long ago, you will be more alerted and prepare if the person will return.*/
				if (displacement.LengthSquared() > 100.f)
				{
					//cout << "MOVING TOWARDS ALERT" << endl;
					position += displacement.Normalized() * (float)dt * GetAttribute(CAttributes::ATTRIBUTE_TYPES::TYPE_SPEED);
					previousPosition = position;
				}
				else
					state = ATTACK;
			}
			else
				state = IDLE;
		}

	}
	else if (state == ATTACK && !pathFindingMode)
	{
		//cout << "IN ATTACK" << endl;
		if (checkCollision())
		{
			position = previousPosition;
			pathFindingMode = true;
		}
		else
		{
			if (checkInsideBoundary(getMinAlertBoundary(), getMaxAlertBoundary()))
			{
				Vector3 displacement(0.f, 0.f, 0.f);
				if (GetWhoCloser() == ENEMY && returnNearestEnemy() != nullptr)
				{
					enemyWithoutY = Vector3(returnNearestEnemy()->GetPos().x, -10.f, returnNearestEnemy()->GetPos().z);
					//cout << "Enemy without Y: " << enemyWithoutY << endl;
					displacement = Vector3(enemyWithoutY.x - thisWithoutY.x, position.y, enemyWithoutY.z - thisWithoutY.z);
				}
				else if (GetWhoCloser() == PLAYER)
					displacement = playerWithoutY - thisWithoutY;

				// << "returnNearestEnemy(): " << returnNearestEnemy()->GetPos()  << endl;
				//cout << "Displacement Squared: " << displacement.LengthSquared() << endl;

				/*Using comparison of magnitude to mimic the real world environment where if the a person just left you not long ago, you will be more alerted and prepare if the person will return.*/
				if (displacement.LengthSquared() > 50.f)
				{
					//cout << "MOVING TOWARDS ATTACK" << endl;
					position += displacement.Normalized() * (float)dt * GetAttribute(CAttributes::ATTRIBUTE_TYPES::TYPE_SPEED);
					previousPosition = position;
				}
				else
				{
					if (GetWhoCloser() == ENEMY)
						returnNearestEnemy()->deductHealthBy(this->GetAttribute(CAttributes::TYPE_ATTACK));
					else if (GetWhoCloser() == PLAYER)
						CPlayerInfo::GetInstance()->deductHealthBy(this->GetAttribute(CAttributes::TYPE_ATTACK));
				}
			}
			else
				state = IDLE;
		}
	}
	
	if (pathFindingMode)
	{
		if (state == IDLE)
		{
			targetObjectPosition = defaultPosition;
		}
		else if (state == ALERT)
		{
			if (GetWhoCloser() == ENEMY && returnNearestEnemy() != nullptr)
				targetObjectPosition = returnNearestEnemy()->GetPos();
			else if (GetWhoCloser() == PLAYER)
				targetObjectPosition = CPlayerInfo::GetInstance()->GetPos();
		}
		else if (state == ATTACK)
		{
			if (GetWhoCloser() == ENEMY && returnNearestEnemy() != nullptr)
				targetObjectPosition = returnNearestEnemy()->GetPos();
			else if (GetWhoCloser() == PLAYER)
				targetObjectPosition = CPlayerInfo::GetInstance()->GetPos();
		}

		if (!scanned)
		{
			positionWithoutY.Set(position.x, -10.f, position.z);
			updatePathfinding(positionWithoutY, scale, dt);
			nearestPosition = nearestPath();
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

		if ((nearestPosition - Vector3(position.x, -10.f, position.z)).LengthSquared() > 0.1f)
			position += directionToGo * (float)dt * GetAttribute(CAttributes::ATTRIBUTE_TYPES::TYPE_SPEED);
		else
		{
			while (path.size() > 0)
			{
				path.pop_back();
			}
			pathFindingMode = false;
			scanned = false;
			nearestPosition.SetZero();
			previousPosition = position;
		}
	}

	if (GetAttribute(CAttributes::ATTRIBUTE_TYPES::TYPE_HEALTH) <= 0)
		state = DEAD;

	/*Update enemy facing direction.*/
	if (pathFindingMode)
	{
		Vector3 displacement(nearestPosition - this->GetPos());
		angleToFace = Math::RadianToDegree(atan2(displacement.x, displacement.z));
	}
	else if (state == IDLE)
	{
		try {
			angleToFace = Math::RadianToDegree(atan2(defaultPosition.x - this->position.x, defaultPosition.z - this->position.z));
		}

		catch (exception e)
		{/*Not doing anything because this does not affect any gameplay. It will only make the robot turn 0 degree.*/
		}
	}
	else if (state == ALERT)
	{
		if (GetWhoCloser() == ENEMY  && returnNearestEnemy() != nullptr)
		{
			try {
				angleToFace = Math::RadianToDegree(atan2(returnNearestEnemy()->GetPos().x - this->position.x, returnNearestEnemy()->GetPos().z - this->position.z));
			}

			catch (exception e)
			{ /*Not doing anything because this does not affect any gameplay. It will only make the robot turn 0 degree.*/
			}
		}
		else if (GetWhoCloser() == PLAYER)
		{
			try {
				angleToFace = Math::RadianToDegree(atan2(CPlayerInfo::GetInstance()->GetPos().x - this->position.x, CPlayerInfo::GetInstance()->GetPos().z - this->position.z));
			}

			catch (exception e)
			{ /*Not doing anything because this does not affect any gameplay. It will only make the robot turn 0 degree.*/
			}
		}
	}
	else if (state == ATTACK)
	{
		if (GetWhoCloser() == ENEMY && returnNearestEnemy() != nullptr)
		{
			try {
				angleToFace = Math::RadianToDegree(atan2(returnNearestEnemy()->GetPos().x - this->position.x, returnNearestEnemy()->GetPos().z - this->position.z));
			}

			catch (exception e)
			{ /*Not doing anything because this does not affect any gameplay. It will only make the robot turn 0 degree.*/
			}
		}
		else if (GetWhoCloser() == PLAYER)
		{
			try {
				angleToFace = Math::RadianToDegree(atan2(CPlayerInfo::GetInstance()->GetPos().x - this->position.x, CPlayerInfo::GetInstance()->GetPos().z - this->position.z));
			}

			catch (exception e)
			{ /*Not doing anything because this does not affect any gameplay. It will only make the robot turn 0 degree.*/
			}
		}
	}
}

// Render
void CAnimatedEnemy::Render(void)
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	position.y = -10.f + scale.y * 3.8f; //Robot pieces together from the torso, hence displacement needed based on scale 
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Rotate(angleToFace, 0.f, 1.f, 0.f);

	for (int i = 1; i < 6; ++i)
	{
		modelStack.PushMatrix();

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

	if (GetAttribute(CAttributes::TYPE_HEALTH) > 0.f)
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
	result->setDefaultPosition(_position);
	result->pathFindingMode = false;
	result->setAlertBoundary(Vector3(-100.f, -10.f, -100.f), Vector3(100.f, 10.f, 100.f));
	result->setType(1);
	result->SetLight(true);
	EntityManager::GetInstance()->AddEnemy(result);
	return result;
}