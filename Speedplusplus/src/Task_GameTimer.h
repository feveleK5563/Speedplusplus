#pragma once
#include "TaskSystem.h"
#include "TimeCounter.h"
#include "CardCounter.h"
#include "Task_GameMessage.h"
#include "Task_Ranking.h"
#include "GameDefine.h"

enum struct GameState
{
	Ready,
	CountDown,
	Game,
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
		const int waitCountDown;
		const int waitGame;
		const int waitGameEnd;
		const int waitResult;

		GameState gameState;
		std::unique_ptr<CardCounter> cardCnt[2];
		TimeCounter timeCnt;
		TimeCounter cardAppTimeCnt;
		std::shared_ptr<GameMessage::Task> gameMessage;

		std::shared_ptr<Ranking::Task> ranking;

		Mode mode;

	public:
		Task(Mode mode);		//コンストラクタ
		~Task();	//デストラクタ
		static std::shared_ptr<Task> Create(Mode mode);	//タスクの生成

		void Initialize();			//初期化処理
		void Finalize() override;	//終了処理
		void Update() override;		//更新
		void Draw() override;		//描画

	private:
		void CountUpdate(bool isCntStart);

	public:
		const GameState& GetGameState() const;
	};
}