#pragma once

#include "MeleeInfo.h"

class CKnife :
	public CMeleeInfo
{
public:
	CKnife();
	virtual ~CKnife();

	// Initialise this instance to default values
	void Init(void);
};

