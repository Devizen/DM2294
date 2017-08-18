#ifndef MAP_EDITOR_H
#define MAP_EDITOR_H

#include "SingletonTemplate.h"

/*For using Vector3 to store Coordintaes X, Y and Z.*/
#include "Vector3.h"

/*For string stream.*/
#include <sstream>

using std::ostringstream;

class CPatrol;
class CEnemy3D;
class CHorde;
class CAnimatedEnemy;

class Map_Editor : public Singleton<Map_Editor>
{
public:
	/*To toggle between different options.*/
	enum OPTION_SELECTION_LEVEL
	{
		OBJECT_SELECT = 0,
		ENVIRONMENT_SELECT,
		ENEMY_SELECT,
		OPTION_SELECT_NONE
	};

	/*To toggle between environment and enemy objects.*/
	enum OBJECT_TYPE
	{
		ENVIRONMENT = 0,
		ENEMY,
		OBJECT_TYPE_NONE
	};

	/*Options for Environment Objects*/
	enum ENVIRONMENT_OBJECT
	{
		CRATE = 0,
		ENVIRONMENT_OBJECT_NONE
	};
	/*Options for Enemy Objects*/
	enum ENEMY_OBJECT
	{
		ROBOT = 0,
		TOWER,
		HORDE,
		ENEMY_OBJECT_NONE
	};

	/*Track last option*/
	enum LAST_CREATED_TYPE
	{
		CREATED_ENVIRONMENT = 0,
		CREATED_ENEMY,
		CREATED_ENEMY_HORDE,
		CREATED_NONE
	};

	Map_Editor();
	~Map_Editor();

	/*Toggle Map Editing On and Off.*/
	bool mapEditing;
	/*Render the objects out for visual reference.*/
	void renderObject(void);
	/*Update the options.*/
	void renderOption(void);
	/*Update the options.*/
	void updateOption(double dt);
	/*Reset options.*/
	void resetOption(void);
	/*Removing last object created*/
	void removeLastCreated(void);

	OBJECT_TYPE objectType;
	ENVIRONMENT_OBJECT environmentObject;
	ENEMY_OBJECT enemyObject;
	OPTION_SELECTION_LEVEL optionSelectionLevel;

	LAST_CREATED_TYPE lastCreatedType;

	/*For saving the original displacement.*/
	Vector3 _displacement;
	/*For tuning the displacement.*/
	Vector3 _displacementModifier;
	/*For scaling the object.*/
	Vector3 _scale;

	/*To display Option Selection Level*/
	ostringstream ss_OptionSelectLevel;

	/*To display Object Type*/
	ostringstream ss_ObjectType;

	/*To display Environment Object*/
	ostringstream ss_EnvironmentObject;

	/*To display Enemy Object*/
	ostringstream ss_EnemyObject;

	/*To check if adding enemy waypoint is needed.*/
	bool addWaypoint;

	/*Enemy Object to add waypoint.*/
	CAnimatedEnemy* _enemy;
	/*Enemy turrent to change variables.*/
	CEnemy3D* turret;
	/*Enemy Horde to change variables.*/
	CEnemy3D* _horde;
private:

protected:
};

#endif