#ifndef TEXT_H
#define TEXT_H

#include <string>

class Mesh;

using std::string;

class CText
{
public:
	CText(Mesh* _modelMesh);
	~CText();

	enum TEXT_TYPE
	{
		TEXT_CONVERSATION = 0,
		TEXT_BATTLE,
		TEXT_NONE
	};

	string message;
	float durationElapsed;
	float maxDuration;
	TEXT_TYPE textType;
	Mesh* modelMesh;
	bool activateText;
	float translateText;
	float translateBackground;
	float scaleText;
	float scaleBackground;
};

namespace Create
{
	CText* Text(const std::string& _meshName = "text", string _message = "", float _durationElapsed = 0.f, float _maxDuration = 1.f, CText::TEXT_TYPE _textType = CText::TEXT_NONE);
};
#endif