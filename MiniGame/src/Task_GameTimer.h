#pragma once
#include "TaskSystem.h"
#include "TimeCounter.h"
#include "CardCounter.h"

enum struct GameState
{
	Ready	= 240,
	Game	= 600,
	GameEnd,
	Result,
	End,
};

namespace GameTimer
{
	const std::string	defGroupName("ゲームタイマー");	//グループ名

	//----------------------------------------------
	class Task : public TaskAbstract
	{
	private:
		GameState gameState;
		std::unique_ptr<CardCounter> cardCnt[2];
		TimeCounter timeCnt;

	public:
		Task();		//コンストラクタ
		~Task();	//デストラクタ
		static std::shared_ptr<Task> Create();	//タスクの生成

		void Initialize();			//初期化処理
		void Finalize() override;	//終了処理
		void Update() override;		//更新
		void Draw() override;		//描画

		const GameState& GetTimeState() const;
	};
}