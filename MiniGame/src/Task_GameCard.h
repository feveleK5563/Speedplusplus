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
		std::unique_ptr<CardBehaviorAbstract> cardBehavior;	//振る舞い

	public:
		//コンストラクタ
		Task(CardType cardType, const CardID& id, const Math::Vec2& pos, float priority);

		//デストラクタ
		~Task();
		
		//タスクの生成
		static std::shared_ptr<Task> Create(CardType cardType, const CardID& id, const Math::Vec2& pos, float priority);

		void Initialize(CardType cardType, const CardID& id, const Math::Vec2& pos);	//初期化処理
		void Finalize() override;	//終了処理
		void Update() override;		//更新
		void Draw() override;		//描画
	};
}