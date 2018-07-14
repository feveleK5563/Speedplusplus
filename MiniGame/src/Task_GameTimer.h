#pragma once
#include "TaskSystem.h"
#include "TimeCounter.h"
#include "Card.h"

namespace GameTimer
{
	const std::string	defGroupName("ゲームタイマー");	//グループ名
	const float			defPriority(0.f);			//デフォルトの処理優先度

	//----------------------------------------------
	class Task : public TaskAbstract
	{
	private:
		std::unique_ptr<Card> card[2];
		TimeCounter timeCnt;

	public:
		Task();		//コンストラクタ
		~Task();	//デストラクタ
		static std::shared_ptr<Task> Create();	//タスクの生成

		void Initialize();			//初期化処理
		void Finalize() override;	//終了処理
		void Update() override;		//更新
		void Draw() override;		//描画
	};
}