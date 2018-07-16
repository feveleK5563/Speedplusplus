#include "CardCounter.h"
#include "SystemDefine.h"

CardCounter::CardCounter(int number, const Math::Vec2& startPos, const Math::Vec2& endPos, float scale):
	card(CardID::CardID(Suit::Etc, number, Side::Back),
		startPos, endPos,
		scale, scale, 0.f, (float)SystemDefine::GetRand(-5, 5)),
	changeNumber(number)
{
	card.ChangeFrontBack();
}

bool CardCounter::Update(int number)
{
	bool isMoveEnd = card.Update(30.f);
	if (isMoveEnd)
	{
		if (card.GetID().side == Side::Back)
		{
			card.SetSuitAndNumber(Suit::Etc, changeNumber);
			card.ChangeFrontBack();
		}
		else if (number != card.GetID().number)
		{
			changeNumber = number;
			card.ChangeFrontBack();
		}
	}
	return isMoveEnd;
}

void CardCounter::Draw()
{
	card.Draw();
}