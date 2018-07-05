#pragma once
#include "TaskSystem.h"
#include "Card.h"

namespace TitleLogo
{
	const std::string	defGroupName("タイトルロゴ");	//グループ名
	const float			defPriority(1.2f);		//デフォルトの処理優先度

	//----------------------------------------------
	class Task : public TaskAbstract
	{
	private:
		Card logoCard;

	public:
		Task();		//コンストラクタ
		~Task();	//デストラクタ
		static const std::shared_ptr<const Task> Create();	//タスクの生成

		void Initialize() override;	//初期化処理
		void Finalize() override;	//終了処理
		void Update() override;		//更新
		void Draw() override;		//描画
	};
}