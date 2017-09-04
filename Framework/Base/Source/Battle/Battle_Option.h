#ifndef BATTLE_OPTION_H
#define BATTLE_OPTION_H

enum class ECBattle_Option
{
	NO_TOP_OPTION = 0,
	ATTACK_OPTION,
	DEFEND_OPTION,
	RUN_OPTION,
	NO_BOTTOM_OPTION
};

/*Increment*/
ECBattle_Option& operator++(ECBattle_Option &_option);
ECBattle_Option operator++(ECBattle_Option &_option, int);
/*Decrement*/
ECBattle_Option& operator--(ECBattle_Option &_option);
ECBattle_Option operator--(ECBattle_Option &_option, int);

#endif