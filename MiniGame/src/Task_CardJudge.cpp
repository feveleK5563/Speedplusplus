#include "Task_CardJudge.h"
#include <algorithm>
#include "DxLib.h"
#include "SystemDefine.h"
#include "InputState.h"
#include "Task_SceneTitle.h"
#include "SystemDefine.h"
#include "Task_JudgeEffect.h"

namespace CardJudge
{
	//☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★
	//★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆

	//----------------------------------------------
	//タスクのコンストラクタ
	Task::Task():
		TaskAbstract(defGroupName, defPriority),
		progress(0),
		centerCardNum(0)
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
		CreateRandomCard();
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
			Judge();
			break;
		}
		centerCardNum = (int)centerCardBundle.size();
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
		SetNextHandCard();

		//カード左
		GameCard::Task::Create(
			CardType::HandCardLeft,
			*(handCard[0].second),
			Math::Vec2(SystemDefine::windowSizeX / 2.f, SystemDefine::windowSizeY + 200.f));

		//カード右
		GameCard::Task::Create(
			CardType::HandCardRight,
			*(handCard[1].second),
			Math::Vec2(SystemDefine::windowSizeX / 2.f, SystemDefine::windowSizeY + 200.f));

		progress = 1;
	}

	//----------------------------------------------
	//選択した手札によって、スコアの変化とエフェクトの生成を行う
	void Task::Judge()
	{
		//同時押しは無効
		if (SelectLeftCard() && SelectRightCard())
			return;

		//パス
		if (SelectThrowCard())
		{
			CreateRandomCard();
			float ang = 90.f + SystemDefine::GetRand(-20, 20);
			float length = 200.f;
			if (handCard[0].first == false && handCard[1].first == false)
			{
				JudgeEffect::Task::Create(
					JEffect::Right, ang, length);
			}
			else
			{
				JudgeEffect::Task::Create(
					JEffect::Wrong, ang, length);
			}
			progress = 0;
			return;
		}

		//カード左
		if (SelectLeftCard())
		{
			centerCardBundle.emplace_back(handCard[0].second);
			float ang = -30.f + SystemDefine::GetRand(-20, 20);
			float length = 400.f;
			if (handCard[0].first == true)
			{
				JudgeEffect::Task::Create(
					JEffect::Right, ang, length);
			}
			else
			{
				JudgeEffect::Task::Create(
					JEffect::Wrong, ang, length);
			}
			progress = 0;
			return;
		}

		//カード右
		if (SelectRightCard())
		{
			centerCardBundle.emplace_back(handCard[1].second);
			float ang = 210.f + SystemDefine::GetRand(-20, 20);
			float length = 400.f;
			if (handCard[1].first == true)
			{
				JudgeEffect::Task::Create(
					JEffect::Right, ang, length);
			}
			else
			{
				JudgeEffect::Task::Create(
					JEffect::Wrong, ang, length);
			}
			progress = 0;
			return;
		}
	}

	//----------------------------------------------
	//ランダムにカードを作成、追加する
	void Task::CreateRandomCard()
	{
		std::shared_ptr<CardID> cardID = 
			std::make_shared<CardID>(
			Suit(SystemDefine::GetRand(0, 3)),
			SystemDefine::GetRand(0, 12),
			Side::Front);

		GameCard::Task::Create(
			CardType::CenterCard,
			*cardID,
			Math::Vec2(SystemDefine::windowSizeX / 2.f, -200.f));

		centerCardBundle.emplace_back(cardID);
	}

	//----------------------------------------------
	//手札を設定する
	void Task::SetNextHandCard()
	{
		//中心に何のカードもなかった場合は処理なし
		if (centerCardBundle.empty())
			return;

		//中心カード束の一番上を取得
		auto centerTop = centerCardBundle.back();

		std::vector<std::shared_ptr<CardID>> right;	//正解を格納
		right.reserve(8);
		std::vector<std::shared_ptr<CardID>> wrong;	//間違いを格納
		wrong.reserve(44);

		for (int s = 0; s < 4; ++s)
		{
			for (int n = 0; n < 13; ++n)
			{
				//先頭カードと同じカードだった場合はやり直し
				if (centerTop->suit != Suit(s) && centerTop->number != n)
					continue;

				//差が1か12のカードは正解
				if (abs(centerTop->number - n) % 11 == 1)
				{
					right.emplace_back(std::make_shared<CardID>(Suit(s), n, Side::Front));
				}
				else
				{
					wrong.emplace_back(std::make_shared<CardID>(Suit(s), n, Side::Front));
				}
			}
		}

		//正解となるカードを選択
		int isRight = SystemDefine::GetRand(0, 2);
		if (isRight == 2)	//正解ナシ
		{
			std::random_shuffle(wrong.begin(), wrong.end());
			for (int i = 0; i < 2; ++i)
			{
				handCard[i].first = false;
				handCard[i].second = wrong[i];
			}
		}
		else	//正解アリ
		{
			handCard[isRight].first = true;
			handCard[isRight].second = right[SystemDefine::GetRand(0, (int)right.size() - 1)];
			handCard[!isRight].first = false;
			handCard[!isRight].second = wrong[SystemDefine::GetRand(0, (int)wrong.size() - 1)];
		}
	}

	//----------------------------------------------
	//中央カードの枚数を取得
	const int* Task::GetCenterCardNum() const
	{
		return &centerCardNum;
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