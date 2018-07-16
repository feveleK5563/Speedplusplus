#pragma once
#include "TaskSystem.h"
#include "Task_GameTimer.h"
#include "Task_CardJudge.h"

namespace SceneGame
{
	const std::string	defGroupName("本編");	//グループ名
	const float			defPriority(0.f);		//デフォルトの処理優先度

	//----------------------------------------------
	class Task : public TaskAbstract
	{
	private:
		std::shared_ptr<GameTimer::Task> gameTimer;
		std::shared_ptr<CardJudge::Task> cardJudge;

	public:
		Task();		//コンストラクタ
		~Task();	//デストラクタ
		static std::shared_ptr<Task> Create();	//タスクの生成

		void Initialize();			//初期化処理
		void Finalize() override;	//終了処理
		void Update() override;		//更新
		void Draw() override;		//描画
	};
}