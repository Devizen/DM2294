#ifndef BATTLE_EDITOR
#define BATTLE_EDITOR

#include <string>

class CBattle_Editor
{
	static CBattle_Editor* s_instance;
public:
	CBattle_Editor();
	~CBattle_Editor();

	static CBattle_Editor* GetInstance(void);

	/*Set state.*/
	void SetState(bool _state);

	/*Get state*/
	bool GetState(void);

	/*Save animation.*/
	void Save(void);
	/*Load animation.*/
	void Load(std::string _model, unsigned _animationCount);
	/*Render Battle Editor to screen.*/
	void Render(void);

	/*Update Battle Editor; controls.*/
	void Update(double dt);
	/*Destroy everything to prevent memory leak.*/
	void DestroyAll(void);
private:
	/*Check if Battle Editor is on.*/
	bool state;
	/*How many animation(s) to play.*/
	unsigned animationCount;
protected:
};

#endif