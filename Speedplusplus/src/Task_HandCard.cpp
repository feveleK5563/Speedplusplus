#include "Task_HandCard.h"
#include "DxLib.h"
#include "SystemDefine.h"
#include "Task_CenterCard.h"
#include "Task_Sound.h"

namespace HandCard
{
	//☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★
	//★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆

	//----------------------------------------------
	//タスクのコンストラクタ
	Task::Task(const CardID& id, const Math::Vec2& pos, const CardDestination& destination, Hand hand):
		TaskAbstract(defGroupName, Priority::handCard),
		card(id,
			pos,
			Math::Vec2(SystemDefine::windowSizeX / 2.f, SystemDefine::windowSizeY / 2.f),
			1.f, 1.f,
			0.f, (float)SystemDefine::GetRand(-5, 5)),
		progress(0),
		destination(destination),
		hand(hand),
		gameState(TS::taskSystem.GetTaskOne<GameTimer::Task>(GameTimer::defGroupName)->GetGameState())
	{
		
	}
	//----------------------------------------------
	//タスクのデストラクタ
	Task::~Task()
	{

	}
	//----------------------------------------------
	//タスクの生成
	std::shared_ptr<Task> Task::Create(const CardID& id, const Math::Vec2& pos, const CardDestination& destination, Hand hand)
	{
		std::shared_ptr<Task> task =
			std::make_shared<Task>(id, pos, destination, hand);
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
		switch (hand)
		{
		case Hand::Left:
			card.SetEndMove(Math::Vec2(250.f, SystemDefine::windowSizeY - 200.f),
				1.3f,
				-1.f);
			break;

		case Hand::Right:
			card.SetEndMove(Math::Vec2(SystemDefine::windowSizeX - 250.f, SystemDefine::windowSizeY - 200.f),
				1.3f,
				1.f);
			break;
		}

		card.ChangeFrontBack(100);
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
			if (Button::SelectLeftCardP1() && Button::SelectRightCardP1())
				break;

			if (gameState == GameState::End)
			{
				MoveOut();
				break;
			}

			switch (destination)
			{
			case CardDestination::Out:
				MoveOut();
				break;

			case CardDestination::Center:
				MoveCenter();
				break;

			case CardDestination::Bump:
				MoveBump();
				break;
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


	//----------------------------------------------
	//画面外に移動する
	void Task::MoveOut()
	{
		//ゲーム本編終了時か、
		//パスボタンか逆方向ボタンを押したら画面外に移動
		auto sound = TS::taskSystem.GetTaskOne<Sound::Task>(Sound::defGroupName);
		sound->PlaySE_HandOut(100);
		switch (hand)
		{
		case Hand::Left:
			card.SetEndMove(Math::Vec2(-300.f, SystemDefine::windowSizeY - 200.f),
				1.3f, -90.f);
			break;

		case Hand::Right:
			card.SetEndMove(Math::Vec2(SystemDefine::windowSizeX + 300.f, SystemDefine::windowSizeY - 200.f),
				1.3f, 90.f);
			break;
		}
		++progress;
	}

	//----------------------------------------------
	//中心に移動するカードを生成
	void Task::MoveCenter()
	{
		//カード移動情報が中心になったら中心に移動するカードを生成して消去
		CenterCard::Task::Create(card.GetID(), card.GetPos(), false);
		KillMe();
	}

	//----------------------------------------------
	//数フレーム中心に移動後画面外へ移動するカードを生成
	void Task::MoveBump()
	{
		CenterCard::Task::Create(card.GetID(), card.GetPos(), true);
		KillMe();
	}
}