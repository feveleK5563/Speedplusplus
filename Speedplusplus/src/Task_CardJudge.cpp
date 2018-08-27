#include "Task_CardJudge.h"
#include <algorithm>
#include "DxLib.h"
#include "SystemDefine.h"
#include "Task_SceneTitle.h"
#include "SystemDefine.h"
#include "Task_JudgeEffect.h"
#include "Task_Sound.h"

#include <iostream>

namespace CardJudge
{
	//☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★
	//★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆

	//----------------------------------------------
	//タスクのコンストラクタ
	Task::Task(Mode mode):
		TaskAbstract(defGroupName, defPriority),
		centerCardNum(0),
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
		if (auto gt = TS::taskSystem.GetTaskOne<GameTimer::Task>(GameTimer::defGroupName))
		{
			gameState = &gt->GetGameState();
		}

		for (auto& it : handCardData)
		{
			it.destination = CardDestination::Out;
			it.ID = std::make_shared<CardID>(Suit::Etc, 0, Side::Front);
		}

		ResetScoreFluctuation();

		switch (mode)
		{
		case Mode::Single:
			break;

		case Mode::VS:
			CreateHandCard(Hand::Left, false, (int)Suit::Etc_1P);
			CreateHandCard(Hand::Right, false, (int)Suit::Etc_2P);
			break;

		default:
			return;
		}

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
		std::cout << centerTopCard->number << " " <<
			handCardData[(int)Hand::Left].ID->number << " " <<
			handCardData[(int)Hand::Right].ID->number << " " << std::endl;

		ResetScoreFluctuation();

