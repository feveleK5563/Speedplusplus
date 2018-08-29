#pragma once
#include "TaskSystem.h"
#include "Task_GameTimer.h"
#include "Task_LogoCard.h"
#include "Task_HandCard.h"
#include "Task_CenterCard.h"
#include "GameDefine.h"
#include <array>

namespace CardJudge
{
	const std::string	defGroupName("カード判定師");	//グループ名
	const float			defPriority(0.f);			//デフォルトの処理優先度

	//----------------------------------------------
	class Task : public TaskAbstract
	{
	private:
		Mode mode;		//現在のゲームモード

		int centerCardNum;		//中央のカード枚数

		struct HandCardData
		{
			CardDestination destination;	//カードの移動先
			std::shared_ptr<CardID> ID;		//手札のカードID
		};
		std::array<HandCardData, 2> handCardData;	//手札情報
		std::array<int, 2> scoreFluctuation;		//各プレイヤーの正誤状況を格納(当たってたら+1, 間違っていたら-1)

		std::shared_ptr<CardID> centerTopCard;	//中心の先頭カード

		const GameState* gameState;	//現在のゲーム進行状況
		

	public:
		Task(Mode mode);	//コンストラクタ
		~Task();			//デストラクタ
		static std::shared_ptr<Task> Create(Mode mode);	//タスクの生成

		void Initialize();			//初期化処理
		void Finalize() override;	//終了処理
		void Update() override;		//更新
		void Draw() override;		//描画

	private:
		//手札を作成する(numに-1超過の数字を入れるとそのEtcカードが生成される)
		void CreateHandCard(Hand hand, bool isRight, int num);

		void Judge_SingleMode();	//選択した手札によって、スコアの変化とエフェクトの生成を行う(SingleMode)
		void Judge_VSMode();		//ボタン入力によって、スコアの変化とエフェクトの生成を行う(VSMode)

		//中央にランダムにカードを作成、追加する
		void CreateRandomCard(Side side);
		//手札を設定する
		void SetNextHandCard(Hand hand, bool isRight);
		//正誤状況をリセットする
		void ResetScoreFluctuation();
		//正誤を判断し、結果に応じてエフェクトを発生させる
		void CheckAndCreateEffect(CardDestination destination, Hand hand, Player player, const Math::Vec2& pos, const float& angle, const float& length);
		//中心のカードと指定数字を比較して正誤を判定する
		bool CheckRightOrWrong(int num);

	public:
		const int& GetCenterCardNum() const;					//中央カードの枚数を取得
		const int& GetScoreFluctuation(Player player) const;	//指定した手の正誤状況を取得
	};
}