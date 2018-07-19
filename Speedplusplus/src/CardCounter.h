#pragma once
#include "Card.h"

//数字カードを使用した数字描画クラス
class CardCounter
{
private:
	Card card;
	int changeNumber;

public:
	//numberに(int)Suit::RedNumを加算すると赤数字になる
	CardCounter(int number, const Math::Vec2& startPos, const Math::Vec2& endPos, float scale);
	bool Update(int number, bool isCntStart);	//0~9の数字を指定, falseでカウント停止
	void Draw();
};