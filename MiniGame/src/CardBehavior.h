#pragma once
#include "Card.h"

enum struct CardType
{
	LogoCard,
	HandCardLeft,
	HandCardRight,
	CenterCard,
};

class CardBehaviorAbstract
{
public:
	virtual ~CardBehaviorAbstract(){}
	virtual bool Update() = 0;
	virtual void Draw() = 0;
};

//-----------------------------------------------------------------------------
//ロゴカードに使用する動き(指定座標から中央 → 中央から画面枠下)
class CB_LogoCard : public CardBehaviorAbstract
{
private:
	Card	card;		//カード情報
	int		progress;	//進行度

public:
	CB_LogoCard(const CardID& id, const Math::Vec2& pos);

	bool Update() override;
	void Draw() override;
};

//-----------------------------------------------------------------------------
//手札に使用する動き(指定座標から画面左右下 → 中央移動カードを生成して消滅)
class CB_HandCard : public CardBehaviorAbstract
{
private:
	Card	card;		//カード情報
	bool	LorR;		//初期カード位置(右か左か)

public:
	CB_HandCard(const CardID& id, const Math::Vec2& pos, bool LorR);

	bool Update() override;
	void Draw() override;
};

//-----------------------------------------------------------------------------
//中央に移動するカード
class CB_CenterCard : public CardBehaviorAbstract
{
private:


public:
	CB_CenterCard(const Card& card);

	bool Update() override;
	void Draw() override;
};