#include "Task_Result_VSMode.h"
#include "SystemDefine.h"
#include "DxLib.h"
#include "Task_GameScore.h"
#include "GameDefine.h"

namespace Result_VSMode
{
	//☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★
	//★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆

	//----------------------------------------------
	//タスクのコンストラクタ
	Task::Task():
		TaskAbstract(defGroupName, defPriority)
	{
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
		int score[2];
		Suit winOrLose[2];
		if (auto sc = TS::taskSystem.GetTaskGroup<GameScore::Task>(GameScore::defGroupName))
		{
			for (int i = 0; i < int(sc->size()); ++i)
			{
				score[i] = (*sc)[i]->GetScore();
			}

			if (score[0] > score[1])		{ winOrLose[0] = Suit::Etc_Win;		winOrLose[1] = Suit::Etc_Lose; }
			else if (score[0] < score[1])	{ winOrLose[0] = Suit::Etc_Lose;	winOrLose[1] = Suit::Etc_Win; }
			else							{ winOrLose[0] = Suit::Etc_Draw;	winOrLose[1] = Suit::Etc_Draw; }
		}

		resultCard[0] = std::make_unique<Card>(
			CardID(Suit::Etc, (int)winOrLose[0], Side::Back),
			Math::Vec2(SystemDefine::windowSizeX - 250.f, -150.f), 1.2f, 0.f);

		resultCard[0]->SetEndMove(Math::Vec2(250.f, SystemDefine::windowSizeY / 2.f + 100.f),
			1.f, (float)SystemDefine::GetRand(-5, 5));


		resultCard[1] = std::make_unique<Card>(
			CardID(Suit::Etc, (int)winOrLose[1], Side::Back),
			Math::Vec2(250.f, -150.f), 1.2f, 0.f);

		resultCard[1]->SetEndMove(Math::Vec2(SystemDefine::windowSizeX - 250.f, SystemDefine::windowSizeY / 2.f + 100.f),
			1.f, (float)SystemDefine::GetRand(-5, 5));

		for (auto& it : resultCard)
		{
			it->ChangeFrontBack(100);
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
		for (auto& it : resultCard)
		{
			it->Update(30.f);
		}

		if (Button::PushStartReset())
		{
			KillMe();
		}
	}

	//----------------------------------------------
	//描画
	//----------------------------------------------
	void Task::Draw()
	{
		for (auto& it : resultCard)
		{
			it->Draw();
		}
	}
}