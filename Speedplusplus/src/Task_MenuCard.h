#pragma once
#include <array>
#include "TaskSystem.h"
#include "Card.h"
#include "GameDefine.h"

namespace MenuCard
{
	const std::string	defGroupName("メニュー画面カード");	//グループ名

	//----------------------------------------------
	class Task : public TaskAbstract
	{
	private:
		Mode mode;	//選択中のモード
		std::array<std::unique_ptr<Card>, 2> menuMessageCard;	//メニュー用カード

		const int	seVolume;		//効果音の音量
		Math::Vec2	centerPos;		//中心の座標
		Math::Vec2	startPos[2];	//カードの開始位置
		Math::Vec2	endPos[2];		//カードの移動終了位置

		int progress;			//進行度
		TimeCounter timeCnt;	//時間計測

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

		//モードの取得
		const Mode& GetMode();

	private:
		//モード選択
		void SelectMode();
	};
}