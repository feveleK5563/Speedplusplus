#pragma once
#include "TaskSystem.h"
#include "ImageDrawer.h"

namespace Back
{
	const std::string	defGroupName("背景");	//グループ名
	const float			defPriority(1.f);		//デフォルトの処理優先度

	//----------------------------------------------
	class Resource
	{
	private:
		static std::weak_ptr<Resource> instance;

	public:
		std::string imageBack;
		ImageData imageDataBack;

		std::string imageFrame;
		ImageData imageDataFrame;

		Resource();		//コンストラクタ
		~Resource();	//デストラクタ
		static std::shared_ptr<Resource> Create();	//リソースの生成
	};

	//----------------------------------------------
	class Task : public TaskAbstract
	{
	private:
		std::shared_ptr<Resource> res;	//確保したリソース

		ImageDrawer imageBack;
		ImageDrawer imageFrame;

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