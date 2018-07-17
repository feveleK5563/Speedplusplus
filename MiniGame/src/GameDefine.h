#pragma once

//処理優先度まとめ
namespace Priority
{
	const float		backGround	(1.0f);	//背景
	const float		logoCard	(1.1f);	//ロゴカード
	const float		countCard	(1.2f);	//計測用カード
	const float		centerCard	(1.3f);	//中心のカード
	const float		judgeEffect	(1.4f);	//結果エフェクト
	const float		handCard	(1.5f);	//手札
}

bool SelectLeftCard();
bool SelectRightCard();
bool SelectThrowCard();