#include "Text.h"
#include "../Text_Manager/Text_Manager.h"
#include "MeshBuilder.h"

CText::CText(Mesh * _modelMesh) :
	modelMesh(_modelMesh)
	, message("")
	, durationElapsed(0.f)
	, maxDuration(0.f)
	, textType(TEXT_NONE)
	, activateText(true)
	, translateText(0.f)
	, translateBackground(0.f)
	, scaleText(0.f)
	, scaleBackground(0.f)
{
}

CText::~CText()
{
}

CText * Create::Text(const std::string & _meshName, string _message, float _durationElapsed, float _maxDuration, CText::TEXT_TYPE _textType)
{
	Mesh* _modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (_modelMesh == nullptr)
		return nullptr;

	CText* text = new CText(_modelMesh);
	text->message = _message;
	text->durationElapsed = _durationElapsed;
	text->maxDuration = _maxDuration;
	text->textType = _textType;

	Text_Manager::GetInstance()->addText(text);

	return text;
}