#include "CardCounter.h"
#include "SystemDefine.h"

CardCounter::CardCounter(int number, const Math::Vec2& startPos, const Math::Vec2& endPos, float scale):
	card(CardID::CardID(Suit::Etc, number, Side::Back),
		startPos, endPos,
		scale, scale, 0.f, (float)SystemDefine::GetRand(-5, 5)),
	changeNumber(number)
{
	card.ChangeFrontBack(100);
}

//0~9の数を指定, falseでカウント停止
bool CardCounter::Update(int number, bool isCntStart)
{
	bool isMoveEnd = card.Update(30.f);
	if (isCntStart && isMoveEnd)
	{
		//裏返っていたら表に返す
		if (card.GetID().side == Side::Back)
		{
			card.SetSuitAndNumber(Suit::Etc, changeNumber);
			card.ChangeFrontBack(100);
		}
		//数値が表示されているものと異なっている場合
		else if (number != card.GetID().number)
		{
			changeNumber = number;
			card.ChangeFrontBack(100);
		}
	}
	return isMoveEnd;
}

void CardCounter::Draw()
{
	card.Draw();
}