#include "Task_CenterCard.h"
#include "DxLib.h"
#include "SystemDefine.h"
#include "GameDefine.h"
#include "Task_CardJudge.h"

namespace CenterCard
{
	//☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★
	//★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆

	//----------------------------------------------
	//タスクのコンストラクタ
	Task::Task(const CardID& id, const Math::Vec2& pos):
		TaskAbstract(defGroupName, Priority::countCard),
		card(id,
			pos, Math::Vec2(SystemDefine::windowSizeX / 2.f, SystemDefine::windowSizeY / 2.f),
			1.3f, 1.f,
			0.f, (float)SystemDefine::GetRand(-5, 5)),
		progress(0),
		moveSpeed(15.f),
		centerCardNum(TS::taskSystem.GetTaskOne<CardJudge::Task>(CardJudge::defGroupName)->GetCenterCardNum()),
		cardOrder(centerCardNum),
		gameState(TS::taskSystem.GetTaskOne<GameTimer::Task>(GameTimer::defGroupName)->GetTimeState())
	{
	}
	//----------------------------------------------
	//タスクのデストラクタ
	Task::~Task()
	{

	}
	//----------------------------------------------
	//タスクの生成
	std::shared_ptr<Task> Task::Create(const CardID& id, const Math::Vec2& pos)
	{
		std::shared_ptr<Task> task = std::make_shared<Task>(id, pos);
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
		//ゲーム中にカードが裏返っていたら表にする
		if (card.GetID().side == Side::Back &&
			gameState == GameState::Game)
		{
			card.ChangeFrontBack();
		}

		bool isMoveEnd = card.Update(moveSpeed);
		switch (progress)
		{
		case 0:
			if (isMoveEnd)
			{
				++progress;
			}
			break;

		case 1:
			//中心に20枚重なったら削除
			if (centerCardNum - cardOrder > 30)
			{
				KillMe();
			}
			//(仮)タイトル画面移行時にカードを画面外へ散らばらせる
			if (gameState == GameState::End)
			{
				float angle = (float)SystemDefine::GetRand(0, 360);
				card.SetEndMove(
					card.GetPos() +
					Math::Vec2(	1000.f * cos(Math::ToRadian(angle)),
								1000.f * sin(Math::ToRadian(angle))),
					1.f,
					90.f
				);
				moveSpeed = 30.f;
				++progress;
			}
			break;

		case 2:
			if (isMoveEnd)
			{
				KillMe();
			}
			break;
		}
	}

	//----------------------------------------------
	//描画
	//----------------------------------------------
	void Task::Draw()
	{
		card.Draw();
	}
}