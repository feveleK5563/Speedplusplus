#include "Task_CardJudge.h"
#include "DxLib.h"
#include "SystemDefine.h"
#include "InputState.h"
#include "Task_SceneTitle.h"

namespace CardJudge
{
	//☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★
	//★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆

	//----------------------------------------------
	//タスクのコンストラクタ
	Task::Task():
		TaskAbstract(defGroupName, defPriority),
		progress(0)
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
	std::shared_ptr<Task> Task::Create()
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
		switch (progress)
		{
		case 0:
			CreateHandCard();
			break;

		case 1:
			SetHandCard();
			break;
		}
	}

	//----------------------------------------------
	//描画
	//----------------------------------------------
	void Task::Draw()
	{
		
	}

	//----------------------------------------------
	//手札を作成する
	void Task::CreateHandCard()
	{
		//カード左
		handCard[0] = std::make_shared<CardID>(Suit::Spade, 9, Side::Front);
		GameCard::Task::Create(
			CardType::HandCardLeft,
			*handCard[0],
			Math::Vec2(SystemDefine::windowSizeX / 2.f, SystemDefine::windowSizeY + 200.f),
			GameCard::defPriority
		);

		//カード右
		handCard[1] = std::make_shared<CardID>(Suit::Heart, 9, Side::Front);
		GameCard::Task::Create(
			CardType::HandCardRight,
			*handCard[1],
			Math::Vec2(SystemDefine::windowSizeX / 2.f, SystemDefine::windowSizeY + 200.f),
			GameCard::defPriority
		);

		progress = 1;
	}

	//----------------------------------------------
	//選択した手札を登録する
	void Task::SetHandCard()
	{
		if ((Input::key[KEY_INPUT_A] == DOWN || Input::joypad1[PadInput::LEFT] == DOWN) &&
			(Input::key[KEY_INPUT_D] == DOWN || Input::joypad1[PadInput::B] == DOWN))
			return;

		//カード左
		if (Input::key[KEY_INPUT_A] == DOWN ||
			Input::joypad1[PadInput::LEFT] == DOWN)
		{
			centerCardBundle.emplace_back(handCard[0]);
			for (int i = 0; i < 2; ++i)
			{
				handCard[i].reset();
			}
			progress = 0;
			return;
		}

		//カード右
		if (Input::key[KEY_INPUT_D] == DOWN ||
			Input::joypad1[PadInput::B] == DOWN)
		{
			centerCardBundle.emplace_back(handCard[1]);
			for (int i = 0; i < 2; ++i)
			{
				handCard[i].reset();
			}
			progress = 0;
			return;
		}
	}

	//----------------------------------------------
	//中心カードの枚数を取得
	int Task::GetCardNum() const
	{
		return (int)centerCardBundle.size();
	}
}