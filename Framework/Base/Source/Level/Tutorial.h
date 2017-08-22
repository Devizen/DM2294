#ifndef TUTORIAL_H
#define TUTORIAL_H

#include "../SceneText.h"
#include "SingletonTemplate.h"

class Tutorial : public Singleton<Tutorial>, public SceneText
{
	//float m_worldHeight;
	//float m_worldWidth;
public:
	Tutorial();
	~Tutorial();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	
};

#endif