		if (*gameState == GameState::Game)
		{
			//入力を基にスコアの変化とエフェクトの生成を行う
			switch (mode)
			{
			case Mode::Single:
				Judge_SingleMode();
				break;

			case Mode::VS:
				Judge_VSMode();
				break;
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
	//手札を作成する(numに-1超過の数字を入れるとそのEtcカードが生成される)
	void Task::CreateHandCard(Hand hand, bool isRight, int num)
	{
		CardID id;

		//カードの情報を設定
		if (num <= -1)
		{
			SetNextHandCard(hand, isRight);
			id = *(handCardData[(int)hand].ID);
		}
		else
		{
			id = CardID(Suit::Etc, (int)num, Side::Back);
		}

		//手札タスクを生成
		HandCard::Task::Create(
			id,
			Math::Vec2(SystemDefine::windowSizeX / 2.f, SystemDefine::windowSizeY + 200.f),
			handCardData[(int)hand].destination,
			hand);
		handCardData[(int)hand].destination = CardDestination::Non;
	}

	//----------------------------------------------
	//選択した手札によって、スコアの変化とエフェクトの生成を行う(SingleMode)
	void Task::Judge_SingleMode()
	{
		//手札が移動する場合は新たに生成する
		if (handCardData[(int)Hand::Left].destination != CardDestination::Non &&
			handCardData[(int)Hand::Right].destination != CardDestination::Non)
		{
			int isRight = SystemDefine::GetRand(0, 2);
			switch (isRight)
			{
			case 0:
				CreateHandCard(Hand::Left, true, -1);
				CreateHandCard(Hand::Right, false, -1);
				break;

			case 1:
				CreateHandCard(Hand::Left, false, -1);
				CreateHandCard(Hand::Right, true, -1);
				break;

			case 2:
				CreateHandCard(Hand::Left, false, -1);
				CreateHandCard(Hand::Right, false, -1);
				break;
			}
		}

		//同時押しは無効
		if (Button::SelectLeftCardP1() && Button::SelectRightCardP1())
			return;

		//パス
		if (Button::SelectPassP1())
		{
			CheckAndCreateEffect(CardDestination::Out, Hand::Center, Player::Player1, 90.f, 200.f);
			CreateRandomCard(Side::Front);
			++centerCardNum;
			for (auto& it : handCardData)
			{
				it.destination = CardDestination::Out;
			}
			return;
		}

		//カード左
		if (Button::SelectLeftCardP1())
		{
			CheckAndCreateEffect(CardDestination::Center, Hand::Left, Player::Player1, -30.f, 400.f);
			centerTopCard = handCardData[(int)Hand::Left].ID;
			++centerCardNum;
			handCardData[(int)Hand::Left].destination = CardDestination::Center;
			handCardData[(int)Hand::Right].destination = CardDestination::Out;
			return;
		}

		//カード右
		if (Button::SelectRightCardP1())
		{
			CheckAndCreateEffect(CardDestination::Center, Hand::Right, Player::Player1, 210.f, 400.f);
			centerTopCard = handCardData[(int)Hand::Right].ID;
			++centerCardNum;
			handCardData[(int)Hand::Left].destination = CardDestination::Out;
			handCardData[(int)Hand::Right].destination = CardDestination::Center;
			return;
		}
	}
	//----------------------------------------------
	//ボタン入力によって、スコアの変化とエフェクトの生成を行う(VSMode)
	void Task::Judge_VSMode()
	{
		if (handCardData[(int)Hand::Left].destination != CardDestination::Non)
		{
			//左カード生成
			CreateHandCard(Hand::Left, SystemDefine::GetRand(0, 1), -1);
		}
		if (handCardData[(int)Hand::Right].destination != CardDestination::Non)
		{
			//右カード生成
			CreateHandCard(Hand::Right, SystemDefine::GetRand(0, 1), -1);
		}

		//カードが数字以外だった場合
		if ((handCardData[(int)Hand::Left].ID->suit >= Suit::Etc) ||
			(handCardData[(int)Hand::Right].ID->suit >= Suit::Etc))
		{
			handCardData[(int)Hand::Left].destination = CardDestination::Out;
			handCardData[(int)Hand::Right].destination = CardDestination::Out;
			return;
		}

		bool selectP1 = Button::SelectLeftCardP1() || Button::SelectRightCardP1();
		bool selectP2 = Button::SelectLeftCardP2() || Button::SelectRightCardP2();

		//プレイヤー1とプレイヤー2が同時に入力した場合は弾かれる
		if (selectP1 && selectP2)
		{
			handCardData[(int)Hand::Left].destination = CardDestination::Bump;
			handCardData[(int)Hand::Right].destination = CardDestination::Bump;
			return;
		}

		//プレイヤー1
		if (selectP1)
		{
			//中心にカードを送る
			CheckAndCreateEffect(CardDestination::Center, Hand::Left, Player::Player1, 210.f, 400.f);
			centerTopCard = handCardData[(int)Hand::Left].ID;
			++centerCardNum;
			handCardData[(int)Hand::Left].destination = CardDestination::Center;
		}
		else if (Button::SelectPassP1())
		{
			//パス
			CheckAndCreateEffect(CardDestination::Out, Hand::Left, Player::Player1, 210.f, 400.f);
			handCardData[(int)Hand::Left].destination = CardDestination::Out;
		}

		//プレイヤー2
		if (selectP2)
		{
			//中心にカードを送る
			CheckAndCreateEffect(CardDestination::Center, Hand::Right, Player::Player2, -30.f, 400.f);
			centerTopCard = handCardData[(int)Hand::Right].ID;
			++centerCardNum;
			handCardData[(int)Hand::Right].destination = CardDestination::Center;
		}
		else if (Button::SelectPassP2())
		{
			//パス
			CheckAndCreateEffect(CardDestination::Out, Hand::Right, Player::Player2, -30.f, 400.f);
			handCardData[(int)Hand::Right].destination = CardDestination::Out;
		}
	}

	//----------------------------------------------
	//ランダムにカードを作成、中央に追加する
	void Task::CreateRandomCard(Side side)
	{
		std::shared_ptr<CardID> cardID = 
			std::make_shared<CardID>(
			Suit(SystemDefine::GetRand(0, 3)),
			SystemDefine::GetRand(0, 12),
			side);

		CenterCard::Task::Create(
			*cardID, Math::Vec2(SystemDefine::windowSizeX / 2.f, -200.f), false);

		centerTopCard= cardID;
	}

	//----------------------------------------------
	//指定した手札を再設定する
	void Task::SetNextHandCard(Hand hand, bool isRight)
	{
		//中心を指定しちゃった場合は処理しない
		if (hand == Hand::Center)
			return;

		std::vector<std::shared_ptr<CardID>> right;	//正解を格納
		right.reserve(8);
		std::vector<std::shared_ptr<CardID>> wrong;	//間違いを格納
		wrong.reserve(44);

		for (int s = 0; s < 4; ++s)
		{
			for (int n = 0; n < 13; ++n)
			{
				//先頭カード、現在の左右手札と同じカードは除外
				if ((centerTopCard->suit == Suit(s) && centerTopCard->number == n) ||
					(handCardData[(int)Hand::Left].ID->suit == Suit(s) && handCardData[(int)Hand::Left].ID->number == n) ||
					(handCardData[(int)Hand::Right].ID->suit == Suit(s) && handCardData[(int)Hand::Right].ID->number == n))
					continue;

				if (CheckRightOrWrong(n))
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

		if (isRight)	//正解のカードを設定
		{
			handCardData[(int)hand].ID = right[SystemDefine::GetRand(0, (int)right.size() - 1)];
		}
		else	//間違いのカードを設定
		{
			handCardData[(int)hand].ID = wrong[SystemDefine::GetRand(0, (int)wrong.size() - 1)];
		}
		//手札の移動情報を無効にする
		handCardData[(int)hand].destination = CardDestination::Out;
	}
	//----------------------------------------------
	//正誤状況をリセットする
	void Task::ResetScoreFluctuation()
	{
		for (auto& it : scoreFluctuation)
		{
			it = 0;
		}
	}
	//----------------------------------------------
	//正誤に応じてエフェクトを発生させる
	void Task::CheckAndCreateEffect(CardDestination destination, Hand hand, Player player, const float& angle, const float& length)
	{
		bool rightORwrong;

		switch (destination)
		{
		case CardDestination::Out:
			switch (mode)
			{
			case Mode::Single:
				rightORwrong =	!CheckRightOrWrong(handCardData[(int)Hand::Left].ID->number) &&
								!CheckRightOrWrong(handCardData[(int)Hand::Right].ID->number);
				break;

			case Mode::VS:
				rightORwrong = !CheckRightOrWrong(handCardData[(int)hand].ID->number);
				break;
			}
			break;

		case CardDestination::Center:
			rightORwrong = CheckRightOrWrong(handCardData[(int)hand].ID->number);
			break;
		}

		float ang = angle + SystemDefine::GetRand(-20, 20);

		if (rightORwrong)
		{
			//正解
			JudgeEffect::Task::Create(
				JEffect::Right, ang, length);
			scoreFluctuation[(int)player] = 1;
		}
		else
		{
			//間違い
			JudgeEffect::Task::Create(
				JEffect::Wrong, ang, length);
			scoreFluctuation[(int)player] = -1;

			auto sound = TS::taskSystem.GetTaskOne<Sound::Task>(Sound::defGroupName);
			sound->PlaySE_PaperTear(200);
		}
	}
	//----------------------------------------------
	//中心のカードと手札を比較して正誤を判定する
	bool Task::CheckRightOrWrong(int num)
	{
		//差が1か12のカードは正解
		return abs(centerTopCard->number - num) % 11 == 1;
	}

	//----------------------------------------------
	//中央カードの枚数を取得
	const int& Task::GetCenterCardNum() const
	{
		return centerCardNum;
	}

	//----------------------------------------------
	//正誤状況を取得
	const int& Task::GetScoreFluctuation(Player player) const
	{
		return scoreFluctuation[(int)player];
	}
}