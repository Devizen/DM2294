#ifndef BATTLE_OPTION_H
#define BATTLE_OPTION_H

enum class Battle_Option
{
	NO_TOP_OPTION = 0,
	ATTACK_OPTION,
	DEFEND_OPTION,
	RUN_OPTION,
	NO_BOTTOM_OPTION
};

/*Increment*/
Battle_Option& operator++(Battle_Option &_option);
Battle_Option operator++(Battle_Option &_option, int);
/*Decrement*/
Battle_Option& operator--(Battle_Option &_option);
Battle_Option operator--(Battle_Option &_option, int);

#endif