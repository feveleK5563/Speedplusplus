#include "Task_Ranking.h"
#include "DxLib.h"
#include "SystemDefine.h"

namespace Ranking
{
	std::weak_ptr<Resource> Resource::instance;
	//----------------------------------------------
	//リソースのコンストラクタ
	Resource::Resource()
	{
		barName = "BarName";
		numName = "NumName";

		Image::imageLoader.LoadOneImage(barName, "data/image/ResultScoreBar.png");
		Image::imageLoader.LoadDivImage(numName, "data/image/ResultScoreNum.png", 20, 10, 2, 32, 40);
		barImg = Image::imageLoader.GetImageData(barName);
		numImg = Image::imageLoader.GetImageData(numName);
	}
	//----------------------------------------------
	//リソースのデストラクタ
	Resource::~Resource()
	{
		Image::imageLoader.DeleteImageData(barName);
		Image::imageLoader.DeleteImageData(numName);
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
		res(Resource::Create()),
		bar(res->barImg, false),
		number(res->numImg, true),
		moveTimeCnt(-1)
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
		for (int i = 0; i < rankNum; ++i)
		{
			rankScore[i] = i; //仮スコア

			settingPos[i] = Math::Vec2(SystemDefine::windowSizeX - 400, 300 + (60 * i));

			easeMove[i] = std::make_unique<EasingMover>
				(Math::Vec2(SystemDefine::windowSizeX, settingPos[i].y), settingPos[i],
					1.f, 1.f, 0.f, 0.f);
		}

		rankRelativePos = Math::Vec2(30, 30);
		for (int i = 0; i < scoreNum; ++i)
		{
			scoreRelativePos[i] = Math::Vec2(100 + (i * 30), 30);
		}
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
		moveTimeCnt.Run();
		for (int i = 0; i < rankNum; ++i)
		{
			if (moveTimeCnt.GetNowCntTime() > 10 * i)
			{
				easeMove[i]->Update(30.f);
			}
		}
	}

	//----------------------------------------------
	//描画
	//----------------------------------------------
	void Task::Draw()
	{
		for (int i = 0; i < rankNum; ++i)
		{
			//バー
			bar.DrawOne(
				easeMove[i]->GetPos(),
				easeMove[i]->GetScale(),
				easeMove[i]->GetAngle(),
				false, 0,
				Color(255, 255, 255, 255));

			//ランクの数字
			number.DrawOne(
				easeMove[i]->GetPos() + rankRelativePos,
				easeMove[i]->GetAngle(),
				easeMove[i]->GetScale(),
				false,
				10 + i,
				Color(255, 255, 255, 255));

			for (int j = 0; j < scoreNum; ++j)
			{
				//スコア
				int numPlace = (int)powf(10.f, (float)(scoreNum - 1 - i));
				number.DrawOne(
					easeMove[i]->GetPos() + scoreRelativePos[j],
					easeMove[i]->GetAngle(),
					easeMove[i]->GetScale(),
					false,
					(rankScore[i] / numPlace) % 10,
					Color(255, 255, 255, 255));
			}
		}
	}
}