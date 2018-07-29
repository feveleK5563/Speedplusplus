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
	Task::Task(bool isBackMode):
		TaskAbstract(defGroupName, defPriority),
		isBackMode(isBackMode)
	{ 
	}
	//----------------------------------------------
	//タスクのデストラクタ
	Task::~Task()
	{

	}
	//----------------------------------------------
	//タスクの生成
	std::shared_ptr<Task> Task::Create(bool isBackMode)
	{
		std::shared_ptr<Task> task = std::make_shared<Task>(isBackMode);
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
		gameTimer = GameTimer::Task::Create();
		cardJudge = CardJudge::Task::Create();
		gameScore = GameScore::Task::Create();
	}

	//----------------------------------------------
	//終了処理
	//----------------------------------------------
	void Task::Finalize()
	{
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
			cardJudge->KillMe();
			gameScore->KillMe();
			KillMe();
		}
	}

	//----------------------------------------------
	//描画
	//----------------------------------------------
	void Task::Draw()
	{
		
	}

	//裏モードか否かを取得
	bool Task::GetIsBackMode() const
	{
		return isBackMode;
	}
}