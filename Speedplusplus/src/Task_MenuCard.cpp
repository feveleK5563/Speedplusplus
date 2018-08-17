#include "Task_MenuCard.h"
#include "DxLib.h"
#include "GameDefine.h"
#include "SystemDefine.h"
#include "Task_Sound.h"

namespace MenuCard
{
	//☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★
	//★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆

	//----------------------------------------------
	//タスクのコンストラクタ
	Task::Task():
		TaskAbstract(defGroupName, Priority::messageCard),
		mode(Mode::Non),
		seVolume(150),
		progress(0),
		timeCnt(0)
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
		std::shared_ptr<Task> task = 
			std::make_shared<Task>();
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
		centerPos = Math::Vec2(SystemDefine::windowSizeX / 2.f, SystemDefine::windowSizeY / 2.f);
		startPos[0] = Math::Vec2(300.f, -200.f);
		startPos[1] = Math::Vec2(SystemDefine::windowSizeX - 300.f, SystemDefine::windowSizeY + 200.f);
		endPos[0] = Math::Vec2(300.f, SystemDefine::windowSizeY / 2.f);
		endPos[1] = Math::Vec2(SystemDefine::windowSizeX - 300.f, SystemDefine::windowSizeY / 2.f);

		menuCard[(int)Mode::Single] =
			std::make_unique<Card>(CardID(Suit::Etc, (int)Suit::Etc_SingleMode, Side::Back),
				startPos[0],
				1.f,
				0.f);

		menuCard[(int)Mode::VS] =
			std::make_unique<Card>(CardID(Suit::Etc, (int)Suit::Etc_VSMode, Side::Back),
				startPos[1],
				1.f,
				0.f);

		for (int i = 0; i < (int)menuCard.size(); ++i)
		{
			menuCard[i]->ChangeFrontBack(seVolume);
			menuCard[i]->SetEndMove(
				endPos[i],
				1.f,
				(float)SystemDefine::GetRand(-5, 5));
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
		bool endMove = true;
		for (int i = 0; i < (int)menuCard.size(); ++i)
		{
			endMove = menuCard[i]->Update(30.f) && endMove;
		}

		switch (progress)
		{
		case 0:
			if (endMove)
			{
				SelectMode();
			}
			break;

		case 1:
			timeCnt.Run();
			if (timeCnt.IsTimeEnd())
			{
				for (int i = 0; i < (int)menuCard.size(); ++i)
				{
					menuCard[i]->ChangeFrontBack(seVolume);
					menuCard[i]->SetEndMove(
						startPos[i],
						1.f,
						(float)SystemDefine::GetRand(-5, 5));
				}
				++progress;
			}
			break;

		case 2:
			if (endMove)
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
		for (int i = 0; i < (int)menuCard.size(); ++i)
		{
			menuCard[i]->Draw();
		}
	}

	//------------------------------------
	//モードの取得
	const Mode& Task::GetMode()
	{
		return mode;
	}

	//------------------------------------
	//モード選択
	void Task::SelectMode()
	{
		//左(一人プレイ)選択
		if (SelectLeftCard())
		{
			if (mode != Mode::Single)
			{
				mode = Mode::Single;
				if (menuCard[(int)Mode::Single]->GetID().side != Side::Front)
					menuCard[(int)Mode::Single]->ChangeFrontBack(seVolume);
				if (menuCard[(int)Mode::VS]->GetID().side != Side::Back)
					menuCard[(int)Mode::VS]->ChangeFrontBack(seVolume);
			}
		}
		//右(二人プレイ)選択
		else if (SelectRightCard())
		{
			if (mode != Mode::VS)
			{
				mode = Mode::VS;
				if (menuCard[(int)Mode::VS]->GetID().side != Side::Front)
					menuCard[(int)Mode::VS]->ChangeFrontBack(seVolume);
				if (menuCard[(int)Mode::Single]->GetID().side != Side::Back)
					menuCard[(int)Mode::Single]->ChangeFrontBack(seVolume);
			}
		}

		//決定
		if (PushStart())
		{
			switch (mode)
			{
			case Mode::Single:
				menuCard[(int)Mode::Single]->SetEndMove(
					centerPos,
					1.f,
					(float)SystemDefine::GetRand(-5, 5));
				menuCard[(int)Mode::VS]->SetEndMove(
					startPos[(int)Mode::VS],
					1.f,
					0.f);
				break;

			case Mode::VS:
				menuCard[(int)Mode::VS]->SetEndMove(
					centerPos,
					1.f,
					(float)SystemDefine::GetRand(-5, 5));
				menuCard[(int)Mode::Single]->SetEndMove(
					startPos[(int)Mode::Single],
					1.f,
					0.f);
				break;

			case Mode::Non:
				return;
			}
			++progress;
			TS::taskSystem.GetTaskOne<Sound::Task>(Sound::defGroupName)->PlaySE_HandOut(200);
			timeCnt.SetEndTime(60);
		}
	}
}