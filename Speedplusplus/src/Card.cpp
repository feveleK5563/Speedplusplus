#include "Card.h"
#include "DxLib.h"
#include "TaskSystem.h"
#include "Task_Sound.h"

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
bool Card::Update(float speed)
{
	bool endMove = emover.Update(speed);
	bool endTurn = TurnCard();
	return endMove && endTurn;
}

//-----------------------------------------------------------------------------
//描画
void Card::Draw()
{
	int showCard;
	if (id.side == Side::Back ||
		id.side == Side::NextFront)
	{
		//カード裏
		showCard = (int)Suit::Etc * 13 + (int)Suit::Etc_Back;
	}
	else
	{
		showCard = (int)id.suit * 13 + id.number;
	}

	SetDrawMode(DX_DRAWMODE_BILINEAR);
	imageDrawer.DrawOne(
		emover.GetPos(),
		emover.GetScale() * scaleX,
		emover.GetScale(),
		emover.GetAngle(),
		false,
		showCard,
		Color(255, 255, 255, 255)
	);
	SetDrawMode(DX_DRAWMODE_NEAREST);
}

//-----------------------------------------------------------------------------
//カードの裏表を変更する
void Card::ChangeFrontBack(int soundVolume)
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

	if (auto sound = TS::taskSystem.GetTaskOne<Sound::Task>(Sound::defGroupName))
	{
		sound->PlaySE_TurnOver(soundVolume);
	}
}

//-----------------------------------------------------------------------------
//カードの動作目標を変更する
void Card::SetEndMove(const Math::Vec2& endPos, float endScale, float endDegAngle)
{
	emover.SetEndMove(endPos, endScale, endDegAngle);
}

//-----------------------------------------------------------------------------
//カードのスート、数字を設定する
void Card::SetSuitAndNumber(Suit suit, int number)
{
	id.suit = suit;
	id.number = number;
}

//-----------------------------------------------------------------------------
//座標を取得する
const Math::Vec2& Card::GetPos() const
{
	return emover.GetPos();
}

//---------------------------------------------------------------------------- -
//カード情報を得る
const CardID& Card::GetID() const
{
	return id;
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