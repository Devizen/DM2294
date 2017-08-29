/*Text Display was done by Lai Wei Jing, Eugene (160540A)*/
/*Tested by Chan Zhi Hao, Lim Guan Sheng, Marcus and Kwong Yu Ming*/

#include "Text.h"
#include "../Text_Manager/Text_Manager.h"
#include "MeshBuilder.h"

CText::CText(Mesh * _modelMesh) :
	modelMesh(_modelMesh)
	, storedMessage("")
	, message("")
	, durationElapsed(0.f)
	, maxDuration(0.f)
	, textType(TEXT_NONE)
	, activateText(true)
	, translateText(0.f)
	, translateBackground(0.f)
	, scaleText(0.f)
	, scaleBackground(0.f)
	, textConversation(NULL)
	, printSpeed(3.f)
{
}

CText::~CText()
{
}

CText * Create::Text(const std::string & _meshName, string _message, float _durationElapsed, float _maxDuration, CText::TEXT_TYPE _textType, float _printSpeed)
{
	Mesh* _modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (_modelMesh == nullptr)
		return nullptr;

	CText* text = new CText(_modelMesh);
	text->message = _message;
	if (_textType == CText::TEXT_CONVERSATION)
		for (unsigned i = 0; i < 3; ++i)
		{
			text->textConversation.push_back("");
		}
	text->durationElapsed = _durationElapsed;
	text->maxDuration = _maxDuration;
	text->textType = _textType;
	text->printSpeed = _printSpeed;

	if (_textType != CText::TEXT_NONE)
		Text_Manager::GetInstance()->addText(text);
	if (_textType != CText::TEXT_BATTLE)
		Text_Manager::GetInstance()->displayingText = true;

	return text;
}