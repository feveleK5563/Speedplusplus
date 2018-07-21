#include "Task_Sound.h"
#include "DxLib.h"

namespace Sound
{
	std::weak_ptr<Resource> Resource::instance;
	//----------------------------------------------
	//リソースのコンストラクタ
	Resource::Resource()
	{
		bgm = LoadSoundMem("data/bgm/放課後はお菓子でもつまみながら的なBGM.ogg");

		se_turnOver		= LoadSoundMem("data/se/card-turn-over1.ogg", 8);
		se_handOut		= LoadSoundMem("data/se/card-hand-out1.ogg", 8);
		se_shaffle		= LoadSoundMem("data/se/card-shuffle2.ogg", 1);
		se_paperTear	= LoadSoundMem("data/se/paper-tear3.ogg", 8);
		se_paperTake	= LoadSoundMem("data/se/paper-take2.ogg", 8);
	}
	//----------------------------------------------
	//リソースのデストラクタ
	Resource::~Resource()
	{
		DeleteSoundMem(bgm);
		DeleteSoundMem(se_turnOver);
		DeleteSoundMem(se_handOut);
		DeleteSoundMem(se_shaffle);
		DeleteSoundMem(se_paperTear);
		DeleteSoundMem(se_paperTake);
	}
	//----------------------------------------------
	//リソースの生成
	std::shared_ptr<Resource> Resource::Create()
	{
		auto sp = instance.lock();
		if (!sp)
		{
			sp = std::make_shared<Resource>();
			instance = sp;
		}
		return sp;
	}

	//☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★
	//★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆

	//----------------------------------------------
	//タスクのコンストラクタ
	Task::Task():
		TaskAbstract(defGroupName, defPriority),
		res(Resource::Create())
	{
	}
	//----------------------------------------------
	//タスクのデストラクタ
	Task::~Task()
	{

	}
	//----------------------------------------------
	//タスクの生成
	std::shared_ptr<Task> Task::Create()
	{
		std::shared_ptr<Task> task = 
			std::make_shared<Task>();
		TS::taskSystem.RegistrationTask(task);

		task->Initialize();
		return task;
	}

	//◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆
	//◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇

	//----------------------------------------------
	//初期化処理
	//----------------------------------------------
	void Task::Initialize()
	{

	}

	//----------------------------------------------
	//終了処理
	//----------------------------------------------
	void Task::Finalize()
	{

	}

	//----------------------------------------------
	//更新
	//----------------------------------------------
	void Task::Update()
	{
		
	}

	//----------------------------------------------
	//描画
	//----------------------------------------------
	void Task::Draw()
	{
		
	}

	//----------------------------------------------
	//BGMの再生
	void Task::PlayBGM() const
	{
		if (!CheckSoundMem(res->bgm))
		{
			PlaySoundMem(res->bgm, DX_PLAYTYPE_LOOP);
		}
	}

	//----------------------------------------------
	//SE(カードめくる)の再生
	void Task::PlaySE_TurnOver(int volume) const
	{
		ChangeNextPlayVolumeSoundMem(volume, res->se_turnOver);
		PlaySoundMem(res->se_turnOver, DX_PLAYTYPE_BACK);
	}
	//----------------------------------------------
	//SE(カード投げる)の再生
	void Task::PlaySE_HandOut(int volume) const
	{
		ChangeNextPlayVolumeSoundMem(volume, res->se_handOut);
		PlaySoundMem(res->se_handOut, DX_PLAYTYPE_BACK);
	}
	//----------------------------------------------
	//SE(シャッフル)の再生
	void Task::PlaySE_Shaffle(int volume) const
	{
		ChangeNextPlayVolumeSoundMem(volume, res->se_shaffle);
		PlaySoundMem(res->se_shaffle, DX_PLAYTYPE_BACK);
	}
	//----------------------------------------------
	//SE(紙破る)の再生
	void Task::PlaySE_PaperTear(int volume) const
	{
		ChangeNextPlayVolumeSoundMem(volume, res->se_paperTear);
		PlaySoundMem(res->se_paperTear, DX_PLAYTYPE_BACK);
	}
	//----------------------------------------------
	//SE(紙めくる)の再生
	void Task::PlaySE_PaperTake(int volume) const
	{
		ChangeNextPlayVolumeSoundMem(volume, res->se_paperTake);
		PlaySoundMem(res->se_paperTake, DX_PLAYTYPE_BACK);
	}
}