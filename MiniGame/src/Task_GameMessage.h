#pragma once
#include "TaskSystem.h"
#include "ImageLoader.h"
#include "ImageDrawer.h"
#include "EasingMover.h"

enum struct MessageType
{
	Ready	= 0,
	Finish	= 1,
	HiScore	= 2,
};

namespace GameMessage
{
	const std::string	defGroupName("ゲームメッセージ");	//グループ名

	//----------------------------------------------
	class Resource
	{
	private:
		static std::weak_ptr<Resource> instance;
		std::string imageName;

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
		EasingMover easeMover;
		ImageDrawer imageDrawer;

		MessageType type;
		TimeCounter waitTimeCnt;
		int progress;

	public:
		//コンストラクタ
		Task(MessageType type, int waitTime);
		
		//デストラクタ
		~Task();
		
		//タスクの生成
		static std::shared_ptr<Task> Create(MessageType type, int waitTime);

		void Initialize();			//初期化処理
		void Finalize() override;	//終了処理
		void Update() override;		//更新
		void Draw() override;		//描画
	};
}