#ifndef ADJUSTER_H
#define ADJUSTER_H
#include "Vector3.h"

class CAdjuster
{
	static CAdjuster* s_instance;

	enum MODE
	{
		TRANSLATE_MODE = 0,
		SCALE_MODE,
		NO_MODE
	};

public:
	CAdjuster();
	~CAdjuster();

	void Init(void);
	void Update(double dt);
	Vector3 GetPosition(void);
	Vector3 GetScale(void);
	static CAdjuster* GetInstance(void);

	void DestroyAll(void);
private:
	Vector3 position;
	Vector3 scale;
	MODE mode;
protected:
};

#endif