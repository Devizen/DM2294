#include "Battle_Editor.h"

CBattle_Editor* CBattle_Editor::s_instance = 0;

CBattle_Editor::CBattle_Editor() 
	: state(false)
	, animationCount(0)
{
}

CBattle_Editor::~CBattle_Editor()
{
}

CBattle_Editor * CBattle_Editor::GetInstance(void)
{
	if (s_instance == nullptr)
		s_instance = new CBattle_Editor();

	return s_instance;
}

void CBattle_Editor::SetState(bool _state)
{
	state = _state;
}

bool CBattle_Editor::GetState(void)
{
	return state;
}

void CBattle_Editor::Save(void)
{
}

void CBattle_Editor::Load(std::string _model, unsigned _animationCount)
{
}

void CBattle_Editor::Render(void)
{
}

void CBattle_Editor::Update(double dt)
{
}

void CBattle_Editor::DestroyAll(void)
{
	if (s_instance)
	{
		delete s_instance;
		s_instance = nullptr;
	}
}
