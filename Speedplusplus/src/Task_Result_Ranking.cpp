#include "Task_Result_Ranking.h"
#include "DxLib.h"
#include <fstream>
#include "SystemDefine.h"
#include "GameDefine.h"
#include "Task_GameScore.h"
#include "Task_Sound.h"

namespace Result_Ranking
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
		TaskAbstract(defGroupName, Priority::result),
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
		//スコアの設定
		for (int i = 0; i < rankNum - 1; ++i)
		{
			//初期スコアの設定
			rankData[i].score = 100 * (rankNum - 1 - i);
			settingPos[i] = Math::Vec2(SystemDefine::windowSizeX - 350.f, 300.f + (80 * i));

			rankData[i].numType = NumberType::RankNumber;
			rankData[i].rank = i + 1;
			rankData[i].easeMove = std::make_unique<EasingMover>
				(Math::Vec2(SystemDefine::windowSizeX, settingPos[i].y), settingPos[i],
					1.f, 1.f, 0.f, 0.f);
		}
		//スコアの読み込み
		LoadScoreData();
		
		//プレイヤー用スコアデータを設定
		if (auto score = TS::taskSystem.GetTaskOne<GameScore::Task>(GameScore::defGroupName))
		{
			//ゲーム終了画面でランキング処理が呼ばれた場合、
			//プレイヤー用スコアデータにプレイしたスコアを代入
			rankData[rankNum - 1].score = score->GetScore();
		}
		else
		{
			//それ以外(タイトル画面)で呼ばれた場合、
			//プレイヤー用スコアデータは無効、ランクイン処理をスキップする
			rankData[rankNum - 1].score = -1;
			progress = 2;
		}
		rankData[rankNum - 1].numType = NumberType::HiScore;
		rankData[rankNum - 1].rank = 5 + 1 + 10;
		settingPos[rankNum - 1] = Math::Vec2(SystemDefine::windowSizeX - 350.f, SystemDefine::windowSizeY + 120.f);


		rankRelativePos = Math::Vec2(30.f, -30.f);
		for (int i = 0; i < scoreNum; ++i)
		{
			scoreRelativePos[i] = Math::Vec2((100.f + (i * 35)), -30.f);
		}

		for (int i = 0; i < rankNum - 1; ++i)
		{
			playSoundEffect[i] = true;
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
		bool isEndMove = AppRankMove();

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
			if (Button::PushStartReset())
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

					for (int i = 0; i < rankNum - 1; ++i)
					{
						//戻るときにもSEを鳴らすためにSEの再生状態をリセットする
						playSoundEffect[i] = true;
					}
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
					rankData[i].rank + (int)rankData[i].numType,
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
	//ランキングデータの出現動作
	bool Task::AppRankMove()
	{
		//全ての出現動作が終了したか否かを格納
		bool isEndMove = true;

		for (int i = 0; i < rankNum; ++i)
		{
			if (rankData[i].easeMove == false)
				continue;

			//出現するスコアデータの動きを5フレームずつ遅らせる
			if (moveTimeCnt.GetNowCntTime() > 5 * i)
			{
				if (i < rankNum - 1 && playSoundEffect[i])
				{
					//SEの再生
					auto sound = TS::taskSystem.GetTaskOne<Sound::Task>(Sound::defGroupName);
					sound->PlaySE_HandOut(200);
					playSoundEffect[i] = false;
				}
				isEndMove = rankData[i].easeMove->Update(15.f) && isEndMove;
			}
		}

		return isEndMove;
	}

	//----------------------------------------------
	//スコアランキングの読み込み
	void Task::LoadScoreData()
	{
		std::ifstream ifs;
		ifs.open("scoredata/score.bin", std::ios::in);

		if (!ifs)
		{
			//読み込みに失敗したら新しくファイルを作成する
			WrightScoreData();
			return;
		}

		for (int i = 0; i < rankNum - 1; ++i)
		{
			int scr;
			ifs >> std::hex >> scr;
			rankData[i].score = scr >> 1;
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
			int scr = rankData[i].score << 1;
			ofs << std::hex << scr << " ";
		}
		ofs.close();
	}

	//----------------------------------------------
	//ランクイン時の処理
	void Task::RankIn()
	{
		//ランクインできる位置を検索する
		int rankPosition = 5;
		for (int i = 0; i < rankNum - 1; ++i)
		{
			if (rankData[rankNum - 1].score >= rankData[i].score)
			{
				rankPosition = i;

				auto sound = TS::taskSystem.GetTaskOne<Sound::Task>(Sound::defGroupName);
				sound->PlaySE_PaperTake(200);
				break;
			}
		}

		//プレイヤー用スコアデータを生成する
		rankData[rankNum - 1].rank = rankData[rankPosition].rank;
		rankData[rankNum - 1].easeMove = std::make_unique<EasingMover>(
			Math::Vec2(SystemDefine::windowSizeX, settingPos[rankPosition].y), settingPos[rankPosition],
			1.f, 1.f, 0.f, 0.f);

		//プレイヤースコア以下のデータの移動先とランクを更新
		for (int i = rankPosition; i < rankNum - 1; ++i)
		{
			++rankData[i].rank;

			rankData[i].easeMove->SetEndMove(
				settingPos[i + 1],
				1.f,
				0.f
			);
		}

		//ランクの昇順にソート
		std::sort(rankData.begin(), rankData.end(),
			[](	const RankData& left, const RankData& right)
		{
			return left.rank % 10 < right.rank % 10;
		});

		//スコアをファイルに書き込む
		WrightScoreData();
	}
}