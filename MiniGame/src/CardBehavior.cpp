#include "CardBehavior.h"
#include "SystemDefine.h"
#include "InputState.h"

//-----------------------------------------------------------------------------
//ロゴカード(タイトル画面)
CB_LogoCard::CB_LogoCard():
	card( CardID(Suit::Etc, 1, Side::Back),
		 Math::Vec2(SystemDefine::windowSizeX / 2.f, -200),
		 Math::Vec2(SystemDefine::windowSizeX / 2.f, SystemDefine::windowSizeY / 2.f),
		 1.f, 1.f,
		 0.f, (float)SystemDefine::OutputRandomRange(-5, 5, 1)[0]),
	progress(0)
{
	card.ChangeFrontBack();
}
bool CB_LogoCard::Update()
{
	bool endUpdate = card.Update();

	//進行度によって処理を変える
	switch (progress)
	{
	case 0:	//ロゴが中心に移動
			//ボタン押したら画面下に消える
		if (Input::key[KEY_INPUT_SPACE] == DOWN ||
			Input::joypad1[PadInput::A] == DOWN)
		{
			++progress;
			card.SetEndMove(Math::Vec2(SystemDefine::windowSizeX / 2.f, SystemDefine::windowSizeY + 300.f),
				1.f,
				0.f);
		}
		break;

	case 1:
		//画面下に消えたら消滅
		if (endUpdate)
		{
			return true;
		}
		break;
	}

	return false;
}
void CB_LogoCard::Draw()
{
	card.Draw();
}