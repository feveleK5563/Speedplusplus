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
	Task::Task(const int& setScore, bool isUseCombo, bool isRedScore,
		const Math::Vec2& startPos, const Math::Vec2& endPos, float size):
		TaskAbstract(defGroupName, Priority::countCard),
		isUseCombo(isUseCombo),
		isRedScore(isRedScore),
		combo(0),
		score(0),
		startPos(startPos),
		endPos(endPos),
		size(size),
		timeCnt(-1),
		scoreFluctuation(setScore)
	{
	}
	//----------------------------------------------
	//タスクのデストラクタ
	Task::~Task()
	{

	}
	//----------------------------------------------
	//タスクの生成
	std::shared_ptr<Task> Task::Create(const int& setScore, bool isUseCombo, bool isRedScore,
		const Math::Vec2& startPos, const Math::Vec2& endPos, float size)
	{
		std::shared_ptr<Task> task = 
			std::make_shared<Task>(setScore, isUseCombo, isRedScore, startPos, endPos, size);
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
		//各カードとの距離を求める
		float cdist = float(int((256 / 10) * size + 1) * 10);

		if (isUseCombo)
		{
			comboCardCnt = std::make_unique<CardCounter>(
				(int)Suit::Etc_RedNum + 0, startPos, endPos, size - 0.05f);
		}

		for (int i = 0; i < cardNum; ++i)
		{
			if (isRedScore)
			{
				scoreCardCnt[i] = std::make_unique<CardCounter>(
					(int)Suit::Etc_RedNum + 0,
					Math::Vec2((startPos.x + cdist) + (cdist * ((cardNum - 1) - i)), startPos.y),
					Math::Vec2((endPos.x + cdist) + (cdist * i), endPos.y), size);
			}
			else
			{
				scoreCardCnt[i] = std::make_unique<CardCounter>(
					0, Math::Vec2((startPos.x + cdist) + (cdist * ((cardNum - 1) - i)), startPos.y),
					Math::Vec2((endPos.x + cdist) + (cdist * i), endPos.y), size);
			}
		}
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

		if (isUseCombo)	comboCardCnt->Update((int)Suit::Etc_RedNum + combo, true);
		
		for (int i = 0; i < cardNum; ++i)
		{
			if (timeCnt.GetNowCntTime() > 5 * i)
			{
				int numPlace = (int)powf(10.f, (float)(cardNum - 1 - i));
				if (isRedScore)
				{
					scoreCardCnt[i]->Update((int)Suit::Etc_RedNum + ((score / numPlace) % 10), true);
				}
				else
				{
					scoreCardCnt[i]->Update((score / numPlace) % 10, true);
				}
			}
		}
	}

	//----------------------------------------------
	//描画
	//----------------------------------------------
	void Task::Draw()
	{
		if (isUseCombo)
		{
			comboCardCnt->Draw();
		}

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
			if (isUseCombo) combo = std::min(combo + 1, 9);		//コンボ最大値 9
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