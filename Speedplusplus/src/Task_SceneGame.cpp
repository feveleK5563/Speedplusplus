#include "Task_SceneGame.h"
#include "DxLib.h"
#include "Task_SceneTitle.h"
#include "SystemDefine.h"

namespace SceneGame
{
	//☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★
	//★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆

	//----------------------------------------------
	//タスクのコンストラクタ
	Task::Task(Mode mode):
		TaskAbstract(defGroupName, defPriority),
		mode(mode)
	{ 
	}
	//----------------------------------------------
	//タスクのデストラクタ
	Task::~Task()
	{

	}
	//----------------------------------------------
	//タスクの生成
	std::shared_ptr<Task> Task::Create(Mode mode)
	{
		std::shared_ptr<Task> task = std::make_shared<Task>(mode);
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
		switch (mode)
		{
		case Mode::Single:
			gameTimer = GameTimer::Task::Create(mode);
			cardJudge = CardJudge::Task::Create(mode);
			gameScore[0] = GameScore::Task::Create(
				cardJudge->GetScoreFluctuation(Player::Player1),
				true, false,
				Math::Vec2(900.f, SystemDefine::windowSizeY + 100.f),
				Math::Vec2(900.f, 140.f),
				0.3f);
			break;

		case Mode::VS:
			gameTimer = GameTimer::Task::Create(mode);
			cardJudge = CardJudge::Task::Create(mode);
			gameScore[0] = GameScore::Task::Create(
				cardJudge->GetScoreFluctuation(Player::Player1),
				false, false,
				Math::Vec2(SystemDefine::windowSizeX + 150.f, 130.f),
				Math::Vec2(10, 130),
				0.35f);
			gameScore[1] = GameScore::Task::Create(
				cardJudge->GetScoreFluctuation(Player::Player2),
				false, true,
				Math::Vec2(-150.f, 130.f),
				Math::Vec2(820, 130),
				0.35f);
			break;

		default:
			KillMe();
			return;
		}

	}

	//----------------------------------------------
	//終了処理
	//----------------------------------------------
	void Task::Finalize()
	{
		if (cardJudge)	cardJudge->KillMe();
		for (auto& it : gameScore)
		{
			if (it)	it->KillMe();
		}

		//ロゴカードを作成
		LogoCard::Task::Create(
			CardID(Suit::Etc, (int)Suit::Etc_Logo, Side::Front),
			Math::Vec2(SystemDefine::windowSizeX / 2.f, SystemDefine::windowSizeY / 2.f));

		SceneTitle::Task::Create();
		TS::taskSystem.KillTask("カード判定師");
	}

	//----------------------------------------------
	//更新
	//----------------------------------------------
	void Task::Update()
	{
		if (gameTimer->state == TaskState::Kill)
		{
			KillMe();
		}
	}

	//----------------------------------------------
	//描画
	//----------------------------------------------
	void Task::Draw()
	{
		
	}
}