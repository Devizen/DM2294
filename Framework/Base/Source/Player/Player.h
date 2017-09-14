#ifndef PLAYER_H
#define PLAYER_H

#include "Vector3.h"
#include "../Attributes.h"

class CBattle;

class CPlayer : public CAttributes
{
public:
	enum MOVE_PLAYER
	{
		MOVE_UP = 0,
		MOVE_DOWN,
		MOVE_LEFT,
		MOVE_RIGHT
	};

	CPlayer();
	~CPlayer();

	/*Set player position.*/
	void SetPosition(Vector3 _position);

	/*Get player position.*/
	Vector3 GetPosition(void);

	/*Set player target.*/
	void SetTarget(Vector3 _target);

	/*Get player target.*/
	Vector3 GetTarget(void);

	/*Get battle to trigger battle event.*/
	CBattle* GetBattle(void);

	/*Move player position.*/
	void MovePlayer(float move, MOVE_PLAYER direction);

	/*Update player.*/
	void Update(double dt);
private:
	CBattle* battle;
	Vector3 position;
	Vector3 target;
protected:
};

#endif