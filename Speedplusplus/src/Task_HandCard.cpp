#include "Task_HandCard.h"
#include "DxLib.h"
#include "GameDefine.h"
#include "SystemDefine.h"
#include "Task_CenterCard.h"
#include "Task_Sound.h"
#include "Task_SceneGame.h"

namespace HandCard
{
	//☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★
	//★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆

	//----------------------------------------------
	//タスクのコンストラクタ
	Task::Task(const CardID& id, const Math::Vec2& pos, bool LorR):
		TaskAbstract(defGroupName, Priority::handCard),
		card(id,
			pos,
			Math::Vec2(SystemDefine::windowSizeX / 2.f, SystemDefine::windowSizeY / 2.f),
			1.f, 1.f,
			0.f, (float)SystemDefine::GetRand(-5, 5)),
		progress(0),
		LorR(LorR),
		gameState(TS::taskSystem.GetTaskOne<GameTimer::Task>(GameTimer::defGroupName)->GetTimeState())
	{
		if (LorR)
		{
			card.SetEndMove(Math::Vec2(250.f, SystemDefine::windowSizeY - 200.f),
				1.3f,
				-1.f);
		}
		else
		{
			card.SetEndMove(Math::Vec2(SystemDefine::windowSizeX - 250.f, SystemDefine::windowSizeY - 200.f),
				1.3f,
				1.f);
		}
		card.ChangeFrontBack(100);
	}
	//----------------------------------------------
	//タスクのデストラクタ
	Task::~Task()
	{

	}
	//----------------------------------------------
	//タスクの生成
	std::shared_ptr<Task> Task::Create(const CardID& id, const Math::Vec2& pos, bool LorR)
	{
		std::shared_ptr<Task> task =
			std::make_shared<Task>(id, pos, LorR);
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
		bool endUpdate = card.Update(20.f);

		switch (progress)
		{
		case 0:
			//同時押しは無効
			if (SelectLeftCard() && SelectRightCard())
				break;

			//ゲーム本編終了時か、
			//上ボタンか逆方向ボタンを押したら画面外に移動
			if (SelectThrowCard() ||
				(!LorR && SelectLeftCard()) ||
				(LorR && SelectRightCard()) ||
				gameState != GameState::Game)
			{
				auto sound = TS::taskSystem.GetTaskOne<Sound::Task>(Sound::defGroupName);
				sound->PlaySE_HandOut(100);
				if (LorR)
				{
					card.SetEndMove(Math::Vec2(-300.f, SystemDefine::windowSizeY - 200.f),
						1.3f, -90.f);
				}
				else
				{
					card.SetEndMove(Math::Vec2(SystemDefine::windowSizeX + 300.f, SystemDefine::windowSizeY - 200.f),
						1.3f, 90.f);
				}
				++progress;
				return;
			}
			//対応した左右ボタンを押したら中心に移動するカードを作成する
			if ((LorR && SelectLeftCard()) ||
				(!LorR && SelectRightCard()))
			{
				if (TS::taskSystem.GetTaskOne<SceneGame::Task>(SceneGame::defGroupName)->GetIsBackMode())
				{
					CenterCard::Task::Create(card.GetID(), card.GetPos(), Side::Back);
				}
				else
				{
					CenterCard::Task::Create(card.GetID(), card.GetPos(), Side::Front);
				}
				KillMe();
			}
			break;

		case 1:
			if (endUpdate)
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