#include "Task_TitleLogo.h"
#include "DxLib.h"
#include "SystemDefine.h"
#include "InputState.h"

namespace TitleLogo
{
	//----------------------------------------------
	//タスクのコンストラクタ
	Task::Task():
		TaskAbstract(defGroupName, defPriority),
		logoCard(	CardID(Suit::Etc, 1, Side::Back),
					Math::Vec2(SystemDefine::windowSizeX / 2.f, -150),
					Math::Vec2(SystemDefine::windowSizeX / 2.f, SystemDefine::windowSizeY / 2.f),
					1.f, 1.f,
					0.f, float(rand() % 10 - 5))
	{
		Initialize();
	}
	//----------------------------------------------
	//タスクのデストラクタ
	Task::~Task()
	{

	}
	//----------------------------------------------
	//タスクの生成
	const std::shared_ptr<const Task> Task::Create()
	{
		std::shared_ptr<Task> task = std::make_shared<Task>();
		TS::taskSystem.RegistrationTask(task);

		return task;
	}

	//◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆
	//◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇

	//----------------------------------------------
	//初期化処理
	//----------------------------------------------
	void Task::Initialize()
	{
		logoCard.ChangeFrontBack();
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
		logoCard.Update();
		if (Input::key[KEY_INPUT_S] == DOWN)
		{
			logoCard.ChangeFrontBack();
		}
	}

	//----------------------------------------------
	//描画
	//----------------------------------------------
	void Task::Draw()
	{
		logoCard.Draw();
	}
}