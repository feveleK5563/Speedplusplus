#pragma once
#include "Math.h"
#include "ImageDrawer.h"
#include "Resource_Card.h"
#include "EasingMover.h"

enum struct Suit
{
	Spade	= 0,	//スペード
	Club	= 1,	//クラブ
	Heart	= 2,	//ハート
	Diamond	= 3,	//ダイヤ
	Etc		= 4,	//その他

	Back	= 54,	//裏
};

enum struct Side
{
	Front,		//表(数字の描いてある面)
	Back,		//裏
	NextFront,	//裏から表への遷移途中
	NextBack,	//表から裏への遷移途中
};

struct CardID
{
	Suit	suit;	//スート
	int		number;	//数字
	Side	side;	//裏表
	CardID();
	CardID(Suit suit, int number, Side side);
};

//カードクラス
class Card
{
private:
	std::shared_ptr<CardR::Resource> res;	//確保したリソース

public:
	CardID		id;				//カードの識別

	EasingMover	emover;			//イージングを利用した動作
	float		scaleX;			//横幅(めくる処理に使用する)
	ImageDrawer	imageDrawer;	//描画処理くん

	//コンストラクタ(カードの識別、座標、描画サイズ、角度を指定)
	Card(const CardID& id, const Math::Vec2& pos, float scale, float degAngle);
	//コンストラクタ(カードの識別、座標の移動情報、描画サイズの変更情報、角度の変更情報を指定)
	Card(	const CardID& id,
			const Math::Vec2& startPos, const Math::Vec2& endPos,
			float startScale,			float endScale,
			float startDegAngle,		float endDegAngle);

	//更新(カードがめくり終わったらtrueを返す)
	bool Update(float speed);

	//描画
	void Draw();

	//カードの裏表を変更する
	void ChangeFrontBack();

	//カードの動作目標を変更する
	void SetEndMove(const Math::Vec2& endPos, float endScale, float endDegAngle);

	//座標を取得する
	const Math::Vec2& GetPos() const;

	//カード情報を得る
	const CardID& GetID() const;

private:
	//カードの裏表情報に応じたカードめくり処理(終了したらtrueを返す)
	bool TurnCard();
};