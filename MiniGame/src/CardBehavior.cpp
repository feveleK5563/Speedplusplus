#include "CardBehavior.h"
#include "SystemDefine.h"
#include "InputState.h"
#include "Task_GameCard.h"
#include "Task_CardJudge.h"

//-----------------------------------------------------------------------------
//ロゴカード(タイトル画面)
CB_LogoCard::CB_LogoCard(const CardID& id, const Math::Vec2& pos):
	card(	id,
			pos,
			Math::Vec2(SystemDefine::windowSizeX / 2.f, SystemDefine::windowSizeY / 2.f),
			1.f, 1.f,
			0.f, (float)SystemDefine::GetRand(-5, 5)),
	progress(0)
{
	card.ChangeFrontBack();
}
bool CB_LogoCard::Update()
{
	bool endUpdate = card.Update(30.f);

	//進行度によって処理を変える
	switch (progress)
	{
	case 0:	//ロゴが中心に移動
			//ボタン押したら画面下に消える
		if (Input::key[KEY_INPUT_S] == DOWN)
		{
			card.ChangeFrontBack();
		}
		if (Input::key[KEY_INPUT_SPACE] == DOWN ||
			Input::joypad1[PadInput::START] == DOWN)
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
	card.Draw(1.f);
}


//-----------------------------------------------------------------------------
//手札(ゲーム本編)
CB_HandCard::CB_HandCard(const CardID& id, const Math::Vec2& pos, bool LorR) :
	card(id, pos, 1.3f, 0.f),
	progress(0),
	LorR(LorR)
{
	if (LorR)
	{
		card.SetEndMove(Math::Vec2(250.f, SystemDefine::windowSizeY - 200.f),
						1.3f,
						-1.f);
	}
	else
	{
		card.SetEndMove(Math::Vec2(SystemDefine::windowSizeX - 250.f, SystemDefine::windowSizeY - 200.f),
						1.3f,
						1.f);
	}
	card.ChangeFrontBack();
}
bool CB_HandCard::Update()
{
	bool endUpdate = card.Update(20.f);

	switch (progress)
	{
	case 0:
		//同時押しは無効
		if (CardJudge::SelectLeftCard() && CardJudge::SelectRightCard())
			break;

		//上ボタンか逆方向ボタンを押したら画面外に移動
		if (CardJudge::SelectThrowCard() ||
			(!LorR && CardJudge::SelectLeftCard()) ||
			(LorR && CardJudge::SelectRightCard()))
		{
			if (LorR)
			{
				card.SetEndMove(Math::Vec2(-300.f, SystemDefine::windowSizeY - 200.f),
								1.3f, -90.f);
			}
			else
			{
				card.SetEndMove(Math::Vec2(SystemDefine::windowSizeX + 300.f, SystemDefine::windowSizeY - 200.f),
								1.3f, 90.f);
			}
			++progress;
			return false;
		}
		//対応した左右ボタンを押したら中心に移動
		if ((LorR && CardJudge::SelectLeftCard()) ||
			(!LorR && CardJudge::SelectRightCard()))
		{
			GameCard::Task::Create(CardType::CenterCard, card.GetID(), card.GetPos());
			return true;
		}
		break;

	case 1:
		if (endUpdate)
		{
			return true;
		}
		break;
	}

	return false;
}
void CB_HandCard::Draw()
{
	card.Draw(1.f);
}

//-----------------------------------------------------------------------------
//中央に移動するカード
CB_CenterCard::CB_CenterCard(const CardID& id, const Math::Vec2& pos):
	card(	id,
			pos, Math::Vec2(SystemDefine::windowSizeX / 2.f, SystemDefine::windowSizeY / 2.f),
			1.3f, 1.f,
			0.f, (float)SystemDefine::GetRand(-5, 5)),
	progress(0)
{
	if (auto ts = TS::taskSystem.GetTaskOne<CardJudge::Task>("カード判定師"))
	{
		centerCardNum = ts->GetCenterCardNum();
		cardOrder = *centerCardNum;
	}
}
bool CB_CenterCard::Update()
{
	switch (progress)
	{
	case 0:
		if (card.Update(10.f))
		{
			++progress;
		}
		break;

	case 1:
		//中心に20枚重なったら削除
		if (*centerCardNum - cardOrder > 20)
			return true;
		break;
	}

	return false;
}
void CB_CenterCard::Draw()
{
	card.Draw(1.f);
}