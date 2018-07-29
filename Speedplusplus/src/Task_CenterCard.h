#pragma once
#include "TaskSystem.h"
#include "Card.h"
#include "Task_GameTimer.h"

namespace CenterCard
{
	const std::string	defGroupName("中央カード");	//グループ名

	//----------------------------------------------
	class Task : public TaskAbstract
	{
	private:
		Card	card;				//カード情報
		int		progress;			//進行度
		bool	isOut;				//画面外に消えるか否か
		float	moveSpeed;			//カードの移動速度
		Side	modeSide;			//カードの向き

		const int&	centerCardNum;	//中央のカード枚数
		int			cardOrder;		//このカードの順番

		const GameState& gameState;	//ゲームの進行状況

	public:
		//コンストラクタ
		Task(const CardID& id, const Math::Vec2& pos, Side modeSide);
		//デストラクタ
		~Task();
		//タスクの生成
		static std::shared_ptr<Task> Create(const CardID& id, const Math::Vec2& pos, Side modeSide);

		void Initialize();			//初期化処理
		void Finalize() override;	//終了処理
		void Update() override;		//更新
		void Draw() override;		//描画

	private:
		void WindowOutCard();
	};
}