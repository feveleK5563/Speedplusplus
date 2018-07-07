#pragma once
#include <memory>
#include "TaskSystem.h"
#include "CardBehavior.h"

namespace GameCard
{
	const std::string	defGroupName("ゲームカード");	//グループ名
	const float			defPriority(1.5f);			//デフォルトの処理優先度

	//----------------------------------------------
	class Task : public TaskAbstract
	{
	private:
		CardType cardType;	//カードの種類
		std::unique_ptr<CardBehaviorAbstract> cardBehavior;	//振る舞い

	public:
		//コンストラクタ
		Task(CardType cardType);

		//デストラクタ
		~Task();
		
		//タスクの生成
		static std::shared_ptr<const Task> Create(CardType cardType);

		void Initialize() override;	//初期化処理
		void Finalize() override;	//終了処理
		void Update() override;		//更新
		void Draw() override;		//描画
	};
}