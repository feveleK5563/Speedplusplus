#pragma once
#include "TaskSystem.h"
#include "EasingMover.h"
#include "ImageDrawer.h"

enum struct JEffect
{
	Right = 0,
	Wrong = 1,
};

namespace JudgeEffect
{
	const std::string	defGroupName("結果エフェクト");	//グループ名

	//----------------------------------------------
	class Resource
	{
	private:
		static std::weak_ptr<Resource> instance;

	public:
		ImageData imageData;
		std::string imageName;

		Resource();		//コンストラクタ
		~Resource();	//デストラクタ
		static std::shared_ptr<Resource> Create();	//リソースの生成
	};

	//----------------------------------------------
	class Task : public TaskAbstract
	{
	private:
		std::shared_ptr<Resource> res;	//確保したリソース

		JEffect je; //表示するエフェクトの種類
		ImageDrawer imageDrawer;
		EasingMover easingMover;

		//間違えた場合の震え
		float		shakeWidth;
		Math::Vec2	shakePos;

		int progress;	//進行度
		float alpha;	//アルファ値

	public:
		Task(JEffect je, const Math::Vec2& pos, float degAngle, float moveLength);		//コンストラクタ
		~Task();				//デストラクタ
		static std::shared_ptr<Task> Create(
			JEffect je, const Math::Vec2& pos, float degAngle, float moveLength);	//タスクの生成

		void Initialize();			//初期化処理
		void Finalize() override;	//終了処理
		void Update() override;		//更新
		void Draw() override;		//描画
	};
}