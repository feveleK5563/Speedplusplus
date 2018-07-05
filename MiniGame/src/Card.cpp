#include "Card.h"
#include "DxLib.h"

CardID::CardID():
	suit(Suit::Spade),
	number(0),
	side(Side::Front){}

CardID::CardID(Suit suit, int number, Side side):
	suit(suit),
	number(number),
	side(side){}

//-----------------------------------------------------------------------------
//コンストラクタ(カードの識別、座標、描画サイズ、角度を指定)
Card::Card(const CardID& id, const Math::Vec2& pos, float scale, float degAngle):
	res(CardR::Resource::Create()),
	id(id),
	emover(pos, scale, degAngle),
	scaleX(1.f),
	imageDrawer(res->imageData, true){}

//-----------------------------------------------------------------------------
//コンストラクタ(カードの識別、座標の移動情報、描画サイズの変更情報、角度の変更情報を指定)
Card::Card(	const CardID& id,
			const Math::Vec2& startPos, const Math::Vec2& endPos,
			float startScale, float endScale,
			float startDegAngle, float endDegAngle):
	res(CardR::Resource::Create()),
	id(id),
	emover(startPos, endPos, startScale, endScale, startDegAngle, endDegAngle),
	scaleX(1.f),
	imageDrawer(res->imageData, true) {}


//-----------------------------------------------------------------------------
//更新
bool Card::Update()
{
	emover.Update();
	return TurnCard();
}

//-----------------------------------------------------------------------------
//描画
void Card::Draw()
{
	int showCard;
	if (id.side == Side::Back ||
		id.side == Side::NextFront)
	{
		showCard = static_cast<int>(Suit::Back);
	}
	else
	{
		showCard = static_cast<int>(id.suit) * 13 + id.number;
	}

	SetDrawMode(DX_DRAWMODE_BILINEAR);
	imageDrawer.DrawOne(
		emover.GetPos(),
		emover.GetScale() * 1.f * scaleX,
		emover.GetScale() * 1.f,
		emover.GetAngle(),
		false,
		showCard,
		Color(255, 255, 255, 255)
	);
	SetDrawMode(DX_DRAWMODE_NEAREST);
}

//-----------------------------------------------------------------------------
//カードの裏表を変更する
void Card::ChangeFrontBack()
{
	if (scaleX != 1.f)
		return;

	if (id.side == Side::Front)
	{
		id.side = Side::NextBack;
	}
	else if(id.side == Side::Back)
	{
		id.side = Side::NextFront;
	}
}

//-----------------------------------------------------------------------------
//カードの移動先を設定する
void Card::SetEndPos(const Math::Vec2& endPos)
{
	/*startPos = pos;
	diffPos = endPos - pos;*/
}

//-----------------------------------------------------------------------------
//カードの最大サイズを設定する
void Card::SetEndSize(float endSize)
{
	/*startScale = scale;
	diffScale = endSize - scale;*/
}


//-----------------------------------------------------------------------------
//カードの裏表情報に応じたカードめくり処理(終了したらtrueを返す)
bool Card::TurnCard()
{
	//カードが開く
	if (id.side == Side::Front ||
		id.side == Side::Back)
	{
		scaleX += 0.3f;

		if (scaleX >= 1.f)
		{
			scaleX = 1.f;
			return true;
		}
		return false;
	}

	//カードが閉じる
	scaleX -= 0.3f;
	if (scaleX <= 0.f)
	{
		scaleX = 0.f;
		if (id.side == Side::NextBack)
			id.side = Side::Back;
		else
			id.side = Side::Front;
	}
	return false;
}