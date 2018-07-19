#include "Task_Ranking.h"
#include "DxLib.h"
#include <fstream>
#include "SystemDefine.h"
#include "GameDefine.h"
#include "Task_GameScore.h"

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
		Image::imageLoader.LoadDivImage(numName, "data/image/ResultScoreNum.png", 30, 10, 3, 32, 40);
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
		TaskAbstract(defGroupName, Priority::ranking),
		res(Resource::Create()),
		bar(res->barImg, false),
		number(res->numImg, true),
		moveTimeCnt(-1),
		progress(0)
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
		for (int i = 0; i < rankNum - 1; ++i)
		{
			rankData[i].score = 10 * (rankNum - 1 - i);	//初期スコア
			settingPos[i] = Math::Vec2(SystemDefine::windowSizeX - 350.f, 300.f + (80 * i));

			rankData[i].rank = i + 1 + 10;
			rankData[i].easeMove = std::make_unique<EasingMover>
				(Math::Vec2(SystemDefine::windowSizeX, settingPos[i].y), settingPos[i],
					1.f, 1.f, 0.f, 0.f);
		}
		LoadScoreData();

		if (auto score = TS::taskSystem.GetTaskOne<GameScore::Task>(GameScore::defGroupName))
		{
			rankData[rankNum - 1].score = score->GetScore();
		}
		else
		{
			rankData[rankNum - 1].score = -1;
			progress += 2;
		}
		rankData[rankNum - 1].rank = 5 + 1 + 10;
		settingPos[rankNum - 1] = Math::Vec2(SystemDefine::windowSizeX - 350.f, SystemDefine::windowSizeY + 120.f);


		rankRelativePos = Math::Vec2(30.f, -30.f);
		for (int i = 0; i < scoreNum; ++i)
		{
			scoreRelativePos[i] = Math::Vec2((100.f + (i * 35)), -30.f);
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
		bool isEndMove = true;

		for (int i = 0; i < rankNum; ++i)
		{
			if (rankData[i].easeMove == false)
				continue;

			if (moveTimeCnt.GetNowCntTime() > 5 * i)
			{
				isEndMove = rankData[i].easeMove->Update(15.f) && isEndMove;
			}
		}

		switch (progress)
		{
		case 0:	//ランクインしている場合は登録
			if (isEndMove)
			{
				if (moveTimeCnt.GetNowCntTime() > 90.f)
				{
					RankIn();
					++progress;
				}
			}
			break;

		case 1:	//待機
			if (isEndMove)
			{
				++progress;
			}
			break;

		case 2:	//入力待ちから終了
			if (PushStart())
			{
				++progress;
				moveTimeCnt.ResetCntTime();
				for (int i = 0; i < rankNum; ++i)
				{
					if (rankData[i].easeMove == false)
						continue;

					rankData[i].easeMove->SetEndMove(
						Math::Vec2(SystemDefine::windowSizeX, rankData[i].easeMove->GetPos().y),
						1.f, 0.f
					);
				}
			}
			break;

		case 3:	//引っ込んで消滅
			if (isEndMove)
			{
				KillMe();
			}
			break;
		}
	}

	//----------------------------------------------
	//描画
	//----------------------------------------------
	void Task::Draw()
	{
		for (int i = 0; i < rankNum; ++i)
		{
			if (rankData[i].easeMove == false)
				continue;

			//バー
			bar.DrawOne(
				rankData[i].easeMove->GetPos(),
				rankData[i].easeMove->GetScale(),
				rankData[i].easeMove->GetAngle(),
				false, 0,
				Color(255, 255, 255, 255));

			//ランク
			if (rankData[i].rank % 10 < rankNum)
			{
				number.DrawOne(
					rankData[i].easeMove->GetPos() + rankRelativePos,
					rankData[i].easeMove->GetScale(),
					rankData[i].easeMove->GetAngle(),
					false,
					rankData[i].rank,
					Color(255, 255, 255, 255));
			}

			for (int j = 0; j < scoreNum; ++j)
			{
				//スコア
				int numPlace = (int)powf(10.f, (float)(scoreNum - 1 - j));
				number.DrawOne(
					rankData[i].easeMove->GetPos() + scoreRelativePos[j],
					rankData[i].easeMove->GetScale(),
					rankData[i].easeMove->GetAngle(),
					false,
					(rankData[i].score / numPlace) % 10,
					Color(255, 255, 255, 255));
			}
		}
	}

	//----------------------------------------------
	//スコアランキングの読み込み
	void Task::LoadScoreData()
	{
		std::ifstream ifs;
		ifs.open("scoredata/score.bin", std::ios::in);

		if (!ifs)
		{
			//読み込みに失敗したらファイルを作成する
			WrightScoreData();
			return;
		}

		for (int i = 0; i < rankNum - 1; ++i)
		{
			ifs >> rankData[i].score;
		}
		ifs.close();
	}

	//----------------------------------------------
	//スコアランキングの書き込み
	void Task::WrightScoreData()
	{
		std::ofstream ofs;
		ofs.open("scoredata/score.bin", std::ios::out);

		if (!ofs)
		{
			//ファイルの作成エラー
			return;
		}

		for (int i = 0; i < rankNum - 1; ++i)
		{
			ofs << rankData[i].score << " ";
		}
		ofs.close();
	}

	//----------------------------------------------
	//ランクイン
	void Task::RankIn()
	{
		int rankPosition = 5;
		for (int i = 0; i < rankNum - 1; ++i)
		{
			if (rankData[rankNum - 1].score >= rankData[i].score)
			{
				rankPosition = i;
				break;
			}
		}

		rankData[rankNum - 1].rank = rankData[rankPosition].rank + 10;
		rankData[rankNum - 1].easeMove = std::make_unique<EasingMover>(
			Math::Vec2(SystemDefine::windowSizeX, settingPos[rankPosition].y), settingPos[rankPosition],
			1.f, 1.f, 0.f, 0.f);

		for (int i = rankPosition; i < rankNum - 1; ++i)
		{
			++rankData[i].rank;

			rankData[i].easeMove->SetEndMove(
				settingPos[i + 1],
				1.f,
				0.f
			);
		}

		//昇順にソート
		std::sort(rankData.begin(), rankData.end(),
			[](	const RankData& left, const RankData& right)
		{
			return left.rank % 10 < right.rank % 10;
		});

		WrightScoreData();
	}
}