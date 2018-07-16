#include "GameDefine.h"
#include "InputState.h"

bool SelectLeftCard()
{
	return	Input::key[KEY_INPUT_A] == DOWN ||
			Input::joypad1[PadInput::LEFT] == DOWN;
}
bool SelectRightCard()
{
	return	Input::key[KEY_INPUT_D] == DOWN ||
			Input::joypad1[PadInput::B] == DOWN;
}
bool SelectThrowCard()
{
	return	Input::key[KEY_INPUT_W] == DOWN ||
			Input::joypad1[PadInput::L1] == DOWN ||
			Input::joypad1[PadInput::R1] == DOWN;
}