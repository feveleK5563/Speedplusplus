#include "Task_SceneTitle.h"
#include "DxLib.h"
#include "InputState.h"
#include "SystemDefine.h"
#include "Task_Back.h"
#include "Task_SceneGame.h"

namespace SceneTitle
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
		std::shared_ptr<Task> task = std::make_shared<Task>();
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
		if (!TS::taskSystem.FindTask(Back::defGroupName))
		{
			Back::Task::Create();
		}

		logoCardRef = LogoCard::Task::Create(
			CardID(Suit::Etc, (int)Suit::Etc_Logo, Side::Back),
			Math::Vec2(SystemDefine::windowSizeX / 2.f, -200));
	}

	//----------------------------------------------
	//終了処理
	//----------------------------------------------
	void Task::Finalize()
	{
		SceneGame::Task::Create();
	}

	//----------------------------------------------
	//更新
	//----------------------------------------------
	void Task::Update()
	{
		if (logoCardRef->state == TaskState::Kill)
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