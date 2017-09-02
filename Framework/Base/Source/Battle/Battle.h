#ifndef BATTLE_H
#define BATTLE_H
#include <vector>

using std::vector;

enum class Battle_Option;
class CEnemy;
class CPlayer;

class CBattle
{
	enum TURN
	{
		PLAYER_TURN = 0,
		ENEMY_TURN,
		NO_ONE_TURN
	};
	enum FIGHT_STATE
	{
		PLAYER_ATTACK = 0,
		ENEMY_ATTACK,
		NO_ATTACK
	};
public:
	CBattle();
	~CBattle();

	void Init(void);

	void SetBattleState(bool _battleState);

	bool GetBattleState(void);

	void AddPlayer(CPlayer* _player);
	void AddEnemy(CEnemy* _enemy);

	void Render();
	void Update(double dt);
	void Exit(void);

private:
	/*Check if a battle is ongoing or not.*/
	bool battleState;
	/*Player or enemy go first.*/
	bool checkTurn;
	/*Keep track of battle is at whose turn.*/
	TURN turn;
	/*Shake enemy to visual being damaged.*/
	float shake;
	/*Shake everything to visual being attacked.*/
	float shakeAll;
	/*Keep track of fight state.*/
	FIGHT_STATE fightState;

	/*Selecting option to do.*/
	Battle_Option option;
	vector<CEnemy*>battleList;
	vector<CPlayer*>playerList;

	/*For Positionining*/
	float adjustX;
	float adjustY;
	float scaleX;
	float scaleY;
protected:
};

namespace Activate
{
	void BattleScene(CBattle* _battle, CEnemy* _enemy, CPlayer* _player);
}

#endif