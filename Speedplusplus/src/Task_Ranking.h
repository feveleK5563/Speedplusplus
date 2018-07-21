#pragma once
#include <array>
#include "TaskSystem.h"
#include "ImageDrawer.h"
#include "EasingMover.h"
#include "TimeCounter.h"

namespace Ranking
{
	const std::string	defGroupName("ランキング");	//グループ名
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

		static const int rankNum = 6;	//1位~5位 + プレイヤースコア
		struct RankData
		{
			int								rank;
			std::unique_ptr<EasingMover>	easeMove;
			int								score;
		};
		std::array<RankData, rankNum>	rankData;
		Math::Vec2						settingPos[rankNum];
		bool playSoundEffect[rankNum - 1];	//SE再生フラグ

		static const int scoreNum = 4;			//スコアの表示桁数
		Math::Vec2	rankRelativePos;			//順位数字の表示位置(バーとの相対座標)
		Math::Vec2	scoreRelativePos[scoreNum];	//スコアの表示位置(バーとの相対座標)

		TimeCounter moveTimeCnt;	//動作のための時間経過
		int progress;				//進行度

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

	private:
		bool AppRankMove();		//ランキングデータの出現動作
		void LoadScoreData();	//スコアランキングの読み込み
		void WrightScoreData();	//スコアランキングの書き込み
		void RankIn();			//ランクに登録する
	};
}