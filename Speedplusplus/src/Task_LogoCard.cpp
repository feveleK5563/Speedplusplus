#include "Task_LogoCard.h"
#include "DxLib.h"
#include "SystemDefine.h"
#include "GameDefine.h"
#include "Task_Sound.h"

namespace LogoCard
{
	//☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★
	//★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆

	//----------------------------------------------
	//タスクのコンストラクタ
	Task::Task(const CardID& id, const Math::Vec2& pos):
		TaskAbstract(defGroupName, Priority::logoCard),
		card(id, pos, 1.f, (float)SystemDefine::GetRand(-5, 5)),
		progress(0),
		isBack(false)
	{
		if (card.GetID().side == Side::Back)
		{
			card.ChangeFrontBack(150);
			card.SetEndMove(
				Math::Vec2(SystemDefine::windowSizeX / 2.f, SystemDefine::windowSizeY / 2.f),
				1.f, (float)SystemDefine::GetRand(-5, 5));
		}
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
		bool endUpdate = card.Update(30.f);

		//進行度によって処理を変える
		switch (progress)
		{
		case 0:	//ロゴが中心に移動、ボタン押したら画面下に消える
			if (SelectThrowCard())
			{
				//【隠し要素】Wキー(LR)押したら向きを反転
				card.ChangeFrontBack(150);
				isBack = !isBack;
			}
			if (PushStart())
			{
				auto sound = TS::taskSystem.GetTaskOne<Sound::Task>(Sound::defGroupName);
				sound->PlaySE_HandOut(200);

				++progress;
				card.SetEndMove(Math::Vec2(SystemDefine::windowSizeX / 2.f, SystemDefine::windowSizeY + 300.f),
					1.f,
					0.f);
			}
			break;

		case 1:
			//画面下に消えたら消滅
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

	//カードが裏だったらtrueを返す
	bool Task::GetIsBack() const
	{
		return isBack;
	}
}