/*Text Display was done by Lai Wei Jing, Eugene (160540A)*/
/*Tested by Chan Zhi Hao, Lim Guan Sheng, Marcus and Kwong Yu Ming*/

#ifndef TEXT_H
#define TEXT_H

#include <vector>
#include <string>

class Mesh;

using std::vector;
using std::string;

class CText
{
public:
	CText(Mesh* _modelMesh);
	~CText();

	enum TEXT_TYPE
	{
		TEXT_CONVERSATION = 0,
		TEXT_POPUP,
		TEXT_BATTLE,
		TEXT_IMPACT,
		TEXT_STAY,
		TEXT_NONE
	};

	string storedMessage;
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
	float printSpeed;
	vector<string>textConversation;
	vector<CText*>textImpact;
};

namespace Create
{
	CText* Text(const std::string& _meshName = "text"
		, string _message = ""
		, float _durationElapsed = 0.f
		, float _maxDuration = 1.f
		, CText::TEXT_TYPE _textType = CText::TEXT_NONE
		, float _printSpeed = 3.f);
};
#endif