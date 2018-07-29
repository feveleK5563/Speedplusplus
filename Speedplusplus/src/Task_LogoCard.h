#pragma once
#include "TaskSystem.h"
#include "Card.h"

namespace LogoCard
{
	const std::string	defGroupName("ロゴカード");	//グループ名

	//----------------------------------------------
	class Task : public TaskAbstract
	{
	private:
		Card	card;		//カード情報
		int		progress;	//進行度
		bool	LorR;		//カード位置(右か左か)
		bool	isBack;		//カードが裏か否か

	public:
		Task(const CardID& id, const Math::Vec2& pos);		//コンストラクタ
		~Task();	//デストラクタ
		static std::shared_ptr<Task> Create(const CardID& id, const Math::Vec2& pos);	//タスクの生成

		void Initialize();			//初期化処理
		void Finalize() override;	//終了処理
		void Update() override;		//更新
		void Draw() override;		//描画

		//カードが裏だったらtrueを返す
		bool GetIsBack() const;
	};
}