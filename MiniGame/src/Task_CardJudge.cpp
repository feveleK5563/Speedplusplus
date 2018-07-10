#include "Task_CardJudge.h"
#include "DxLib.h"
#include "SystemDefine.h"
#include "InputState.h"
#include "Task_SceneTitle.h"
#include "SystemDefine.h"

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
		std::shared_ptr<CardID> cardID = RandomNextCard();
		GameCard::Task::Create(
			CardType::CenterCard,
			*cardID,
			Math::Vec2(SystemDefine::windowSizeX / 2.f, -200.f),
			GameCard::defPriority - 0.1f
		);
		centerCardBundle.emplace_back(cardID);
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
		//手札を設定
		switch (SystemDefine::OutputRandomRange(0, 2, 1)[0])
		{
		case 0:
			handCard[0] = ChoiceNextCard(true);
			handCard[1] = ChoiceNextCard(false);
			break;

		case 1:
			handCard[0] = ChoiceNextCard(false);
			handCard[1] = ChoiceNextCard(true);
			break;

		case 2:
			handCard[0] = ChoiceNextCard(false);
			handCard[1] = ChoiceNextCard(false);
			break;
		}

		//カード左
		GameCard::Task::Create(
			CardType::HandCardLeft,
			*handCard[0],
			Math::Vec2(SystemDefine::windowSizeX / 2.f, SystemDefine::windowSizeY + 200.f),
			GameCard::defPriority
		);

		//カード右
		GameCard::Task::Create(
			CardType::HandCardRight,
			*handCard[1],
			Math::Vec2(SystemDefine::windowSizeX / 2.f, SystemDefine::windowSizeY + 200.f),
			GameCard::defPriority
		);

		progress = 1;
	}

	//----------------------------------------------
	//入力に応じて手札を登録する
	void Task::SetHandCard()
	{
		//同時押しは無効
		if (SelectLeftCard() && SelectRightCard())
			return;

		//カード左
		if (SelectLeftCard())
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
		if (SelectRightCard())
		{
			centerCardBundle.emplace_back(handCard[1]);
			for (int i = 0; i < 2; ++i)
			{
				handCard[i].reset();
			}
			progress = 0;
			return;
		}

		//カードを捨て、新規にカードを追加する
		if (SelectThrowCard())
		{
			for (int i = 0; i < 2; ++i)
			{
				handCard[i].reset();
			}
			std::shared_ptr<CardID> cardID = RandomNextCard();
			GameCard::Task::Create(
				CardType::CenterCard,
				*cardID,
				Math::Vec2(SystemDefine::windowSizeX / 2.f, -200.f),
				GameCard::defPriority - 0.1f
			);
			centerCardBundle.emplace_back(cardID);
			progress = 0;
		}
	}

	//----------------------------------------------
	//ランダムにカードを追加する
	std::shared_ptr<CardID> Task::RandomNextCard()
	{
		return std::make_shared<CardID>(
			Suit(SystemDefine::OutputRandomRange(0, 3, 1)[0]),
			SystemDefine::OutputRandomRange(0, 12, 1)[0],
			Side::Front);
	}

	//----------------------------------------------
	//手札を設定する
	std::shared_ptr<CardID> Task::ChoiceNextCard(bool isAnswerCard)
	{
		//中心に何のカードもなかった場合はただのランダム
		if (centerCardBundle.empty())
			return RandomNextCard();

		//中心カード束の一番上を取得
		auto centerTop = centerCardBundle.back();

		std::vector<std::shared_ptr<CardID>> answer;
		answer.reserve(52);
		for (int s = 0; s < 4; ++s)
		{
			for (int n = 0; n < 13; ++n)
			{
				if (((abs(centerTop->number - n) == 1) == isAnswerCard) &&
					(centerTop->suit != Suit(s) && centerTop->number != n))
				{
					answer.emplace_back(std::make_shared<CardID>(Suit(s), n, Side::Front));
				}
			}
		}

		return answer[SystemDefine::OutputRandomRange(0, (int)answer.size() - 1, 1)[0]];
	}

	//----------------------------------------------
	//中心カードの枚数を取得
	int Task::GetCardNum() const
	{
		return (int)centerCardBundle.size();
	}


	bool SelectLeftCard()
	{
		return	Input::key[KEY_INPUT_A]			== DOWN ||
				Input::joypad1[PadInput::LEFT]	== DOWN;
	}
	bool SelectRightCard()
	{
		return	Input::key[KEY_INPUT_D]			== DOWN ||
				Input::joypad1[PadInput::B]		== DOWN;
	}
	bool SelectThrowCard()
	{
		return	Input::key[KEY_INPUT_W]			== DOWN ||
				Input::joypad1[PadInput::Y]		== DOWN ||
				Input::joypad1[PadInput::UP]	== DOWN;
	}
}