#pragma once
#include "TaskSystem.h"
#include "EasingMover.h"
#include "ImageDrawer.h"

namespace JudgeEffect
{
	const std::string	defGroupName("結果エフェクト");	//グループ名
	const float			defPriority(1.4f);				//デフォルトの処理優先度

	//----------------------------------------------
	class Resource
	{
	private:
		static std::weak_ptr<Resource> instance;

	public:
		ImageData imageData;

		Resource();		//コンストラクタ
		~Resource();	//デストラクタ
		static std::shared_ptr<Resource> Create();	//リソースの生成
	};

	//----------------------------------------------
	class Task : public TaskAbstract
	{
	private:
		std::shared_ptr<Resource> res;	//確保したリソース

		int rw; //正解か間違いか
		ImageDrawer imageDrawer;
		EasingMover easingMover;

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