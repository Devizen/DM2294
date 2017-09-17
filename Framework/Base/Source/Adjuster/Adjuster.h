#ifndef ADJUSTER_H
#define ADJUSTER_H
#include "Vector3.h"

class CAdjuster
{
	/*Singleton instance.*/
	static CAdjuster* s_instance;

public:
	/*Switching between position and scale mode.*/
	enum MODE
	{
		TRANSLATE_MODE = 0,
		SCALE_MODE,
		NO_MODE
	};

	CAdjuster();
	~CAdjuster();

	/*Initialise default values.*/
	void Init(void);
	/*Update values of position and scaling through keypress.*/
	void Update(double dt);
	/*Set the speed of updating values.*/
	void SetSpeed(float _speed);
	/*Set the mode to positioning or scaling.*/
	void SetMode(MODE _mode);
	/*Set the Adjuster to on or off.*/
	void SetState(bool _state);

	/*Get adjusted position values.*/
	Vector3 GetPosition(void);
	/*Get adjusted scale values.*/
	Vector3 GetScale(void);
	/*Get the type of mode Adjuster is using; position or scale.*/
	MODE GetMode(void);
	/*Get the state of Adjuster; on or off.*/
	bool GetState(void);
	/*Get the singleton instance.*/
	static CAdjuster* GetInstance(void);
	/*Delete all points in Adjuster.*/
	void DestroyAll(void);
private:
	/*Adjusting object(s).*/
	Vector3 position;
	/*Scaling object(s).*/
	Vector3 scale;
	/*Switching between positioning and scaling mode.*/
	MODE mode;
	/*Speed of updating values.*/
	float speed;
	/*Adjuster on or off.*/
	bool state;
protected:
};

#endif