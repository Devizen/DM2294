#include "Battle_Option.h"

Battle_Option & operator++(Battle_Option & _option)
{
	_option = static_cast<Battle_Option>(static_cast<int>(_option) + 1);
	if (_option == Battle_Option::NO_BOTTOM_OPTION)
		_option = static_cast<Battle_Option>(1);
	return _option;
}

Battle_Option operator++(Battle_Option & _option, int)
{
	Battle_Option result = _option;
	++_option;
	return result;
}

Battle_Option & operator--(Battle_Option & _option)
{
	_option = static_cast<Battle_Option>(static_cast<int>(_option) - 1);
	if (_option == Battle_Option::NO_TOP_OPTION)
		_option = static_cast<Battle_Option>(3);
	return _option;
}

Battle_Option operator--(Battle_Option & _option, int)
{
	Battle_Option result = _option;
	--_option;
	return result;
}
