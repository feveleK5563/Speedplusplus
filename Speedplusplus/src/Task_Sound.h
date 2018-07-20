#pragma once
#include "TaskSystem.h"

namespace Sound
{
	const std::string	defGroupName("サウンド");	//グループ名
	const float			defPriority(0.f);			//デフォルトの処理優先度

	//----------------------------------------------
	class Resource
	{
	private:
		static std::weak_ptr<Resource> instance;

	public:
		int bgm;
		int se_turnOver;
		int se_handOut;
		int se_shaffle;
		int se_paperTear;

		Resource();		//コンストラクタ
		~Resource();	//デストラクタ
		static std::shared_ptr<Resource> Create();	//リソースの生成
	};

	//----------------------------------------------
	class Task : public TaskAbstract
	{
	private:
		std::shared_ptr<Resource> res;	//確保したリソース

	public:
		int bgm;
		int se_turnOver;
		int se_handOut;
		int se_shaffle;
		int se_paperTear;

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

		void PlayBGM() const;			//BGMの再生
		void PlaySE_TurnOver(int volume) const;		//SE(カードめくる)の再生
		void PlaySE_HandOut(int volume) const;		//SE(カード投げる)の再生
		void PlaySE_Shaffle(int volume) const;		//SE(シャッフル)の再生
		void PlaySE_PaperTear(int volume) const;	//SE(紙破る)の再生
	};
}