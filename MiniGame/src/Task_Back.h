#pragma once
#include "TaskSystem.h"
#include "ImageDrawer.h"

namespace Back
{
	const std::string	defGroupName("背景");	//グループ名
	const float			defPriority(1.f);		//デフォルトの処理優先度

	const int resourceNum = 2;
	//----------------------------------------------
	class Resource
	{
	private:
		static std::weak_ptr<Resource> instance;

	public:
		std::string	imageName[resourceNum];
		ImageData	imageData[resourceNum];

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
		static std::shared_ptr<Task> Create();	//タスクの生成

		void Initialize();			//初期化処理
		void Finalize() override;	//終了処理
		void Update() override;		//更新
		void Draw() override;		//描画
	};
}