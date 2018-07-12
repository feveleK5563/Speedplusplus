#include "Task_GameCard.h"
#include "DxLib.h"

namespace GameCard
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
	std::shared_ptr<Task> Task::Create(CardType cardType, const CardID& id, const Math::Vec2& pos)
	{
		std::shared_ptr<Task> task =
			std::make_shared<Task>();
		TS::taskSystem.RegistrationTask(task);

		task->Initialize(cardType, id, pos);
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
			priority = centerPriority;
			cardBehavior = std::make_unique<CB_LogoCard>(id, pos);
			break;

		case CardType::HandCardLeft:	//手札用カード左
			cardBehavior = std::make_unique<CB_HandCard>(id, pos, true);
			break;

		case CardType::HandCardRight:	//手札用カード右
			cardBehavior = std::make_unique<CB_HandCard>(id, pos, false);
			break;

		case CardType::CenterCard:		//中央に移動するカード
			priority = centerPriority;
			cardBehavior = std::make_unique<CB_CenterCard>(id, pos);
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