#include "GameDefine.h"
#include "InputState.h"

namespace Button
{
	//スタート・リセット選択
	bool PushStartReset()
	{
		return	Input::key[KEY_INPUT_SPACE] == DOWN ||
				Input::joypad1[PadInput::START] == DOWN;
	}

	//P1左カード選択
	bool SelectLeftCardP1()
	{
		return	Input::key[KEY_INPUT_A] == DOWN ||
				Input::joypad1[PadInput::LEFT] == DOWN;
	}
	//P1右カード選択
	bool SelectRightCardP1()
	{
		return	Input::key[KEY_INPUT_D] == DOWN ||
				Input::joypad1[PadInput::B] == DOWN;
	}
	//P1パス選択
	bool SelectPassP1()
	{
		return	Input::key[KEY_INPUT_W] == DOWN ||
				Input::joypad1[PadInput::L1] == DOWN ||
				Input::joypad1[PadInput::R1] == DOWN;
	}

	//P2左カード選択
	bool SelectLeftCardP2()
	{
		return	Input::key[KEY_INPUT_LEFT] == DOWN ||
				Input::joypad2[PadInput::LEFT] == DOWN;
	}
	//P2右カード選択
	bool SelectRightCardP2()
	{
		return	Input::key[KEY_INPUT_RIGHT] == DOWN ||
				Input::joypad2[PadInput::B] == DOWN;
	}
	//P2パス選択
	bool SelectPassP2()
	{
		return	Input::key[KEY_INPUT_UP] == DOWN ||
				Input::joypad2[PadInput::L1] == DOWN ||
				Input::joypad2[PadInput::R1] == DOWN;
	}
}