#pragma once
#include "TaskSystem.h"
#include "TimeCounter.h"
#include "CardCounter.h"

namespace GameScore
{
	const std::string	defGroupName("ゲームスコア");	//グループ名

	//----------------------------------------------
	class Task : public TaskAbstract
	{
	private:
		static const int cardNum = 4;	//カード枚数
		TimeCounter timeCnt;			//時間計測
		bool		isUseCombo;			//コンボを使用するか否か
		bool		isRedScore;			//スコアカードを赤数字にするか否か
		int			combo;				//コンボ数
		int			score;				//スコア数
		Math::Vec2	startPos;			//初期座標
		Math::Vec2	endPos;				//移動先座標
		float		size;				//カードのサイズ

		std::unique_ptr<CardCounter> comboCardCnt;			//コンボの表示用カード
		std::unique_ptr<CardCounter> scoreCardCnt[cardNum];	//スコアの表示用カード
		const int&			scoreFluctuation;				//正誤状況

	public:
		//コンストラクタ
		Task(const int& setScore, bool isUseCombo, bool isRedScore,
			const Math::Vec2& startPos, const Math::Vec2& endPos, float size);
		
		//デストラクタ
		~Task();
		
		//タスクの生成
		static std::shared_ptr<Task> Create(const int& setScore, bool isUseCombo, bool isRedScore,
			const Math::Vec2& startPos, const Math::Vec2& endPos, float size);

		void Initialize();			//初期化処理
		void Finalize() override;	//終了処理
		void Update() override;		//更新
		void Draw() override;		//描画

	private:
		void ScoreUpdate();

	public:
		const int& GetScore() const;
	};
}