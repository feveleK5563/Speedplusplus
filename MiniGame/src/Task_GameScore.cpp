#define NOMINMAX
#include "Task_GameScore.h"
#include "DxLib.h"
#include "SystemDefine.h"
#include "GameDefine.h"
#include "Task_CardJudge.h"

namespace GameScore
{
	//☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★
	//★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆

	//----------------------------------------------
	//タスクのコンストラクタ
	Task::Task():
		TaskAbstract(defGroupName, Priority::countCard),
		score(0),
		combo(0),
		timeCnt(-1),
		scoreFluctuation(TS::taskSystem.GetTaskOne<CardJudge::Task>(CardJudge::defGroupName)->GetScoreFluctuation())
	{
		comboCardCnt = std::make_unique<CardCounter>(
			(int)Suit::Etc_RedNum + 0, Math::Vec2(900.f, SystemDefine::windowSizeX + 100.f),
			Math::Vec2(895.f, 140.f), 0.25f);

		for (int i = 0; i < cardNum; ++i)
		{
			scoreCardCnt[i] = std::make_unique<CardCounter>(
				0,	Math::Vec2(980.f + (80 * ((cardNum - 1) - i)), -150.f),
					Math::Vec2(980.f + (80 * i), 130.f), 0.3f);
		}
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
		std::shared_ptr<Task> task = 
			std::make_shared<Task>();
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
		ScoreUpdate();

		timeCnt.Run();

		comboCardCnt->Update((int)Suit::Etc_RedNum + combo, true);
		for (int i = 0; i < cardNum; ++i)
		{
			if (timeCnt.GetNowCntTime() > 5 * i)
			{
				int numPlace = (int)powf(10.f, (float)(cardNum - 1 - i));
				scoreCardCnt[i]->Update((score / numPlace) % 10, true);
			}
		}
	}

	//----------------------------------------------
	//描画
	//----------------------------------------------
	void Task::Draw()
	{
		comboCardCnt->Draw();
		for (int i = 0; i < cardNum; ++i)
		{
			scoreCardCnt[i]->Draw();
		}
	}

	//----------------------------------------------
	//スコアの更新
	void Task::ScoreUpdate()
	{
		if (scoreFluctuation > 0)
		{
			score = std::min(score + (10 + (combo * 2)), 9999);	//スコア最大値 9999
			combo = std::min(combo + 1, 9);						//コンボ最大値 9
		}
		else if (scoreFluctuation < 0)
		{
			score = std::max(score - 10, 0);	//スコア最小値 0
			combo = 0;
		}
	}

	//----------------------------------------------
	//スコアの取得
	const int& Task::GetScore() const
	{
		return score;
	}
}