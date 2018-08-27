#pragma once

//処理優先度まとめ
namespace Priority
{
	const float		backGround	(1.0f);	//背景
	const float		messageCard	(1.1f);	//メッセージカード(ロゴ、メニュー画面)
	const float		countCard	(1.2f);	//計測用カード
	const float		centerCard	(1.3f);	//中心のカード
	const float		judgeEffect	(1.4f);	//結果エフェクト
	const float		handCard	(1.5f);	//手札
	const float		ranking		(1.6f);	//ランキング	
	const float		message		(1.7f);	//メッセージ	
}

//ゲームモード
enum struct Mode
{
	Non		= -1,	//無効
	Single	= 0,	//シングルプレイ
	VS		= 1,	//対戦
};

//右左中のカード
enum struct Hand
{
	Center	= -1,	//中心
	Left	= 0,	//左手
	Right	= 1,	//右手
};

//カードの移動先
enum struct CardDestination
{
	Non,	//移動なし
	Out,	//画面外(移動後消える)
	Center,	//中心
	Bump,	//カード同士の衝突
};

//プレイヤー
enum struct Player
{
	Player1	= 0,	//プレイヤー1
	Player2	= 1,	//プレイヤー2
};

//ボタン入力まとめ
namespace Button
{
	bool PushStartReset();		//スタート・リセット選択

	bool SelectLeftCardP1();	//P1左カード選択
	bool SelectRightCardP1();	//P1右カード選択
	bool SelectPassP1();		//P1左パス選択

	bool SelectLeftCardP2();	//P2左カード選択
	bool SelectRightCardP2();	//P2右カード選択
	bool SelectPassP2();		//P2パス選択
}