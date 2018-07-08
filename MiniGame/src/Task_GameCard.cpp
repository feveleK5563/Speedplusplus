#include "Task_GameCard.h"
#include "DxLib.h"

namespace GameCard
{
	//☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★
	//★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆

	//----------------------------------------------
	//タスクのコンストラクタ
	Task::Task(CardType cardType, const CardID& id, const Math::Vec2& pos, float priority):
		TaskAbstract(defGroupName, priority)
	{
		Initialize(cardType, id, pos);
	}
	//----------------------------------------------
	//タスクのデストラクタ
	Task::~Task()
	{

	}
	//----------------------------------------------
	//タスクの生成
	std::shared_ptr<Task> Task::Create(CardType cardType, const CardID& id, const Math::Vec2& pos, float priority)
	{
		std::shared_ptr<Task> task =
			std::make_shared<Task>(cardType, id, pos, priority);
		TS::taskSystem.RegistrationTask(task);

		return task;
	}

	//◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆
	//◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇

	//----------------------------------------------
	//初期化処理
	//----------------------------------------------
	void Task::Initialize(CardType cardType, const CardID& id, const Math::Vec2& pos)
	{
		switch (cardType)
		{
		case CardType::LogoCard:		//ロゴカード
			cardBehavior = std::make_unique<CB_LogoCard>(id, pos);
			break;

		case CardType::HandCardLeft:	//プレイヤー操作用カード左
			cardBehavior = std::make_unique<CB_HandCard>(id, pos, true);
			break;

		case CardType::HandCardRight:	//プレイヤー操作用カード右
			cardBehavior = std::make_unique<CB_HandCard>(id, pos, false);
			break;
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
		if (cardBehavior->Update())
		{
			KillMe();
		}
	}

	//----------------------------------------------
	//描画
	//----------------------------------------------
	void Task::Draw()
	{
		cardBehavior->Draw();
	}
}