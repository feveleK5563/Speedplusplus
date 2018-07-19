#pragma once
#include "TaskSystem.h"
#include "ImageDrawer.h"
#include "EasingMover.h"
#include "TimeCounter.h"

namespace Ranking
{
	const std::string	defGroupName("ランキング");	//グループ名
	const float			defPriority(0.f);			//デフォルトの処理優先度

	//----------------------------------------------
	class Resource
	{
	private:
		static std::weak_ptr<Resource> instance;
		std::string barName;
		std::string numName;

	public:
		ImageData barImg;
		ImageData numImg;

		Resource();		//コンストラクタ
		~Resource();	//デストラクタ
		static std::shared_ptr<Resource> Create();	//リソースの生成
	};

	//----------------------------------------------
	class Task : public TaskAbstract
	{
	private:
		std::shared_ptr<Resource> res;	//確保したリソース
		ImageDrawer bar, number;

		static const int rankNum = 5;	//1位~5位
		int								rankScore[rankNum];
		std::unique_ptr<EasingMover>	easeMove[rankNum];
		Math::Vec2						settingPos[rankNum];

		static const int scoreNum = 4;			//スコアの表示桁数
		Math::Vec2	rankRelativePos;			//順位数字の表示位置(バーとの相対座標)
		Math::Vec2	scoreRelativePos[scoreNum];	//スコアの表示位置(バーとの相対座標)

		TimeCounter moveTimeCnt;	//動作のための時間経過

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