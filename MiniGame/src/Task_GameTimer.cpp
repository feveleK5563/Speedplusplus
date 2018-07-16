#include "Task_GameTimer.h"
#include "DxLib.h"
#include "SystemDefine.h"
#include "GameDefine.h"

namespace GameTimer
{
	//☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★
	//★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆

	//----------------------------------------------
	//タスクのコンストラクタ
	Task::Task():
		TaskAbstract(defGroupName, Priority::countCard),
		gameState(GameState::Ready),
		timeCnt((int)GameState::Ready)
	{ 
		cardCnt[0] = std::make_unique<CardCounter>(0,	Math::Vec2(250, SystemDefine::windowSizeY + 200.f),
														Math::Vec2(120, 150), 0.5f);

		cardCnt[1] = std::make_unique<CardCounter>(3,	Math::Vec2(120, SystemDefine::windowSizeY + 200.f),
														Math::Vec2(250, 150), 0.5f);
	}
	//----------------------------------------------
	//タスクのデストラクタ
	Task::~Task()
	{

	}
	//----------------------------------------------
	//タスクの生成
	std::shared_ptr<Task> Task::Create()
	{
		std::shared_ptr<Task> task = std::make_shared<Task>();
		TS::taskSystem.RegistrationTask(task);

		task->Initialize();
		return task;
	}

	//◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆
	//◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇

	//----------------------------------------------
	//初期化処理
	//----------------------------------------------
	void Task::Initialize()
	{

	}

	//----------------------------------------------
	//終了処理
	//----------------------------------------------
	void Task::Finalize()
	{

	}

	//----------------------------------------------
	//更新
	//----------------------------------------------
	void Task::Update()
	{
		timeCnt.Run();
		cardCnt[0]->Update(timeCnt.GetRemainingTime() / 60 / 10);
		cardCnt[1]->Update(timeCnt.GetRemainingTime() / 60 % 10);

		switch (gameState)
		{
		case GameState::Ready:	//ゲーム開始前
			if (timeCnt.GetRemainingTime() <= 60)
			{
				gameState = GameState::Game;
				timeCnt.SetEndTime((int)GameState::Game);
				timeCnt.ResetCntTime();
			}
			break;

		case GameState::Game:		//ゲーム中
			if (timeCnt.GetRemainingTime() < 60)
			{
				gameState = GameState::GameEnd;
			}
			break;

		case GameState::GameEnd:	//ゲーム終了
			gameState = GameState::Result;
			break;

		case GameState::Result:		//結果画面
			gameState = GameState::End;
			break;

		case GameState::End:	//終了
			KillMe();
			break;
		}
	}

	//----------------------------------------------
	//描画
	//----------------------------------------------
	void Task::Draw()
	{
		for (int i = 0; i < 2; ++i)
		{
			cardCnt[i]->Draw();
		}
	}

	//----------------------------------------------
	//状態の取得
	const GameState& Task::GetTimeState() const
	{
		return gameState;
	}
}