#pragma once
#include "TaskSystem.h"
#include "Card.h"

namespace CenterCard
{
	const std::string	defGroupName("中央カード");	//グループ名

	//----------------------------------------------
	class Task : public TaskAbstract
	{
	private:
		Card	card;				//カード情報
		int		progress;			//進行度
		int		cardOrder;			//このカードの順番
		const int*	centerCardNum;	//中央のカード枚数

	public:
		Task(const CardID& id, const Math::Vec2& pos);		//コンストラクタ
		~Task();	//デストラクタ
		static std::shared_ptr<Task> Create(const CardID& id, const Math::Vec2& pos);	//タスクの生成

		void Initialize();			//初期化処理
		void Finalize() override;	//終了処理
		void Update() override;		//更新
		void Draw() override;		//描画
	};
}