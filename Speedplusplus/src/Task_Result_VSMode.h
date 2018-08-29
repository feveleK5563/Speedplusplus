#pragma once
#include "TaskSystem.h"
#include <array>
#include "Card.h"
#include "GameDefine.h"

namespace Result_VSMode
{
	const std::string	defGroupName("リザルト(VSモード)");	//グループ名
	const float			defPriority(Priority::handCard);	//デフォルトの処理優先度

	//----------------------------------------------
	class Task : public TaskAbstract
	{
	private:
		std::array<std::unique_ptr<Card>, 2> resultCard;

	public:
		//コンストラクタ
		Task();
		
		//デストラクタ
		~Task();
		
		//タスクの生成
		static std::shared_ptr<Task> Create();

		void Initialize();			//初期化処理
		void Finalize() override;	//終了処理
		void Update() override;		//更新
		void Draw() override;		//描画
	};
}