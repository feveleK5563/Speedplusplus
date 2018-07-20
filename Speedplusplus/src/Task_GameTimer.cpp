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
		waitCountDown	(180),
		waitGame		(3600),
		waitGameEnd		(180),
		waitResult		(20)
	{ 
		cardCnt[0] = std::make_unique<CardCounter>(
			0,
			Math::Vec2(250, SystemDefine::windowSizeY + 200.f),
			Math::Vec2(120, 150), 0.5f);

		cardCnt[1] = std::make_unique<CardCounter>(
			(int)Suit::Etc_RedNum,
			Math::Vec2(120, SystemDefine::windowSizeY + 200.f),
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
		gameMessage = GameMessage::Task::Create(MessageType::Ready, 50);
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
		CountUpdate(gameState == GameState::CountDown ||
					gameState == GameState::Game);

		switch (gameState)
		{
		case GameState::Ready:	//ゲーム開始前
			if (gameMessage->state != TaskState::Active)
			{
				gameState = GameState::CountDown;
				timeCnt.SetEndTime(waitCountDown);
				timeCnt.ResetCntTime();
			}
			break;

		case GameState::CountDown:	//ゲーム開始直前のカウント
			if (timeCnt.IsTimeEnd())
			{
				gameState = GameState::Game;
				timeCnt.SetEndTime(waitGame);
				timeCnt.ResetCntTime();
			}
			break;

		case GameState::Game:		//ゲーム中
			if (timeCnt.IsTimeEnd())
			{
				gameState = GameState::GameEnd;
				gameMessage = GameMessage::Task::Create(MessageType::Finish, 90);

				timeCnt.SetEndTime(waitGameEnd);
				timeCnt.ResetCntTime();
			}
			break;

		case GameState::GameEnd:	//ゲーム終了
			if (timeCnt.IsTimeEnd())
			{
				gameState = GameState::Result;
				ranking = Ranking::Task::Create();

				timeCnt.SetEndTime(-1);
				timeCnt.ResetCntTime();
			}
			break;

		case GameState::Result:		//結果画面
			if (ranking->state == TaskState::Kill)
			{
				timeCnt.SetEndTime(waitResult);
				timeCnt.ResetCntTime();
			}
			if (timeCnt.IsTimeEnd())
			{
				gameState = GameState::End;
			}
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
	//timeCntの更新と十、一の位の計算
	void Task::CountUpdate(bool showCnt)
	{
		timeCnt.Run();

		cardAppTimeCnt.Run();

		int tenPlace = 0, unitPlace = 0;

		if (showCnt)
		{
			tenPlace = (timeCnt.GetRemainingTime() + 60) / 60 / 10;
			unitPlace = (timeCnt.GetRemainingTime() + 60) / 60 % 10;
		}

		cardCnt[0]->Update(tenPlace, true);

		if (gameState != GameState::Ready ||
			cardAppTimeCnt.GetNowCntTime() > 5)
		{
			//一の位は、残り時間が3以下の場合赤数字になる
			if (tenPlace == 0 && unitPlace <= 3)
				cardCnt[1]->Update((int)Suit::Etc_RedNum + unitPlace, true);
			else
				cardCnt[1]->Update(unitPlace, true);
		}
	}

	//----------------------------------------------
	//状態の取得
	const GameState& Task::GetTimeState() const
	{
		return gameState;
	}
}