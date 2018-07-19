#include "Task_CardJudge.h"
#include <algorithm>
#include "DxLib.h"
#include "SystemDefine.h"
#include "Task_SceneTitle.h"
#include "SystemDefine.h"
#include "GameDefine.h"
#include "Task_JudgeEffect.h"

namespace CardJudge
{
	//☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★
	//★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆

	//----------------------------------------------
	//タスクのコンストラクタ
	Task::Task():
		TaskAbstract(defGroupName, defPriority),
		isHaveHandCard(false),
		centerCardNum(0),
		scoreFluctuation(0)
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
		auto gt = TS::taskSystem.GetTaskOne<GameTimer::Task>(GameTimer::defGroupName);
		gameState = &gt->GetTimeState();

		CreateRandomCard(Side::Back);
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
		scoreFluctuation = 0;

		if (*gameState == GameState::Game)
		{
			if (isHaveHandCard)
			{
				Judge();
			}
			else
			{
				CreateHandCard();
			}
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
		SetNextHandCard();

		//カード左
		HandCard::Task::Create(
			*(handCard[0].second),
			Math::Vec2(SystemDefine::windowSizeX / 2.f, SystemDefine::windowSizeY + 200.f),
			true);

		//カード右
		HandCard::Task::Create(
			*(handCard[1].second),
			Math::Vec2(SystemDefine::windowSizeX / 2.f, SystemDefine::windowSizeY + 200.f),
			false);

		isHaveHandCard = true;
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
			CreateRandomCard(Side::Front);
			CreateEffect(90.f, 200.f,
				handCard[0].first == false && handCard[1].first == false);
			++centerCardNum;
			return;
		}

		//カード左
		if (SelectLeftCard())
		{
			centerTopCard = handCard[0].second;
			CreateEffect(-30.f, 400.f, handCard[0].first == true);
			++centerCardNum;
			return;
		}

		//カード右
		if (SelectRightCard())
		{
			centerTopCard = handCard[1].second;
			CreateEffect(210.f, 400.f, handCard[1].first == true);
			++centerCardNum;
			return;
		}
	}

	//----------------------------------------------
	//ランダムにカードを作成、追加する
	void Task::CreateRandomCard(Side side)
	{
		std::shared_ptr<CardID> cardID = 
			std::make_shared<CardID>(
			Suit(SystemDefine::GetRand(0, 3)),
			SystemDefine::GetRand(0, 12),
			side);

		CenterCard::Task::Create(
			*cardID, Math::Vec2(SystemDefine::windowSizeX / 2.f, -200.f));

		centerTopCard= cardID;
	}

	//----------------------------------------------
	//手札を設定する
	void Task::SetNextHandCard()
	{
		//中心に何のカードもなかった場合は処理なし
		if (centerTopCard == false)
			return;

		std::vector<std::shared_ptr<CardID>> right;	//正解を格納
		right.reserve(8);
		std::vector<std::shared_ptr<CardID>> wrong;	//間違いを格納
		wrong.reserve(44);

		for (int s = 0; s < 4; ++s)
		{
			for (int n = 0; n < 13; ++n)
			{
				//先頭カードと同じカードだった場合はやり直し
				if (centerTopCard->suit == Suit(s) && centerTopCard->number == n)
					continue;

				//差が1か12のカードは正解
				if (abs(centerTopCard->number - n) % 11 == 1)
				{
					//正解のカードを格納
					right.emplace_back(std::make_shared<CardID>(Suit(s), n, Side::Back));
				}
				else
				{
					//間違いのカードを格納
					wrong.emplace_back(std::make_shared<CardID>(Suit(s), n, Side::Back));
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
	//正誤に応じてエフェクトを発生させる
	void Task::CreateEffect(float angle, float length, bool rw)
	{
		float ang = angle + SystemDefine::GetRand(-20, 20);

		if (rw)
		{
			JudgeEffect::Task::Create(
				JEffect::Right, ang, length);
			scoreFluctuation = 1;
		}
		else
		{
			JudgeEffect::Task::Create(
				JEffect::Wrong, ang, length);
			scoreFluctuation = -1;
		}
		isHaveHandCard = false;
	}

	//----------------------------------------------
	//中央カードの枚数を取得
	const int& Task::GetCenterCardNum() const
	{
		return centerCardNum;
	}

	//----------------------------------------------
	//正誤状況を取得
	const int& Task::GetScoreFluctuation() const
	{
		return scoreFluctuation;
	}
}