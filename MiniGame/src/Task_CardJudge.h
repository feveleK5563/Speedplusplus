#pragma once
#include "TaskSystem.h"
#include "Task_GameTimer.h"
#include "Task_LogoCard.h"
#include "Task_HandCard.h"
#include "Task_CenterCard.h"

namespace CardJudge
{
	const std::string	defGroupName("カード判定師");	//グループ名
	const float			defPriority(0.f);			//デフォルトの処理優先度

	//----------------------------------------------
	class Task : public TaskAbstract
	{
	private:
		bool isHaveHandCard;	//手札を持っているかどうか
		int scoreFluctuation;	//正誤状況を格納(正解したら+1, 間違えたら-1になる)

		int centerCardNum;		//中央のカード枚数
		std::pair<bool, std::shared_ptr<CardID>> handCard[2];	//手札
		std::shared_ptr<CardID> centerTopCard;	//中心の先頭カード

		const GameState* gameState;	//現在のゲーム進行状況
		

	public:
		Task();		//コンストラクタ
		~Task();	//デストラクタ
		static std::shared_ptr<Task> Create();	//タスクの生成

		void Initialize();			//初期化処理
		void Finalize() override;	//終了処理
		void Update() override;		//更新
		void Draw() override;		//描画

	private:
		void CreateHandCard();		//手札を作成する
		void Judge();				//選択した手札によって、スコアの変化とエフェクトの生成を行う

		//ランダムにカードを作成、追加する
		void CreateRandomCard(Side side);
		//手札を設定する
		void SetNextHandCard();
		//正誤に応じてエフェクトを発生させる
		void CreateEffect(float angle, float length, bool rw);

	public:
		const int& GetCenterCardNum() const;		//中央カードの枚数を取得
		const int& GetScoreFluctuation() const;		//正誤状況を取得
	};
}