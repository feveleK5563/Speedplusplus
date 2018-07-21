#include "Task_CenterCard.h"
#include "DxLib.h"
#include "SystemDefine.h"
#include "GameDefine.h"
#include "Task_CardJudge.h"
#include "Task_Sound.h"

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
		isOut(false),
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
			card.ChangeFrontBack(150);
		}

		//タイトル画面移行時にカードを画面外へ散らばらせる
		WindowOutCard();

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

	//----------------------------------------------
	//タイトル画面移行時にカードを画面外へ散らばらせる
	void Task::WindowOutCard()
	{
		if (!isOut &&
			gameState == GameState::End)
		{
			isOut = true;

			auto sound = TS::taskSystem.GetTaskOne<Sound::Task>(Sound::defGroupName);
			sound->PlaySE_Shaffle(200);

			float angle = (float)SystemDefine::GetRand(0, 359);
			card.SetEndMove(
				card.GetPos() +
				Math::Vec2(1200.f * cos(Math::ToRadian(angle)),
					1200.f * sin(Math::ToRadian(angle))),
				1.f,
				(float)SystemDefine::GetRand(0, 359)
			);
			moveSpeed = 30.f;
			progress = 2;
		}
	}
}