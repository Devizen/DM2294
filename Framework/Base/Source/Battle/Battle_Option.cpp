#include "Battle_Option.h"

ECBattle_Option & operator++(ECBattle_Option & _option)
{
	_option = static_cast<ECBattle_Option>(static_cast<int>(_option) + 1);
	if (_option == ECBattle_Option::NO_BOTTOM_OPTION)
		_option = static_cast<ECBattle_Option>(1);
	return _option;
}

ECBattle_Option operator++(ECBattle_Option & _option, int)
{
	ECBattle_Option result = _option;
	++_option;
	return result;
}

ECBattle_Option & operator--(ECBattle_Option & _option)
{
	_option = static_cast<ECBattle_Option>(static_cast<int>(_option) - 1);
	if (_option == ECBattle_Option::NO_TOP_OPTION)
		_option = static_cast<ECBattle_Option>(3);
	return _option;
}

ECBattle_Option operator--(ECBattle_Option & _option, int)
{
	ECBattle_Option result = _option;
	--_option;
	return result;
}
