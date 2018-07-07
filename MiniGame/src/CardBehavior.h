#pragma once
#include "Card.h"

enum struct CardType
{
	LogoCard,
};

class CardBehaviorAbstract
{
public:
	virtual ~CardBehaviorAbstract(){}
	virtual bool Update() = 0;
	virtual void Draw() = 0;
};

//-----------------------------------------------------------------------------
//ロゴカード(タイトル画面)
class CB_LogoCard : public CardBehaviorAbstract
{
private:
	Card	card;		//カード情報
	int		progress;	//進行度

public:
	CB_LogoCard();

	bool Update() override;
	void Draw() override;
};