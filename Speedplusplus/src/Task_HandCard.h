#pragma once
#include "TaskSystem.h"
#include "Card.h"
#include "Task_GameTimer.h"
#include "GameDefine.h"
#include "SystemDefine.h"

namespace HandCard
{
	const std::string	defGroupName("手札");	//グループ名

	static const Math::Vec2 LeftSidePos(250.f, SystemDefine::windowSizeY - 200.f);
	static const Math::Vec2 RightSidePos(SystemDefine::windowSizeX - 250.f, SystemDefine::windowSizeY - 200.f);

	//----------------------------------------------
	class Task : public TaskAbstract
	{
	private:
		const GameState& gameState;	//ゲームの進行状況
		Card	card;			//カード情報
		int		progress;		//進行度
		const CardDestination&	destination;	//対応するカードの移動先
		Hand					hand;			//初期カード位置(右か左か)

	public:
		//コンストラクタ
		Task(const CardID& id, const Math::Vec2& pos, const CardDestination& destination, Hand hand);

		//デストラクタ
		~Task();
		
		//タスクの生成
		static std::shared_ptr<Task> Create(const CardID& id, const Math::Vec2& pos, const CardDestination& destination, Hand hand);

		void Initialize();			//初期化処理
		void Finalize() override;	//終了処理
		void Update() override;		//更新
		void Draw() override;		//描画

		void MoveOut();		//画面外に移動する
		void MoveCenter();	//中心に移動するカードを生成
		void MoveBump();	//数フレーム中心に移動後画面外へ移動するカードを生成
	};
}