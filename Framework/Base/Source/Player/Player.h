#ifndef PLAYER_H
#define PLAYER_H

#include "Vector3.h"
#include "../Attributes.h"

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

	/*Move player position.*/
	void MovePlayer(float move, MOVE_PLAYER direction);

	/*Update player.*/
	void Update(double dt);
private:
	Vector3 position;
protected:
};

#endif