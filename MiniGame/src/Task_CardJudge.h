#pragma once
#include "TaskSystem.h"
#include "Task_GameCard.h"

namespace CardJudge
{
	const std::string	defGroupName("カード判定師");	//グループ名
	const float			defPriority(0.f);			//デフォルトの処理優先度

	//----------------------------------------------
	class Task : public TaskAbstract
	{
	private:
		int progress;		//進行度
		int centerCardNum;	//中央のカード枚数
		std::pair<bool, std::shared_ptr<CardID>> handCard[2];	//手札
		std::vector<std::shared_ptr<CardID>> centerCardBundle;	//中心のカード束

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
		//エフェクトを発生させる
		void CreateEffect(float angle, float length, bool rw);

	public:
		const int* GetCenterCardNum() const;		//中央カードの枚数を取得
	};

	bool SelectLeftCard();
	bool SelectRightCard();
	bool SelectThrowCard();
}